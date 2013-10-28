#pragma once

#include "DX11Helper.h"

#include "../../RamJamEngine/include/FontSheet.h"

class DX11FontSheet : public FontSheet
{
public:
	DX11FontSheet();
	virtual ~DX11FontSheet();

	ID3D11ShaderResourceView* GetFontSheetSRV();

	const CD3D11_RECT&	GetCharBoundingRect(WCHAR c);
	const int			GetMaxCharWidth();

	HRESULT Initialize(ID3D11Device* device, const std::wstring& fontName, float pixelFontSize, FontStyle fontStyle, BOOL antiAliased);
	HRESULT BuildFontSheetBitmap( Gdiplus::Font& font, Gdiplus::Graphics& charGraphics, Gdiplus::Bitmap& charBitmap, Gdiplus::Graphics& fontSheetGraphics);
	HRESULT BuildFontSheetTexture(ID3D11Device* device, Gdiplus::Bitmap& fontSheetBitmap);

private:
	ID3D11Texture2D*          mFontSheetTex;
	ID3D11ShaderResourceView* mFontSheetSRV;

	CD3D11_RECT mCharRects[NumChars];
	int mMaxCharWidth;
};