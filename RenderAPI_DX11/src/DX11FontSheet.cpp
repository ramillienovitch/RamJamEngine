#include "DX11FontSheet.h"

#include <Gdiplus.h>

#include "Memory.h"

//////////////////////////////////////////////////////////////////////////
DX11FontSheet::DX11FontSheet()
{
	mFontSheetTex = nullptr;
	mFontSheetSRV = nullptr;
}

//////////////////////////////////////////////////////////////////////////
DX11FontSheet::~DX11FontSheet()
{
	RJE_SAFE_RELEASE(mFontSheetTex);
	RJE_SAFE_RELEASE(mFontSheetSRV);
}

//////////////////////////////////////////////////////////////////////////
ID3D11ShaderResourceView* DX11FontSheet::GetFontSheetSRV()
{
	RJE_ASSERT(mInitialized);
	return mFontSheetSRV;
}

//////////////////////////////////////////////////////////////////////////
const CD3D11_RECT& DX11FontSheet::GetCharBoundingRect( WCHAR c )
{
	RJE_ASSERT(mInitialized);
	return mCharRects[c - StartChar];
}

//////////////////////////////////////////////////////////////////////////
const int DX11FontSheet::GetMaxCharWidth()
{
	RJE_ASSERT(mInitialized);
	return mMaxCharWidth;
}

//////////////////////////////////////////////////////////////////////////
HRESULT DX11FontSheet::Initialize( ID3D11Device* device, const std::wstring& fontName, float pixelFontSize, FontStyle fontStyle, BOOL antiAliased )
{
	// Prevent double Init.
	RJE_ASSERT(!mInitialized);

	HRESULT hr = S_OK;

	// Init GDI+
	ULONG_PTR token = NULL;
	Gdiplus::GdiplusStartupInput  startupInput(NULL, TRUE, TRUE);
	Gdiplus::GdiplusStartupOutput startupOutput;
	Gdiplus::GdiplusStartup(&token, &startupInput, &startupOutput);

	// Create a new scope so all GDI+ objects are destroyed before we shutdown GDI+.
	{
		Gdiplus::Font				font(fontName.c_str(), pixelFontSize, fontStyle, Gdiplus::UnitPixel);
		Gdiplus::TextRenderingHint	hint = antiAliased ? Gdiplus::TextRenderingHintAntiAlias : Gdiplus::TextRenderingHintSystemDefault;

		// Bitmap for drawing a single char.
		int tempSize = static_cast<int>(pixelFontSize * 2);
		Gdiplus::Bitmap		charBitmap(tempSize, tempSize, PixelFormat32bppARGB);
		Gdiplus::Graphics	charGraphics(&charBitmap);
		charGraphics.SetPageUnit(Gdiplus::UnitPixel);
		charGraphics.SetTextRenderingHint(hint);
		
		// Determine mCharHeight, mSpaceWidth, and mTexHeight.
		MeasureChars(font, charGraphics);

		// Bitmap for storing all the char sprites on a sprite sheet.
		Gdiplus::Bitmap		fontSheetBitmap(mTexWidth, mTexHeight, PixelFormat32bppARGB);
		Gdiplus::Graphics	fontSheetGraphics(&fontSheetBitmap);
		fontSheetGraphics.SetCompositingMode(Gdiplus::CompositingModeSourceCopy);
		fontSheetGraphics.Clear(Gdiplus::Color(0, 0, 0, 0));

		BuildFontSheetBitmap(font, charGraphics, charBitmap, fontSheetGraphics);

#ifdef RJE_DEBUG_FONT_SHEET
		CLSID clsid;
		GetEncoderClsid(L"image/bmp", &clsid);
		int szSize = (int)fontName.size();
		WCHAR* filename = new WCHAR[szSize+5];
		for (int i = 0 ; i<szSize ; ++i)
			filename[i] = fontName[i];
		filename[szSize]   = '.';
		filename[szSize+1] = 'b';
		filename[szSize+2] = 'm';
		filename[szSize+3] = 'p';
		filename[szSize+4] = nullchar;
		fontSheetBitmap.Save(filename, &clsid, NULL);
		delete filename;
#endif

		if(FAILED(BuildFontSheetTexture(device, fontSheetBitmap)))
		{
			Gdiplus::GdiplusShutdown(token);
			return hr;
		}
	}

	// Shutdown GDI+: You must delete all of your GDI+ objects (or have them go out of scope) before you call GdiplusShutdown.
	Gdiplus::GdiplusShutdown(token);

	mInitialized = true;

	return hr;
}

//////////////////////////////////////////////////////////////////////////
HRESULT DX11FontSheet::BuildFontSheetBitmap( Gdiplus::Font& font, Gdiplus::Graphics& charGraphics, Gdiplus::Bitmap& charBitmap, Gdiplus::Graphics& fontSheetGraphics )
{
	WCHAR charString[2] = {' ', 0};
	Gdiplus::SolidBrush whiteBrush(Gdiplus::Color(255, 255, 255, 255));
	UINT fontSheetX   = 0;
	UINT fontSheetY   = 0;
	UINT maxCharWidth = 0;
	for(UINT i = 0; i < NumChars; ++i)
	{
		charString[0] = static_cast<WCHAR>(StartChar + i);
		charGraphics.Clear(Gdiplus::Color(0, 0, 0, 0));
		charGraphics.DrawString(charString, 1, &font, Gdiplus::PointF(0.0f, 0.0f), &whiteBrush);

		// Compute tight char horizontal bounds (ignoring empty space).
		int minX = GetCharMinX(charBitmap);
		int maxX = GetCharMaxX(charBitmap);
		int charWidth = maxX - minX + 1;
		if (maxCharWidth < charWidth)
			maxCharWidth = charWidth;

		// Move to next row of the font sheet?
		if(fontSheetX + charWidth >= mTexWidth)
		{
			fontSheetX = 0;
			fontSheetY += static_cast<int>(mCharHeight) + 1;
		}

		// Save the rectangle of this character on the texture atlas.
		mCharRects[i] = CD3D11_RECT(fontSheetX, fontSheetY, fontSheetX + charWidth, fontSheetY + mCharHeight);

		// The rectangle subset of the source image to copy.
		fontSheetGraphics.DrawImage(&charBitmap, fontSheetX, fontSheetY, minX, 0, charWidth, mCharHeight, Gdiplus::UnitPixel);

		// next char
		fontSheetX += charWidth + 1;
	}
	mMaxCharWidth = maxCharWidth;

	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT DX11FontSheet::BuildFontSheetTexture( ID3D11Device* device, Gdiplus::Bitmap& fontSheetBitmap )
{
	HRESULT hr = S_OK;

	// Lock the bitmap for direct memory access
	Gdiplus::BitmapData bmData;
	fontSheetBitmap.LockBits(&Gdiplus::Rect(0, 0, mTexWidth, mTexHeight), Gdiplus::ImageLockModeRead, PixelFormat32bppARGB, &bmData);  

	// Copy into a texture.
	D3D11_TEXTURE2D_DESC texDesc;
	texDesc.Width              = mTexWidth;
	texDesc.Height             = mTexHeight;
	texDesc.MipLevels          = 1;
	texDesc.ArraySize          = 1;
	texDesc.Format             = DXGI_FORMAT_B8G8R8A8_UNORM;
	texDesc.SampleDesc.Count   = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage              = D3D11_USAGE_IMMUTABLE;
	texDesc.BindFlags          = D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags     = 0;
	texDesc.MiscFlags          = 0;

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem          = bmData.Scan0;
	data.SysMemPitch      = mTexWidth * 4;
	data.SysMemSlicePitch = 0;

	hr = device->CreateTexture2D(&texDesc, &data, &mFontSheetTex);
	if(FAILED(hr))
		return hr;

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format                    = DXGI_FORMAT_B8G8R8A8_UNORM;
	srvDesc.ViewDimension             = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels       = 1;
	srvDesc.Texture2D.MostDetailedMip = 0;

	hr = device->CreateShaderResourceView(mFontSheetTex, &srvDesc, &mFontSheetSRV);
	if(FAILED(hr))
		return hr;

	fontSheetBitmap.UnlockBits(&bmData);  

	return hr;
}
