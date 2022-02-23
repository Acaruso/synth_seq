#include "draw_system.hpp"

#include <algorithm>
#include <functional>
#include <iostream>

DrawSystem::DrawSystem() {}

DrawSystem::DrawSystem(SDL_Renderer* renderer) : renderer(renderer) {}

void DrawSystem::push(DrawCommand command)
{
    drawQueue.push_back(command);
}

auto v_getZAxis = [](auto& x) { return x.getZAxis(); };

// larger z_axis means it's on top
void DrawSystem::draw()
{
    std::sort(
        std::begin(drawQueue),
        std::end(drawQueue),
        [](auto& a, auto& b) {
            return std::visit(v_getZAxis, a) > std::visit(v_getZAxis, b);
        }
    );

    auto v_draw = [&](auto& x) { x.draw(renderer); };

    while (!drawQueue.empty()) {
        std::visit(v_draw, drawQueue.back());
        drawQueue.pop_back();
    }
}
