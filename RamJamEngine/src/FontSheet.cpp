#include "FontSheet.h"

#include <Gdiplus.h>

//////////////////////////////////////////////////////////////////////////
FontSheet::FontSheet()
{
	mInitialized  = false;
	mTexWidth     = 1024;

	// To be calculated
	mTexHeight    = 0;
	mSpaceWidth   = 0;
	mCharHeight   = 0;
}

FontSheet::~FontSheet() {}

//////////////////////////////////////////////////////////////////////////
int FontSheet::GetSpaceWidth()
{
	RJE_ASSERT(mInitialized);
	return mSpaceWidth;
}

//////////////////////////////////////////////////////////////////////////
int FontSheet::GetCharHeight()
{
	RJE_ASSERT(mInitialized);
	return mCharHeight;
}

//////////////////////////////////////////////////////////////////////////
void FontSheet::MeasureChars( Gdiplus::Font& font, Gdiplus::Graphics& charGraphics )
{
	WCHAR allChars[NumChars + 1];
	for(WCHAR i = 0; i < NumChars; ++i)
		allChars[i] = StartChar + i;
	allChars[NumChars] = 0;

	Gdiplus::RectF sizeRect;
	charGraphics.MeasureString(allChars, NumChars, &font, Gdiplus::PointF(0, 0), &sizeRect);
	mCharHeight = static_cast<int>(sizeRect.Height + 0.5f);  

	// Given the fixed texture width, figure out how many rows we need to draw
	// all the characters, and consequently how much texture height we need.
	int numRows = static_cast<int>(sizeRect.Width / mTexWidth) + 1;  
	mTexHeight  = static_cast<int>(numRows*mCharHeight) + 1;

	// Measure space character (which we do not store in the atlas).
	WCHAR charString[2] = {' ', 0};
	charGraphics.MeasureString(charString, 1, &font, Gdiplus::PointF(0, 0), &sizeRect);
	mSpaceWidth = static_cast<int>(sizeRect.Width + 0.5f);
}

//////////////////////////////////////////////////////////////////////////
int FontSheet::GetCharMinX( Gdiplus::Bitmap& charBitmap )
{
	int width  = charBitmap.GetWidth();
	int height = charBitmap.GetHeight();

	for(int x = 0; x < width; ++x)
	{
		for(int y = 0; y < height; ++y)
		{
			Gdiplus::Color color;
			charBitmap.GetPixel(x, y, &color);
			if(color.GetAlpha() > 0)
				return x;
		}
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////////
int FontSheet::GetCharMaxX( Gdiplus::Bitmap& charBitmap )
{
	int width  = charBitmap.GetWidth();
	int height = charBitmap.GetHeight();

	for(int x = width-1; x >= 0; --x)
	{
		for(int y = 0; y < height; ++y)
		{
			Gdiplus::Color color;
			charBitmap.GetPixel(x, y, &color);
			if(color.GetAlpha() > 0)
				return x;
		}
	}

	return width-1;
}

//////////////////////////////////////////////////////////////////////////
int FontSheet::GetEncoderClsid( const WCHAR* format, CLSID* pClsid )
{
	UINT  num = 0;          // number of image encoders
	UINT  size = 0;         // size of the image encoder array in bytes

	Gdiplus::ImageCodecInfo* pImageCodecInfo = nullptr;

	Gdiplus::GetImageEncodersSize(&num, &size);
	if(size == 0)
		return -1;  // Failure

	pImageCodecInfo = (Gdiplus::ImageCodecInfo*)(malloc(size));
	if(pImageCodecInfo == nullptr)
		return -1;  // Failure

	GetImageEncoders(num, size, pImageCodecInfo);

	for(UINT j = 0; j < num; ++j)
	{
		if( wcscmp(pImageCodecInfo[j].MimeType, format) == 0 )
		{
			*pClsid = pImageCodecInfo[j].Clsid;
			free(pImageCodecInfo);
			return j;  // Success
		}    
	}

	free(pImageCodecInfo);
	return -1;  // Failure
}
