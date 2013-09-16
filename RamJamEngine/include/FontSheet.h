#pragma once

#include "Types.h"
#include "Debug.h"

// Save font sheet as .bmp file for debugging
//#define RJE_DEBUG_FONT_SHEET

namespace Gdiplus
{
	class Bitmap;
	class Font;
	class Graphics;
}

/// A font sheet is a texture atlas that stores characters for the given font
/// to be used with the SpriteBatch class for rendering text. 
///
/// There is a fair amount of cost to create the font sheet, as we use GDI+ to 
/// render out each character, and then copy the sheet to a texture.  Therefore, 
/// all the FontSheets an application needs should be created at initialization
/// time.  This could even be moved to a pre-build step if we had a content pipeline.


class FontSheet
{
public:
	// Mirrors the Gdi+ FontStyle enum.
	enum FontStyle 
	{
		FontStyleRegular      = 0,
		FontStyleBold         = 1,
		FontStyleItalic       = 2,
		FontStyleBoldItalic   = 3,
		FontStyleUnderline    = 4,
		FontStyleStrikeout    = 8 
	};

	FontSheet();
	virtual ~FontSheet();

	/// Gets the width of the "space" character.  This tells the SpriteBatch
	/// how much space to skip when rendering space characters.
	int GetSpaceWidth();

	/// Returns the character height for the font.  This should be used for
	/// newline characters when rendering text.
	int GetCharHeight();

	// ASCII characters from 33='!' to 127.  
	static const WCHAR StartChar = 33;
	static const WCHAR EndChar   = 127;
	static const UINT  NumChars  = EndChar - StartChar;

protected:

	/// Determines mCharHeight, mSpaceWidth, and mTexHeight.
	void MeasureChars(Gdiplus::Font& font, Gdiplus::Graphics& charGraphics);

	int GetCharMinX(Gdiplus::Bitmap& charBitmap);
	int GetCharMaxX(Gdiplus::Bitmap& charBitmap);

	/// For saving the GDI Bitmap to file (for internal debugging).
	int GetEncoderClsid(const WCHAR* format, CLSID* pClsid);

	bool mInitialized;

	UINT mTexWidth;
	UINT mTexHeight;

	int mSpaceWidth;
	int mCharHeight; 
};
