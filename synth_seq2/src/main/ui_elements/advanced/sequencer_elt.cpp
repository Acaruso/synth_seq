#include "sequencer_elt.hpp"

#include "src/main/sequencer/sequencer.hpp"
#include "src/main/ui_elements/advanced/rect_button_elt.hpp"

void _clock(AppContext& ctx, Coord coord, int i);
Rect _getClockRect(Coord coord, int i);
void _cell(AppContext& ctx, Cell& cell, Coord coord, int i);
Rect _getCellRect(Coord coord, int i);
void _drawSelectedRect(AppContext& ctx, Rect rect);

namespace
{
    int cellWidth = 50;
    int cellHeight = 50;
    int clockCellWidth = 50;
    int clockCellHeight = 20;
    int padding = 10;
}

void sequencerElt(EltParams& params)
{
    Coord coord = params.coord;

    Rect bgRect{
        coord.x,
        coord.y,
        -3,
        ((cellWidth + padding) * (int)params.ctx.sequencer->row.size()) + padding,
        cellHeight + clockCellHeight + (padding * 3),
        green
    };

    params.ctx.graphicsWrapper.drawRect(bgRect);

    Coord newCoord = coord;
    newCoord.x = coord.x + padding;
    newCoord.y = coord.y + padding;

    for (int i = 0; i < params.ctx.sequencer->row.size(); i++) {
        Cell& cell = params.ctx.sequencer->row[i];
        _clock(params.ctx, newCoord, i);
        _cell(params.ctx, cell, newCoord, i);
    }
}

void _clock(AppContext& ctx, Coord coord, int i)
{
    EltParams p(ctx);
    p.rect = _getClockRect(coord, i);
    p.color = white;

    if (ctx.sequencer->playing == false) {
        p.displayColor = white;
    }
    else {
        p.displayColor = ctx.sequencer->step == i ? blue : white;
    }

    rectButtonElt(p);
}

Rect _getClockRect(Coord coord, int i)
{
    return Rect(
        coord.x + ((clockCellWidth + padding) * i),
        coord.y,
        clockCellWidth,
        clockCellHeight
    );
}

void _cell(AppContext& ctx, Cell& cell, Coord coord, int i)
{
    auto& uiState = ctx.getUiState();

    EltParams p(ctx);
    p.rect = _getCellRect(coord, i);
    p.color = white;
    p.displayColor = cell.on ? blue : white;
    p.onClickColor = blue;

    p.onClick = [&]() {
        if (uiState.lshift) {
            ctx.sequencer->selectCell(i);
        }
        else {
            ctx.sequencer->toggleCell(i);
        }
    };

    p.onHold = [&]() {
        if (!uiState.lshift) {
            p.displayColor = p.onClickColor;
        }
    };

    if (ctx.sequencer->mode == Select && ctx.sequencer->selected == i) {
        _drawSelectedRect(ctx, p.rect);
    }

    rectButtonElt(p);
}

Rect _getCellRect(Coord coord, int i)
{
    int buttonSize = 50;
    int padding = 10;

    return Rect(
        coord.x + ((cellWidth + padding) * i),
        coord.y + clockCellHeight + padding,
        cellWidth,
        cellHeight
    );
}

void _drawSelectedRect(AppContext& ctx, Rect rect)
{
    int borderWidth = 4;

    Rect selectedRect(
        rect.x - borderWidth,
        rect.y - borderWidth,
        -2,
        rect.w + (2 * borderWidth),
        rect.h + (2 * borderWidth),
        red
    );

    Rect whiteRect(
        rect.x - 1,
        rect.y - 1,
        -1,
        rect.w + 2,
        rect.h + 2,
        white
    );

    Rect greenRect(
        rect.x - 1,
        rect.y - 1,
        -1,
        rect.w + 2,
        rect.h + 2,
        green
    );

    ctx.graphicsWrapper.drawRect(greenRect);
    ctx.graphicsWrapper.drawRect(selectedRect);
}
