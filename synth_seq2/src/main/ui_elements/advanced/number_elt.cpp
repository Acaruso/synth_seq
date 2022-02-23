#include "number_elt.hpp"

#include <string>
#include <vector>

#include "src/main/graphics/color.hpp"
#include "src/main/ui_elements/decorators/base_decorator.hpp"
#include "src/main/ui_elements/decorators/decorator_utils.hpp"

Rect getInnerRect(EltParams& params)
{
    std::string fontName = params.fontName;
    Coord innerCoord;
    innerCoord.x = params.coord.x + 1;
    innerCoord.y = params.coord.y + 1;
    Font& font = params.ctx.graphicsWrapper.getFont(fontName);
    int maxNumDigits = (int)std::to_string(params.max).size();

    if (fontName == "inconsolata") {
        return Rect{
            innerCoord.x,
            innerCoord.y,
            -1,
            maxNumDigits * font.width + 4,
            font.height + 1,
            red
        };
    }
    else if (fontName == "dos") {
        return Rect{
            innerCoord.x,
            innerCoord.y,
            -1,
            maxNumDigits * font.width + 1,
            font.height - 1,
            red
        };
    }
}

Coord getTextCoord(EltParams& params)
{
    std::string fontName = params.fontName;
    Coord innerCoord;
    innerCoord.x = params.coord.x + 1;
    innerCoord.y = params.coord.y + 1;
    Coord textCoord = innerCoord;

    if (fontName == "inconsolata") {
        textCoord.x += 2;
    }
    else if (fontName == "dos") {
        textCoord.x += 1;
    }

    return textCoord;
}

void numberEltInner(EltParams& params)
{
    std::string fontName = params.fontName;
    Font& font = params.ctx.graphicsWrapper.getFont(params.fontName);

    std::string displayText = params.displayText;

    Rect innerRect = getInnerRect(params);

    params.ctx.graphicsWrapper.drawRect(innerRect);

    Coord textCoord = getTextCoord(params);

    params.ctx.graphicsWrapper.drawText(displayText, fontName, textCoord);

    Rect outerRect{
        params.coord.x,
        params.coord.y,
        -2,
        innerRect.w + 2,
        innerRect.h + 2,
        black
    };

    params.ctx.graphicsWrapper.drawRect(outerRect);
}

void numberElt(EltParams& params)
{
    std::vector<UnboundDecorator> decorators{baseDecorator};
    Elt res = decorate(decorators, params, numberEltInner);
    res(params);
}
