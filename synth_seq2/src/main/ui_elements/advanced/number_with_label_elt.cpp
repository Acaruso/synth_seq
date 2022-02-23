#include "number_with_label_elt.hpp"

#include <string>

#include "src/main/ui_elements/advanced/number_elt.hpp"
#include "src/main/ui_elements/basic/rect_outline_elt.hpp"

void numberWithLabelElt(EltParams& params)
{
    auto& context = params.ctx;
    std::string label = params.label;
    Coord coord = params.coord;

    // label //////////////////////////
    std::string fontName = "inconsolata";
    Font& font = context.graphicsWrapper.getFont(fontName);
    Coord labelCoord = coord;
    labelCoord.x += 4;
    context.graphicsWrapper.drawText(label, fontName, labelCoord);

    // number rect ////////////////////
    EltParams p = params;

    p.coord = Coord{
        coord.x + 100,
        coord.y
    };

    int maxNumDigits = (int)std::to_string(p.max).size();

    p.fontName = fontName;

    p.rect = Rect{
        p.coord.x,
        p.coord.y,
        maxNumDigits * font.width,
        font.height
    };

    numberElt(p);

    // outline rect ///////////////////
    Rect outerRect{
        coord.x,
        coord.y,
        100,
        18
    };

    EltParams p2(context);
    p2.rect = outerRect;
    rectOutlineElt(p2);

    Rect outerBgRect = outerRect;
    outerBgRect.color = white;
    outerBgRect.z = -2;
    params.ctx.graphicsWrapper.drawRect(outerBgRect);
}
