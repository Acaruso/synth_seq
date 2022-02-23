#pragma once

#include <iostream>

#include "src/main/ui_elements/elt_params.hpp"

inline void rectOutlineElt(EltParams& params)
{
    auto& gfx = params.ctx.graphicsWrapper;
    Rect r = params.rect;

    Coord topLeft{r.x, r.y};
    Coord topRight{r.x + r.w, r.y};
    Coord bottomLeft{r.x, r.y + r.h};
    Coord bottomRight{r.x + r.w, r.y + r.h};

    for (int i = 0; i < params.lineWidth; i++) {
        gfx.drawLine(topLeft, topRight);
        gfx.drawLine(topRight, bottomRight);
        gfx.drawLine(bottomRight, bottomLeft);
        gfx.drawLine(bottomLeft, topLeft);
        topLeft.x += 1;
        topLeft.y += 1;
        topRight.x -= 1;
        topRight.y += 1;
        bottomLeft.x += 1;
        bottomLeft.y -= 1;
        bottomRight.x -= 1;
        bottomRight.y -= 1;
    }
}
