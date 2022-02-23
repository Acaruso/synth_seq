#pragma once

#include <string>
#include <variant>

#include <SDL.h>

#include "src/main/graphics/font.hpp"
#include "src/main/graphics/image.hpp"
#include "src/main/graphics/coord.hpp"
#include "src/main/graphics/rect.hpp"

struct DrawRectCommand
{
    Rect rect;
    DrawRectCommand(Rect rect);
    void draw(SDL_Renderer* renderer);
    int getZAxis();
};

struct DrawImageCommand
{
    Image image;
    Coord coord;
    DrawImageCommand(Image image, Coord coord);
    void draw(SDL_Renderer* renderer);
    int getZAxis();
};

struct DrawTextCommand
{
    std::string text;
    Font font;
    Coord coord;
    DrawTextCommand(std::string text, Font font, Coord coord);
    void draw(SDL_Renderer* renderer);
    int getZAxis();
};

struct DrawLineCommand
{
    Coord start;
    Coord end;
    Color color{black};
    DrawLineCommand(Coord start, Coord end);
    void draw(SDL_Renderer* renderer);
    int getZAxis();
};

using DrawCommand = std::variant<
    DrawRectCommand,
    DrawImageCommand,
    DrawTextCommand,
    DrawLineCommand
>;
