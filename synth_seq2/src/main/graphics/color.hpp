#pragma once

#include <string>
#include <sstream>

#include <SDL.h>

struct Color
{
    Uint8 r;
    Uint8 g;
    Uint8 b;
    Uint8 a;
    Color() {}
    Color(Uint8 r, Uint8 g, Uint8 b, Uint8 a) : r(r), g(g), b(b), a(a) {}
    // Color(std::string hex)
    // {
    // }
};

static Color black      = Color(0x47, 0x47, 0x49, 0xFF);
static Color white      = Color(0xFF, 0xFB, 0xEF, 0xFF);
static Color offWhite   = Color(0xF2, 0xEE, 0xE3, 0xFF);
static Color lightBlue  = Color(0x95, 0xD8, 0xE9, 0xFF);
static Color blue       = Color(0x74, 0xA8, 0xB5, 0xFF);
static Color orange     = Color(0xF9, 0xB2, 0x58, 0xFF);
static Color red        = Color(0xF1, 0x82, 0x5B, 0xFF);
static Color green      = Color(0x1F, 0xBA, 0xA8, 0xFF);
static Color lightGreen = Color(0x2A, 0xFF, 0xE6, 0xFF);
static Color darkGreen  = Color(0x12, 0x6D, 0x62, 0xFF);
static Color grey       = Color(0xBF, 0xBB, 0xB3, 0xFF);

static Color activeColor = orange;
static Color inactiveColor = white;
