/*
MD_Cubo - Arduino LED Cube Library

See main header file for comments
*/

#pragma once

/**
 * \file
 * \brief Includes color related definitions
 */

/**
 \def RGB
 Macro to create a RGB uint32_t integer from the R, G and B components.
 */
#define RGB(r,g,b) ((((uint32_t)r<<16) + ((uint32_t)g<<8) + b) & 0x00ffffff)

/**
 \def R
 Extract the R component from a uint32 constructed with the RGB() macro
 \def G
 Extract the G component from a uint32 constructed with the RGB() macro
 \def B
 Extract the B component from a uint32 constructed with the RGB() macro
 */
#define R(c) ((uint8_t)((c >> 16) & 0xff))
#define G(c) ((uint8_t)((c >>  8) & 0xff))
#define B(c) ((uint8_t)(c & 0xff))

// Color definitions for standard colors
// Thanks to the FastLED library for the names and RGB values ;-)
const uint32_t VOX_ALICEBLUE            = RGB(0xf0, 0xf8, 0xff);    ///< Color AliceBlue
const uint32_t VOX_AMETHYST             = RGB(0x99, 0x66, 0xcc);    ///< Color Amethyst
const uint32_t VOX_ANTIQUEWHITE         = RGB(0xfa, 0xeb, 0xd7);    ///< Color AntiqueWhite
const uint32_t VOX_AQUA                 = RGB(0x00, 0xff, 0xff);    ///< Color Aqua
const uint32_t VOX_AQUAMARINE           = RGB(0x7f, 0xff, 0xd4);    ///< Color Aquamarine
const uint32_t VOX_AZURE                = RGB(0xf0, 0xff, 0xff);    ///< Color Azure
const uint32_t VOX_BEIGE                = RGB(0xf5, 0xf5, 0xdc);    ///< Color Beige
const uint32_t VOX_BISQUE               = RGB(0xff, 0xe4, 0xc4);    ///< Color Bisque
const uint32_t VOX_BLACK                = RGB(0x00, 0x00, 0x00);    ///< Color Black
const uint32_t VOX_BLANCHEDALMOND       = RGB(0xff, 0xeb, 0xcd);    ///< Color BlanchedAlmond
const uint32_t VOX_BLUE                 = RGB(0x00, 0x00, 0xff);    ///< Color Blue
const uint32_t VOX_BLUEVIOLET           = RGB(0x8a, 0x2b, 0xe2);    ///< Color BlueViolet
const uint32_t VOX_BROWN                = RGB(0xa5, 0x2a, 0x2a);    ///< Color Brown
const uint32_t VOX_BURLYWOOD            = RGB(0xde, 0xb8, 0x87);    ///< Color BurlyWood
const uint32_t VOX_CADETBLUE            = RGB(0x5f, 0x9e, 0xa0);    ///< Color CadetBlue
const uint32_t VOX_CHARTREUSE           = RGB(0x7f, 0xff, 0x00);    ///< Color Chartreuse
const uint32_t VOX_CHOCOLATE            = RGB(0xd2, 0x69, 0x1e);    ///< Color Chocolate
const uint32_t VOX_CORAL                = RGB(0xff, 0x7f, 0x50);    ///< Color Coral
const uint32_t VOX_CORNFLOWERBLUE       = RGB(0x64, 0x95, 0xed);    ///< Color CornflowerBlue
const uint32_t VOX_CORNSILK             = RGB(0xff, 0xf8, 0xdc);    ///< Color Cornsilk
const uint32_t VOX_CRIMSON              = RGB(0xdc, 0x14, 0x3c);    ///< Color Crimson
const uint32_t VOX_CYAN                 = RGB(0x00, 0xff, 0xff);    ///< Color Cyan
const uint32_t VOX_DARKBLUE             = RGB(0x00, 0x00, 0x8b);    ///< Color DarkBlue
const uint32_t VOX_DARKCYAN             = RGB(0x00, 0x8b, 0x8b);    ///< Color DarkCyan
const uint32_t VOX_DARKGOLDENROD        = RGB(0xb8, 0x86, 0x0b);    ///< Color DarkGoldenrod
const uint32_t VOX_DARKGRAY             = RGB(0xa9, 0xa9, 0xa9);    ///< Color DarkGray
const uint32_t VOX_DARKGREY             = RGB(0xa9, 0xa9, 0xa9);    ///< Color DarkGrey
const uint32_t VOX_DARKGREEN            = RGB(0x00, 0x64, 0x00);    ///< Color DarkGreen
const uint32_t VOX_DARKKHAKI            = RGB(0xbd, 0xb7, 0x6b);    ///< Color DarkKhaki
const uint32_t VOX_DARKMAGENTA          = RGB(0x8b, 0x00, 0x8b);    ///< Color DarkMagenta
const uint32_t VOX_DARKOLIVEGREEN       = RGB(0x55, 0x6b, 0x2f);    ///< Color DarkOliveGreen
const uint32_t VOX_DARKORANGE           = RGB(0xff, 0x8c, 0x00);    ///< Color DarkOrange
const uint32_t VOX_DARKORCHID           = RGB(0x99, 0x32, 0xcc);    ///< Color DarkOrchid
const uint32_t VOX_DARKRED              = RGB(0x8b, 0x00, 0x00);    ///< Color DarkRed
const uint32_t VOX_DARKSALMON           = RGB(0xe9, 0x96, 0x7a);    ///< Color DarkSalmon
const uint32_t VOX_DARKSEAGREEN         = RGB(0x8f, 0xbc, 0x8f);    ///< Color DarkSeaGreen
const uint32_t VOX_DARKSLATEBLUE        = RGB(0x48, 0x3d, 0x8b);    ///< Color DarkSlateBlue
const uint32_t VOX_DARKSLATEGRAY        = RGB(0x2f, 0x4f, 0x4f);    ///< Color DarkSlateGray
const uint32_t VOX_DARKSLATEGREY        = RGB(0x2f, 0x4f, 0x4f);    ///< Color DarkSlateGrey
const uint32_t VOX_DARKTURQUOISE        = RGB(0x00, 0xce, 0xd1);    ///< Color DarkTurquoise
const uint32_t VOX_DARKVIOLET           = RGB(0x94, 0x00, 0xd3);    ///< Color DarkViolet
const uint32_t VOX_DEEPPINK             = RGB(0xff, 0x14, 0x93);    ///< Color DeepPink
const uint32_t VOX_DEEPSKYBLUE          = RGB(0x00, 0xbf, 0xff);    ///< Color DeepSkyBlue
const uint32_t VOX_DIMGRAY              = RGB(0x69, 0x69, 0x69);    ///< Color DimGray
const uint32_t VOX_DIMGREY              = RGB(0x69, 0x69, 0x69);    ///< Color DimGrey
const uint32_t VOX_DODGERBLUE           = RGB(0x1e, 0x90, 0xff);    ///< Color DodgerBlue
const uint32_t VOX_FIREBRICK            = RGB(0xb2, 0x22, 0x22);    ///< Color FireBrick
const uint32_t VOX_FLORALWHITE          = RGB(0xff, 0xfa, 0xf0);    ///< Color FloralWhite
const uint32_t VOX_FORESTGREEN          = RGB(0x22, 0x8b, 0x22);    ///< Color ForestGreen
const uint32_t VOX_FUCHSIA              = RGB(0xff, 0x00, 0xff);    ///< Color Fuchsia
const uint32_t VOX_GAINSBORO            = RGB(0xdc, 0xdc, 0xdc);    ///< Color Gainsboro
const uint32_t VOX_GHOSTWHITE           = RGB(0xf8, 0xf8, 0xff);    ///< Color GhostWhite
const uint32_t VOX_GOLD                 = RGB(0xff, 0xd7, 0x00);    ///< Color Gold
const uint32_t VOX_GOLDENROD            = RGB(0xda, 0xa5, 0x20);    ///< Color Goldenrod
const uint32_t VOX_GRAY                 = RGB(0x80, 0x80, 0x80);    ///< Color Gray
const uint32_t VOX_GREY                 = RGB(0x80, 0x80, 0x80);    ///< Color Grey
const uint32_t VOX_GREEN                = RGB(0x00, 0x80, 0x00);    ///< Color Green
const uint32_t VOX_GREENYELLOW          = RGB(0xad, 0xff, 0x2f);    ///< Color GreenYellow
const uint32_t VOX_HONEYDEW             = RGB(0xf0, 0xff, 0xf0);    ///< Color Honeydew
const uint32_t VOX_HOTPINK              = RGB(0xff, 0x69, 0xb4);    ///< Color HotPink
const uint32_t VOX_INDIANRED            = RGB(0xcd, 0x5c, 0x5c);    ///< Color IndianRed
const uint32_t VOX_INDIGO               = RGB(0x4b, 0x00, 0x82);    ///< Color Indigo
const uint32_t VOX_IVORY                = RGB(0xff, 0xff, 0xf0);    ///< Color Ivory
const uint32_t VOX_KHAKI                = RGB(0xf0, 0xe6, 0x8c);    ///< Color Khaki
const uint32_t VOX_LAVENDER             = RGB(0xe6, 0xe6, 0xfa);    ///< Color Lavender
const uint32_t VOX_LAVENDERBLUSH        = RGB(0xff, 0xf0, 0xf5);    ///< Color LavenderBlush
const uint32_t VOX_LAWNGREEN            = RGB(0x7c, 0xfc, 0x00);    ///< Color LawnGreen
const uint32_t VOX_LEMONCHIFFON         = RGB(0xff, 0xfa, 0xcd);    ///< Color LemonChiffon
const uint32_t VOX_LIGHTBLUE            = RGB(0xad, 0xd8, 0xe6);    ///< Color LightBlue
const uint32_t VOX_LIGHTCORAL           = RGB(0xf0, 0x80, 0x80);    ///< Color LightCoral
const uint32_t VOX_LIGHTCYAN            = RGB(0xe0, 0xff, 0xff);    ///< Color LightCyan
const uint32_t VOX_LIGHTGOLDENRODYELLOW = RGB(0xfa, 0xfa, 0xd2);    ///< Color LightGoldenrodYellow
const uint32_t VOX_LIGHTGREEN           = RGB(0x90, 0xee, 0x90);    ///< Color LightGreen
const uint32_t VOX_LIGHTGRAY            = RGB(0xd3, 0xd3, 0xd3);    ///< Color LightGray
const uint32_t VOX_LIGHTGREY            = RGB(0xd3, 0xd3, 0xd3);    ///< Color LightGrey
const uint32_t VOX_LIGHTPINK            = RGB(0xff, 0xb6, 0xc1);    ///< Color LightPink
const uint32_t VOX_LIGHTSALMON          = RGB(0xff, 0xa0, 0x7a);    ///< Color LightSalmon
const uint32_t VOX_LIGHTSEAGREEN        = RGB(0x20, 0xb2, 0xaa);    ///< Color LightSeaGreen
const uint32_t VOX_LIGHTSKYBLUE         = RGB(0x87, 0xce, 0xfa);    ///< Color LightSkyBlue
const uint32_t VOX_LIGHTSLATEGRAY       = RGB(0x77, 0x88, 0x99);    ///< Color LightSlateGray
const uint32_t VOX_LIGHTSLATEGREY       = RGB(0x77, 0x88, 0x99);    ///< Color LightSlateGrey
const uint32_t VOX_LIGHTSTEELBLUE       = RGB(0xb0, 0xc4, 0xde);    ///< Color LightSteelBlue
const uint32_t VOX_LIGHTYELLOW          = RGB(0xff, 0xff, 0xe0);    ///< Color LightYellow
const uint32_t VOX_LIME                 = RGB(0x00, 0xff, 0x00);    ///< Color Lime
const uint32_t VOX_LIMEGREEN            = RGB(0x32, 0xcd, 0x32);    ///< Color LimeGreen
const uint32_t VOX_LINEN                = RGB(0xfa, 0xf0, 0xe6);    ///< Color Linen
const uint32_t VOX_MAGENTA              = RGB(0xff, 0x00, 0xff);    ///< Color Magenta
const uint32_t VOX_MAROON               = RGB(0x80, 0x00, 0x00);    ///< Color Maroon
const uint32_t VOX_MEDIUMAQUAMARINE     = RGB(0x66, 0xcd, 0xaa);    ///< Color MediumAquamarine
const uint32_t VOX_MEDIUMBLUE           = RGB(0x00, 0x00, 0xcd);    ///< Color MediumBlue
const uint32_t VOX_MEDIUMORCHID         = RGB(0xba, 0x55, 0xd3);    ///< Color MediumOrchid
const uint32_t VOX_MEDIUMPURPLE         = RGB(0x93, 0x70, 0xdb);    ///< Color MediumPurple
const uint32_t VOX_MEDIUMSEAGREEN       = RGB(0x3c, 0xb3, 0x71);    ///< Color MediumSeaGreen
const uint32_t VOX_MEDIUMSLATEBLUE      = RGB(0x7b, 0x68, 0xee);    ///< Color MediumSlateBlue
const uint32_t VOX_MEDIUMSPRINGGREEN    = RGB(0x00, 0xfa, 0x9a);    ///< Color MediumSpringGreen
const uint32_t VOX_MEDIUMTURQUOISE      = RGB(0x48, 0xd1, 0xcc);    ///< Color MediumTurquoise
const uint32_t VOX_MEDIUMVIOLETRED      = RGB(0xc7, 0x15, 0x85);    ///< Color MediumVioletRed
const uint32_t VOX_MIDNIGHTBLUE         = RGB(0x19, 0x19, 0x70);    ///< Color MidnightBlue
const uint32_t VOX_MINTCREAM            = RGB(0xf5, 0xff, 0xfa);    ///< Color MintCream
const uint32_t VOX_MISTYROSE            = RGB(0xff, 0xe4, 0xe1);    ///< Color MistyRose
const uint32_t VOX_MOCCASIN             = RGB(0xff, 0xe4, 0xb5);    ///< Color Moccasin
const uint32_t VOX_NAVAJOWHITE          = RGB(0xff, 0xde, 0xad);    ///< Color NavajoWhite
const uint32_t VOX_NAVY                 = RGB(0x00, 0x00, 0x80);    ///< Color Navy
const uint32_t VOX_OLDLACE              = RGB(0xfd, 0xf5, 0xe6);    ///< Color OldLace
const uint32_t VOX_OLIVE                = RGB(0x80, 0x80, 0x00);    ///< Color Olive
const uint32_t VOX_OLIVEDRAB            = RGB(0x6b, 0x8e, 0x23);    ///< Color OliveDrab
const uint32_t VOX_ORANGE               = RGB(0xff, 0xa5, 0x00);    ///< Color Orange
const uint32_t VOX_ORANGERED            = RGB(0xff, 0x45, 0x00);    ///< Color OrangeRed
const uint32_t VOX_ORCHID               = RGB(0xda, 0x70, 0xd6);    ///< Color Orchid
const uint32_t VOX_PALEGOLDENROD        = RGB(0xee, 0xe8, 0xaa);    ///< Color PaleGoldenrod
const uint32_t VOX_PALEGREEN            = RGB(0x98, 0xfb, 0x98);    ///< Color PaleGreen
const uint32_t VOX_PALETURQUOISE        = RGB(0xaf, 0xee, 0xee);    ///< Color PaleTurquoise
const uint32_t VOX_PALEVIOLETRED        = RGB(0xdb, 0x70, 0x93);    ///< Color PaleVioletRed
const uint32_t VOX_PAPAYAWHIP           = RGB(0xff, 0xef, 0xd5);    ///< Color PapayaWhip
const uint32_t VOX_PEACHPUFF            = RGB(0xff, 0xda, 0xb9);    ///< Color PeachPuff
const uint32_t VOX_PERU                 = RGB(0xcd, 0x85, 0x3f);    ///< Color Peru
const uint32_t VOX_PINK                 = RGB(0xff, 0xc0, 0xcb);    ///< Color Pink
const uint32_t VOX_PLAID                = RGB(0xcc, 0x55, 0x33);    ///< Color Plaid
const uint32_t VOX_PLUM                 = RGB(0xdd, 0xa0, 0xdd);    ///< Color Plum
const uint32_t VOX_POWDERBLUE           = RGB(0xb0, 0xe0, 0xe6);    ///< Color PowderBlue
const uint32_t VOX_PURPLE               = RGB(0x80, 0x00, 0x80);    ///< Color Purple
const uint32_t VOX_RED                  = RGB(0xff, 0x00, 0x00);    ///< Color Red
const uint32_t VOX_ROSYBROWN            = RGB(0xbc, 0x8f, 0x8f);    ///< Color RosyBrown
const uint32_t VOX_ROYALBLUE            = RGB(0x41, 0x69, 0xe1);    ///< Color RoyalBlue
const uint32_t VOX_SADDLEBROWN          = RGB(0x8b, 0x45, 0x13);    ///< Color SaddleBrown
const uint32_t VOX_SALMON               = RGB(0xfa, 0x80, 0x72);    ///< Color Salmon
const uint32_t VOX_SANDYBROWN           = RGB(0xf4, 0xa4, 0x60);    ///< Color SandyBrown
const uint32_t VOX_SEAGREEN             = RGB(0x2e, 0x8b, 0x57);    ///< Color SeaGreen
const uint32_t VOX_SEASHELL             = RGB(0xff, 0xf5, 0xee);    ///< Color Seashell
const uint32_t VOX_SIENNA               = RGB(0xa0, 0x52, 0x2d);    ///< Color Sienna
const uint32_t VOX_SILVER               = RGB(0xc0, 0xc0, 0xc0);    ///< Color Silver
const uint32_t VOX_SKYBLUE              = RGB(0x87, 0xce, 0xeb);    ///< Color SkyBlue
const uint32_t VOX_SLATEBLUE            = RGB(0x6a, 0x5a, 0xcd);    ///< Color SlateBlue
const uint32_t VOX_SLATEGRAY            = RGB(0x70, 0x80, 0x90);    ///< Color SlateGray
const uint32_t VOX_SLATEGREY            = RGB(0x70, 0x80, 0x90);    ///< Color SlateGrey
const uint32_t VOX_SNOW                 = RGB(0xff, 0xfa, 0xfa);    ///< Color Snow
const uint32_t VOX_SPRINGGREEN          = RGB(0x00, 0xff, 0x7f);    ///< Color SpringGreen
const uint32_t VOX_STEELBLUE            = RGB(0x46, 0x82, 0xb4);    ///< Color SteelBlue
const uint32_t VOX_TAN                  = RGB(0xd2, 0xb4, 0x8c);    ///< Color Tan
const uint32_t VOX_TEAL                 = RGB(0x00, 0x80, 0x80);    ///< Color Teal
const uint32_t VOX_THISTLE              = RGB(0xd8, 0xbf, 0xd8);    ///< Color Thistle
const uint32_t VOX_TOMATO               = RGB(0xff, 0x63, 0x47);    ///< Color Tomato
const uint32_t VOX_TURQUOISE            = RGB(0x40, 0xe0, 0xd0);    ///< Color Turquoise
const uint32_t VOX_VIOLET               = RGB(0xee, 0x82, 0xee);    ///< Color Violet
const uint32_t VOX_WHEAT                = RGB(0xf5, 0xde, 0xb3);    ///< Color Wheat
const uint32_t VOX_WHITE                = RGB(0xff, 0xff, 0xff);    ///< Color White
const uint32_t VOX_WHITESMOKE           = RGB(0xf5, 0xf5, 0xf5);    ///< Color WhiteSmoke
const uint32_t VOX_YELLOW               = RGB(0xff, 0xff, 0x00);    ///< Color Yellow
const uint32_t VOX_YELLOWGREEN          = RGB(0x9a, 0xcd, 0x32);    ///< Color YellowGreen

// Special colors for monochrome cubes
const uint32_t VOX_OFF = VOX_BLACK;  ///< Default RGB OFF color also used as monochrome LED off indicator
const uint32_t VOX_ON  = VOX_WHITE;  ///< Default RGB ON color also used as monochrome LED on indicator

