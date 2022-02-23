#include "piano_elt.hpp"

#include <functional>
#include <iostream>

#include "src/main/sequencer/sequencer.hpp"
#include "src/main/ui_elements/advanced/rect_button_elt.hpp"
#include "src/main/ui_elements/advanced/text_elt.hpp"
#include "src/main/util.hpp"

namespace
{
    int buttonSize = 50;
    int padding = 10;
}

Rect getWhiteKeyRect(Coord coord, int i);
Rect getBlackKeyRect(Coord coord, int i);

void whiteKey(
    AppContext& ctx,
    Rect rect,
    std::function<void()> onClick,
    int note
);

void blackKey(
    AppContext& ctx,
    Rect rect,
    std::function<void()> onClick,
    int note
);

void background(EltParams& params, Coord coord);

void octaveControl(EltParams& params, Coord coord);

void pianoElt(EltParams& params)
{
    AppContext& ctx = params.ctx;
    Coord coord = params.coord;
    Sequencer* sequencer = ctx.sequencer;

    background(params, coord);

    octaveControl(params, Coord(coord.x + 450, coord.y));

    int k = 0;

    for (int i = 0; i < 12; i++) {
        int note = ((sequencer->octave + 1) * 12) + i;

        std::function<void()> onClick_ = nullptr;

        if (sequencer->getMode() == Normal) {
            onClick_ = [&]() {
                auto& synthSettings = sequencer->getSelectedTrack().getSynthSettings();
                synthSettings["note"] = note;
                ctx.toAudioQueue->enqueue(
                    SynthSettingsMessage(synthSettings)
                );
            };
        }
        else if (sequencer->getMode() == Select) {
            onClick_ = [&]() {
                auto& synthSettings = sequencer->getSelectedCell().getSynthSettings();
                synthSettings["note"] = note;
            };
        }

        if (i == 1 || i == 3 || i == 6 || i == 8 || i == 10) {
            blackKey(
                params.ctx,
                getBlackKeyRect(coord, i),
                onClick_,
                note
            );
        }
        else {
            whiteKey(
                params.ctx,
                getWhiteKeyRect(coord, k++),
                onClick_,
                note
            );
        }
    }
}

void whiteKey(
    AppContext& ctx,
    Rect rect,
    std::function<void()> onClick,
    int note
) {
    EltParams p(ctx);
    p.rect = rect;
    p.color = inactiveColor;

    if (
        ctx.sequencer->mode == Select
        && ctx.sequencer->getSelectedCell().on
        && ctx.sequencer->getSelectedCell().synthSettings["note"] == note
    ) {
        p.displayColor = activeColor;
    }
    else {
        p.displayColor = inactiveColor;
    }

    p.onClickColor = activeColor;
    p.onClick = onClick;
    p.onHold = [&]() { p.displayColor = p.onClickColor; };
    rectButtonElt(p);
}

void blackKey(
    AppContext& ctx,
    Rect rect,
    std::function<void()> onClick,
    int note
) {
    EltParams p(ctx);
    p.rect = rect;
    p.color = black;

    if (
        ctx.sequencer->mode == Select
        && ctx.sequencer->getSelectedCell().on
        && ctx.sequencer->getSelectedCell().synthSettings["note"] == note
    ) {
        p.displayColor = activeColor;
    }
    else {
        p.displayColor = black;
    }

    p.onClickColor = activeColor;
    p.onClick = onClick;
    p.onHold = [&]() { p.displayColor = p.onClickColor; };
    rectButtonElt(p);
}

Rect getWhiteKeyRect(Coord coord, int i)
{
    Rect newRect = {};
    newRect.x = coord.x + padding + (i * (buttonSize + padding));
    newRect.y = coord.y + buttonSize + (padding * 2);
    newRect.w = buttonSize;
    newRect.h = buttonSize;
    return newRect;
}

Rect getBlackKeyRect(Coord coord, int i)
{
    Rect newRect = {};
    newRect.y = coord.y + padding;
    newRect.w = buttonSize;
    newRect.h = buttonSize;

    int baseX = coord.x + padding + (buttonSize / 2) + (padding / 2);

    if (i == 1) {
        newRect.x = baseX;
    }
    else if (i == 3) {
        newRect.x = baseX + (buttonSize + padding);
    }
    else if (i == 6) {
        newRect.x = baseX + 3 * (buttonSize + padding);
    }
    else if (i == 8) {
        newRect.x = baseX + 4 * (buttonSize + padding);
    }
    else if (i == 10) {
        newRect.x = baseX + 5 * (buttonSize + padding);
    }

    return newRect;
}

void background(EltParams& params, Coord coord)
{
    Rect rect{
        coord.x,
        coord.y,
        -10,
        (buttonSize + padding) * 7 + padding,
        (buttonSize + padding) * 2 + padding,
        red
    };

    params.ctx.graphicsWrapper.drawRect(rect);
}

void octaveControl(EltParams& params, Coord coord)
{
    auto& sequencer = params.ctx.sequencer;

    int yPadding = 25;
    {
        EltParams textParams(params.ctx);
        textParams.coord = coord;
        textParams.label = "current octave: " + std::to_string(sequencer->octave);
        textElt(textParams);
    }
    coord.y += yPadding;
    {
        EltParams textParams(params.ctx);
        textParams.coord = coord;
        textParams.label = "octave up";
        textElt(textParams);

        EltParams p(params.ctx);
        p.rect = Rect(coord.x + 100, coord.y, 20, 20);
        p.displayColor = inactiveColor;
        p.onClick = [&]() {
            sequencer->octave = clamp(sequencer->octave + 1, 0, sequencer->numOctaves);
        };
        p.onHold = [&]() { p.displayColor = activeColor; };
        rectButtonElt(p);
    }
    coord.y += yPadding;
    {
        EltParams textParams(params.ctx);
        textParams.coord = Coord(coord.x, coord.y);
        textParams.label = "octave down";
        textElt(textParams);

        EltParams p(params.ctx);
        p.rect = Rect(coord.x + 100, coord.y, 20, 20);
        p.displayColor = inactiveColor;
        p.onClick = [&]() {
            sequencer->octave = clamp(sequencer->octave - 1, 0, sequencer->numOctaves);
        };
        p.onHold = [&]() { p.displayColor = activeColor; };
        rectButtonElt(p);
    }
}
