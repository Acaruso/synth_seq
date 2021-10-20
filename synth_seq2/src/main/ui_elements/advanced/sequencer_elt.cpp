#include "sequencer_elt.hpp"

#include "src/main/sequencer/sequencer.hpp"
#include "src/main/ui_elements/advanced/rect_button_elt.hpp"
#include "src/main/ui_elements/basic/rect_outline_elt.hpp"

void _clock(AppContext& ctx, Coord coord, int i);
Rect _getClockRect(Coord coord, int i);
void _cell(AppContext& ctx, Cell& cell, Coord coord, int row, int col);
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
    Sequencer* sequencer = params.ctx.sequencer;
    Coord coord = params.coord;

    // clock elts ////////////////////////////////////
    Coord newCoord = coord;
    newCoord.x = coord.x + padding;
    newCoord.y = coord.y + padding;

    for (int i = 0; i < sequencer->numSteps; i++) {
        _clock(params.ctx, newCoord, i);
    }

    // background ////////////////////////////////////
    Coord bgCoord = coord;
    Rect bgRect{
        bgCoord.x,
        bgCoord.y,
        -3,
        ((cellWidth + padding) * (int)sequencer->numSteps) + padding,
        clockCellHeight + (padding * 2),
        green
    };
    params.ctx.graphicsWrapper.drawRect(bgRect);

    bgCoord.y += clockCellHeight + (padding * 2);

    for (int i = 0; i < sequencer->tracks.size(); i++) {
        Rect bgRect2{
            bgCoord.x,
            bgCoord.y,
            -3,
            ((cellWidth + padding) * (int)sequencer->numSteps) + padding,
            cellHeight + (padding),
            green
        };
        params.ctx.graphicsWrapper.drawRect(bgRect2);
        bgCoord.y += cellHeight + padding;
    }

    for (int row = 0; row < sequencer->tracks.size(); row++) {
        auto& track = sequencer->tracks[row];
        for (int col = 0; col < track.cells.size(); col++) {
            Cell& cell = track.cells[col];
            _cell(params.ctx, cell, newCoord, row, col);
        }
        newCoord.y += cellHeight + padding;
    }
}

void _clock(AppContext& ctx, Coord coord, int i)
{
    EltParams p(ctx);
    p.rect = _getClockRect(coord, i);
    p.color = white;

    if (ctx.sequencer->isPlaying() == false) {
        p.displayColor = white;
    }
    else {
        p.displayColor = ctx.sequencer->curStep == i ? blue : white;
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

void _cell(AppContext& ctx, Cell& cell, Coord coord, int row, int col)
{
    auto& uiState = ctx.getUiState();

    EltParams p(ctx);
    p.rect = _getCellRect(coord, col);
    p.color = white;
    p.displayColor = cell.on ? blue : white;
    p.onClickColor = blue;

    p.onClick = [&]() {
        if (uiState.lshift) {
            ctx.sequencer->selectCell(row, col);
        }
        else {
            ctx.sequencer->toggleCell(row, col);
        }
    };

    p.onHold = [&]() {
        if (!uiState.lshift) {
            p.displayColor = p.onClickColor;
        }
    };

    if (
        ctx.sequencer->getMode() == Select
        && ctx.sequencer->getSelected().row == row
        && ctx.sequencer->getSelected().col == col
    ) {
        EltParams p2(ctx);
        int borderWidth = 4;
        p2.rect = Rect(
            p.rect.x - borderWidth,
            p.rect.y - borderWidth,
            2,
            p.rect.w + (2 * borderWidth) - 1,
            p.rect.h + (2 * borderWidth) - 1,
            black
        );

        rectOutlineElt(p2);
        // _drawSelectedRect(ctx, p.rect);
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
        black
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
