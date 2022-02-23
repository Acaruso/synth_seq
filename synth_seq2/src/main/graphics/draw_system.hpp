#pragma once

#include <variant>
#include <vector>

#include "src/main/graphics/color.hpp"
#include "src/main/graphics/draw_commands.hpp"
#include "src/main/graphics/rect.hpp"

class DrawSystem
{
public:
    DrawSystem();
    DrawSystem(SDL_Renderer* renderer);
    void push(DrawCommand command);
    void draw();

private:
    SDL_Renderer* renderer;
    std::vector<DrawCommand> drawQueue;
};
