#pragma once

#include "Types.h"

namespace RJE_GLOBALS
{
#	define MAX_STRING_DBG			4096
#	define VERBOSE_CRITICAL			0
#	define VERBOSE_ERROR			1
#	define VERBOSE_WARNING			2
#	define VERBOSE_MESSAGE			3
#	define VERBOSE_ALL				4

	namespace CONSOLE
	{
#		define CONSOLE_HEIGHT			200
#		define COMMAND_MAX_LENGTH		128
#		define HISTORIC_LENGTH			64
#		define LINE_MAX					8
	}

	namespace SCREEN_INFO
	{
#		define SCREEN_WHITE			1
#		define SCREEN_RED			2
#		define SCREEN_GREEN			3
#		define SCREEN_BLUE			4
#		define SCREEN_BLACK			5
	}

#	define nullchar		'\0'
#	define newline		'\n'
	
	// -----------------------------------------------------------------------

	//************************************************************************
	//	Rendering
	//************************************************************************
	extern BOOL		gFullScreen;
	extern BOOL		gVsyncEnabled;
	extern int		gScreenWidth;
	extern int		gScreenHeight;
	extern BOOL		gEnableMultiSampling;
	extern int		gMultiSamplingCount;
	extern int		gMultisamplingQuality;
	extern BOOL		gUse4xMsaa;
	extern u32		g4xMsaaQuality;

	//************************************************************************
	//	Debug
	//************************************************************************
	extern int		gDebugVerbosity;
	extern BOOL		gShowCursor;

	//************************************************************************
	//	Misc
	//************************************************************************
	extern BOOL		gRunInBackground;
	extern BOOL		gUpdateSceneRuntime;

	namespace Colors
	{
		//-------- Opaque
		const float AliceBlue            [4] = {0.941176534f, 0.972549081f, 1.000000000f, 1.000000000f};
		const float AntiqueWhite         [4] = {0.980392218f, 0.921568692f, 0.843137324f, 1.000000000f};
		const float Aqua                 [4] = {0.000000000f, 1.000000000f, 1.000000000f, 1.000000000f};
		const float Aquamarine           [4] = {0.498039246f, 1.000000000f, 0.831372619f, 1.000000000f};
		const float Azure                [4] = {0.941176534f, 1.000000000f, 1.000000000f, 1.000000000f};
		const float Beige                [4] = {0.960784376f, 0.960784376f, 0.862745166f, 1.000000000f};
		const float Bisque               [4] = {1.000000000f, 0.894117713f, 0.768627524f, 1.000000000f};
		const float Black                [4] = {0.000000000f, 0.000000000f, 0.000000000f, 1.000000000f};
		const float BlanchedAlmond       [4] = {1.000000000f, 0.921568692f, 0.803921640f, 1.000000000f};
		const float Blue                 [4] = {0.000000000f, 0.000000000f, 1.000000000f, 1.000000000f};
		const float BlueViolet           [4] = {0.541176498f, 0.168627456f, 0.886274576f, 1.000000000f};
		const float Brown                [4] = {0.647058845f, 0.164705887f, 0.164705887f, 1.000000000f};
		const float BurlyWood            [4] = {0.870588303f, 0.721568644f, 0.529411793f, 1.000000000f};
		const float CadetBlue            [4] = {0.372549027f, 0.619607866f, 0.627451003f, 1.000000000f};
		const float Chartreuse           [4] = {0.498039246f, 1.000000000f, 0.000000000f, 1.000000000f};
		const float Chocolate            [4] = {0.823529482f, 0.411764741f, 0.117647067f, 1.000000000f};
		const float Coral                [4] = {1.000000000f, 0.498039246f, 0.313725501f, 1.000000000f};
		const float CornflowerBlue       [4] = {0.392156899f, 0.584313750f, 0.929411829f, 1.000000000f};
		const float Cornsilk             [4] = {1.000000000f, 0.972549081f, 0.862745166f, 1.000000000f};
		const float Crimson              [4] = {0.862745166f, 0.078431375f, 0.235294133f, 1.000000000f};
		const float Cyan                 [4] = {0.000000000f, 1.000000000f, 1.000000000f, 1.000000000f};
		const float DarkBlue             [4] = {0.000000000f, 0.000000000f, 0.545098066f, 1.000000000f};
		const float DarkCyan             [4] = {0.000000000f, 0.545098066f, 0.545098066f, 1.000000000f};
		const float DarkGoldenrod        [4] = {0.721568644f, 0.525490224f, 0.043137256f, 1.000000000f};
		const float DarkGray             [4] = {0.662745118f, 0.662745118f, 0.662745118f, 1.000000000f};
		const float DarkGreen            [4] = {0.000000000f, 0.392156899f, 0.000000000f, 1.000000000f};
		const float DarkKhaki            [4] = {0.741176486f, 0.717647076f, 0.419607878f, 1.000000000f};
		const float DarkMagenta          [4] = {0.545098066f, 0.000000000f, 0.545098066f, 1.000000000f};
		const float DarkOliveGreen       [4] = {0.333333343f, 0.419607878f, 0.184313729f, 1.000000000f};
		const float DarkOrange           [4] = {1.000000000f, 0.549019635f, 0.000000000f, 1.000000000f};
		const float DarkOrchid           [4] = {0.600000024f, 0.196078449f, 0.800000072f, 1.000000000f};
		const float DarkRed              [4] = {0.545098066f, 0.000000000f, 0.000000000f, 1.000000000f};
		const float DarkSalmon           [4] = {0.913725555f, 0.588235319f, 0.478431404f, 1.000000000f};
		const float DarkSeaGreen         [4] = {0.560784340f, 0.737254918f, 0.545098066f, 1.000000000f};
		const float DarkSlateBlue        [4] = {0.282352954f, 0.239215702f, 0.545098066f, 1.000000000f};
		const float DarkSlateGray        [4] = {0.184313729f, 0.309803933f, 0.309803933f, 1.000000000f};
		const float DarkTurquoise        [4] = {0.000000000f, 0.807843208f, 0.819607913f, 1.000000000f};
		const float DarkViolet           [4] = {0.580392182f, 0.000000000f, 0.827451050f, 1.000000000f};
		const float DeepPink             [4] = {1.000000000f, 0.078431375f, 0.576470613f, 1.000000000f};
		const float DeepSkyBlue          [4] = {0.000000000f, 0.749019623f, 1.000000000f, 1.000000000f};
		const float DimGray              [4] = {0.411764741f, 0.411764741f, 0.411764741f, 1.000000000f};
		const float DodgerBlue           [4] = {0.117647067f, 0.564705908f, 1.000000000f, 1.000000000f};
		const float Firebrick            [4] = {0.698039234f, 0.133333340f, 0.133333340f, 1.000000000f};
		const float FloralWhite          [4] = {1.000000000f, 0.980392218f, 0.941176534f, 1.000000000f};
		const float ForestGreen          [4] = {0.133333340f, 0.545098066f, 0.133333340f, 1.000000000f};
		const float Fuchsia              [4] = {1.000000000f, 0.000000000f, 1.000000000f, 1.000000000f};
		const float Gainsboro            [4] = {0.862745166f, 0.862745166f, 0.862745166f, 1.000000000f};
		const float GhostWhite           [4] = {0.972549081f, 0.972549081f, 1.000000000f, 1.000000000f};
		const float Gold                 [4] = {1.000000000f, 0.843137324f, 0.000000000f, 1.000000000f};
		const float Goldenrod            [4] = {0.854902029f, 0.647058845f, 0.125490203f, 1.000000000f};
		const float Gray                 [4] = {0.501960814f, 0.501960814f, 0.501960814f, 1.000000000f};
		const float Green                [4] = {0.000000000f, 0.501960814f, 0.000000000f, 1.000000000f};
		const float GreenYellow          [4] = {0.678431392f, 1.000000000f, 0.184313729f, 1.000000000f};
		const float Honeydew             [4] = {0.941176534f, 1.000000000f, 0.941176534f, 1.000000000f};
		const float HotPink              [4] = {1.000000000f, 0.411764741f, 0.705882370f, 1.000000000f};
		const float IndianRed            [4] = {0.803921640f, 0.360784322f, 0.360784322f, 1.000000000f};
		const float Indigo               [4] = {0.294117659f, 0.000000000f, 0.509803951f, 1.000000000f};
		const float Ivory                [4] = {1.000000000f, 1.000000000f, 0.941176534f, 1.000000000f};
		const float Khaki                [4] = {0.941176534f, 0.901960850f, 0.549019635f, 1.000000000f};
		const float Lavender             [4] = {0.901960850f, 0.901960850f, 0.980392218f, 1.000000000f};
		const float LavenderBlush        [4] = {1.000000000f, 0.941176534f, 0.960784376f, 1.000000000f};
		const float LawnGreen            [4] = {0.486274540f, 0.988235354f, 0.000000000f, 1.000000000f};
		const float LemonChiffon         [4] = {1.000000000f, 0.980392218f, 0.803921640f, 1.000000000f};
		const float LightBlue            [4] = {0.678431392f, 0.847058892f, 0.901960850f, 1.000000000f};
		const float LightCoral           [4] = {0.941176534f, 0.501960814f, 0.501960814f, 1.000000000f};
		const float LightCyan            [4] = {0.878431439f, 1.000000000f, 1.000000000f, 1.000000000f};
		const float LightGoldenrodYellow [4] = {0.980392218f, 0.980392218f, 0.823529482f, 1.000000000f};
		const float LightGreen           [4] = {0.564705908f, 0.933333397f, 0.564705908f, 1.000000000f};
		const float LightGray            [4] = {0.827451050f, 0.827451050f, 0.827451050f, 1.000000000f};
		const float LightPink            [4] = {1.000000000f, 0.713725507f, 0.756862819f, 1.000000000f};
		const float LightSalmon          [4] = {1.000000000f, 0.627451003f, 0.478431404f, 1.000000000f};
		const float LightSeaGreen        [4] = {0.125490203f, 0.698039234f, 0.666666687f, 1.000000000f};
		const float LightSkyBlue         [4] = {0.529411793f, 0.807843208f, 0.980392218f, 1.000000000f};
		const float LightSlateGray       [4] = {0.466666698f, 0.533333361f, 0.600000024f, 1.000000000f};
		const float LightSteelBlue       [4] = {0.690196097f, 0.768627524f, 0.870588303f, 1.000000000f};
		const float LightYellow          [4] = {1.000000000f, 1.000000000f, 0.878431439f, 1.000000000f};
		const float Lime                 [4] = {0.000000000f, 1.000000000f, 0.000000000f, 1.000000000f};
		const float LimeGreen            [4] = {0.196078449f, 0.803921640f, 0.196078449f, 1.000000000f};
		const float Linen                [4] = {0.980392218f, 0.941176534f, 0.901960850f, 1.000000000f};
		const float Magenta              [4] = {1.000000000f, 0.000000000f, 1.000000000f, 1.000000000f};
		const float Maroon               [4] = {0.501960814f, 0.000000000f, 0.000000000f, 1.000000000f};
		const float MediumAquamarine     [4] = {0.400000036f, 0.803921640f, 0.666666687f, 1.000000000f};
		const float MediumBlue           [4] = {0.000000000f, 0.000000000f, 0.803921640f, 1.000000000f};
		const float MediumOrchid         [4] = {0.729411781f, 0.333333343f, 0.827451050f, 1.000000000f};
		const float MediumPurple         [4] = {0.576470613f, 0.439215720f, 0.858823597f, 1.000000000f};
		const float MediumSeaGreen       [4] = {0.235294133f, 0.701960802f, 0.443137288f, 1.000000000f};
		const float MediumSlateBlue      [4] = {0.482352972f, 0.407843173f, 0.933333397f, 1.000000000f};
		const float MediumSpringGreen    [4] = {0.000000000f, 0.980392218f, 0.603921592f, 1.000000000f};
		const float MediumTurquoise      [4] = {0.282352954f, 0.819607913f, 0.800000072f, 1.000000000f};
		const float MediumVioletRed      [4] = {0.780392230f, 0.082352944f, 0.521568656f, 1.000000000f};
		const float MidnightBlue         [4] = {0.098039225f, 0.098039225f, 0.439215720f, 1.000000000f};
		const float MintCream            [4] = {0.960784376f, 1.000000000f, 0.980392218f, 1.000000000f};
		const float MistyRose            [4] = {1.000000000f, 0.894117713f, 0.882353008f, 1.000000000f};
		const float Moccasin             [4] = {1.000000000f, 0.894117713f, 0.709803939f, 1.000000000f};
		const float NavajoWhite          [4] = {1.000000000f, 0.870588303f, 0.678431392f, 1.000000000f};
		const float Navy                 [4] = {0.000000000f, 0.000000000f, 0.501960814f, 1.000000000f};
		const float OldLace              [4] = {0.992156923f, 0.960784376f, 0.901960850f, 1.000000000f};
		const float Olive                [4] = {0.501960814f, 0.501960814f, 0.000000000f, 1.000000000f};
		const float OliveDrab            [4] = {0.419607878f, 0.556862772f, 0.137254909f, 1.000000000f};
		const float Orange               [4] = {1.000000000f, 0.647058845f, 0.000000000f, 1.000000000f};
		const float OrangeRed            [4] = {1.000000000f, 0.270588249f, 0.000000000f, 1.000000000f};
		const float Orchid               [4] = {0.854902029f, 0.439215720f, 0.839215755f, 1.000000000f};
		const float PaleGoldenrod        [4] = {0.933333397f, 0.909803987f, 0.666666687f, 1.000000000f};
		const float PaleGreen            [4] = {0.596078455f, 0.984313786f, 0.596078455f, 1.000000000f};
		const float PaleTurquoise        [4] = {0.686274529f, 0.933333397f, 0.933333397f, 1.000000000f};
		const float PaleVioletRed        [4] = {0.858823597f, 0.439215720f, 0.576470613f, 1.000000000f};
		const float PapayaWhip           [4] = {1.000000000f, 0.937254965f, 0.835294187f, 1.000000000f};
		const float PeachPuff            [4] = {1.000000000f, 0.854902029f, 0.725490212f, 1.000000000f};
		const float Peru                 [4] = {0.803921640f, 0.521568656f, 0.247058839f, 1.000000000f};
		const float Pink                 [4] = {1.000000000f, 0.752941251f, 0.796078503f, 1.000000000f};
		const float Plum                 [4] = {0.866666734f, 0.627451003f, 0.866666734f, 1.000000000f};
		const float PowderBlue           [4] = {0.690196097f, 0.878431439f, 0.901960850f, 1.000000000f};
		const float Purple               [4] = {0.501960814f, 0.000000000f, 0.501960814f, 1.000000000f};
		const float Red                  [4] = {1.000000000f, 0.000000000f, 0.000000000f, 1.000000000f};
		const float RosyBrown            [4] = {0.737254918f, 0.560784340f, 0.560784340f, 1.000000000f};
		const float RoyalBlue            [4] = {0.254901975f, 0.411764741f, 0.882353008f, 1.000000000f};
		const float SaddleBrown          [4] = {0.545098066f, 0.270588249f, 0.074509807f, 1.000000000f};
		const float Salmon               [4] = {0.980392218f, 0.501960814f, 0.447058856f, 1.000000000f};
		const float SandyBrown           [4] = {0.956862807f, 0.643137276f, 0.376470625f, 1.000000000f};
		const float SeaGreen             [4] = {0.180392161f, 0.545098066f, 0.341176480f, 1.000000000f};
		const float SeaShell             [4] = {1.000000000f, 0.960784376f, 0.933333397f, 1.000000000f};
		const float Sienna               [4] = {0.627451003f, 0.321568638f, 0.176470593f, 1.000000000f};
		const float Silver               [4] = {0.752941251f, 0.752941251f, 0.752941251f, 1.000000000f};
		const float SkyBlue              [4] = {0.529411793f, 0.807843208f, 0.921568692f, 1.000000000f};
		const float SlateBlue            [4] = {0.415686309f, 0.352941185f, 0.803921640f, 1.000000000f};
		const float SlateGray            [4] = {0.439215720f, 0.501960814f, 0.564705908f, 1.000000000f};
		const float Snow                 [4] = {1.000000000f, 0.980392218f, 0.980392218f, 1.000000000f};
		const float SpringGreen          [4] = {0.000000000f, 1.000000000f, 0.498039246f, 1.000000000f};
		const float SteelBlue            [4] = {0.274509817f, 0.509803951f, 0.705882370f, 1.000000000f};
		const float Tan                  [4] = {0.823529482f, 0.705882370f, 0.549019635f, 1.000000000f};
		const float Teal                 [4] = {0.000000000f, 0.501960814f, 0.501960814f, 1.000000000f};
		const float Thistle              [4] = {0.847058892f, 0.749019623f, 0.847058892f, 1.000000000f};
		const float Tomato               [4] = {1.000000000f, 0.388235331f, 0.278431386f, 1.000000000f};
		const float Turquoise            [4] = {0.250980407f, 0.878431439f, 0.815686345f, 1.000000000f};
		const float Violet               [4] = {0.933333397f, 0.509803951f, 0.933333397f, 1.000000000f};
		const float Wheat                [4] = {0.960784376f, 0.870588303f, 0.701960802f, 1.000000000f};
		const float White                [4] = {1.000000000f, 1.000000000f, 1.000000000f, 1.000000000f};
		const float WhiteSmoke           [4] = {0.960784376f, 0.960784376f, 0.960784376f, 1.000000000f};
		const float Yellow               [4] = {1.000000000f, 1.000000000f, 0.000000000f, 1.000000000f};
		const float YellowGreen          [4] = {0.603921592f, 0.803921640f, 0.196078449f, 1.000000000f};

		//-------- Transparent
		const float Transparent   [4] = {0.00f, 0.00f, 0.00f, 0.00f};
		const float TransDarkGray [4] = {0.12f, 0.12f, 0.12f, 0.78f};
	}
}