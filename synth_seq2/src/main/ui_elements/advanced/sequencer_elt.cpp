#include "sequencer_elt.hpp"

#include "src/main/sequencer/sequencer.hpp"
#include "src/main/ui_elements/advanced/rect_button_elt.hpp"
#include "src/main/ui_elements/basic/rect_outline_elt.hpp"

void _clock(AppContext& ctx, Coord coord, int i);
Rect _getClockRect(Coord coord, int i);
void _cell(AppContext& ctx, Cell& cell, Coord coord, int row, int col);
Rect _getCellRect(Coord coord, int i);
void _background(AppContext& ctx, Coord coord);

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

    Coord newCoord = coord;
    newCoord.x = coord.x + padding;
    newCoord.y = coord.y + padding;

    for (int i = 0; i < sequencer->numSteps; i++) {
        _clock(params.ctx, newCoord, i);
    }

    for (int row = 0; row < sequencer->tracks.size(); row++) {
        auto& track = sequencer->tracks[row];
        for (int col = 0; col < track.cells.size(); col++) {
            Cell& cell = track.cells[col];
            _cell(params.ctx, cell, newCoord, row, col);
        }
        newCoord.y += cellHeight + padding;
    }

    _background(params.ctx, coord);
}

void _clock(AppContext& ctx, Coord coord, int i)
{
    EltParams p(ctx);
    p.rect = _getClockRect(coord, i);
    p.color = inactiveColor;

    if (ctx.sequencer->isPlaying() == false) {
        p.displayColor = inactiveColor;
    }
    else {
        p.displayColor = ctx.sequencer->curStep == i ? activeColor : inactiveColor;
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
    p.displayColor = cell.on ? activeColor : inactiveColor;
    p.onClickColor = activeColor;

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

        p2.lineWidth = 2;

        rectOutlineElt(p2);
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

void _background(AppContext& ctx, Coord coord)
{
    auto& sequencer = ctx.sequencer;
    Coord bgCoord = coord;
    Color bgColor = grey;

    Rect bgRect{
        bgCoord.x,
        bgCoord.y,
        -3,
        ((cellWidth + padding) * (int)sequencer->numSteps) + padding,
        clockCellHeight + (padding * 2),
        bgColor
    };
    ctx.graphicsWrapper.drawRect(bgRect);

    bgCoord.y += clockCellHeight + (padding * 2);

    for (int i = 0; i < sequencer->tracks.size(); i++) {
        Rect bgRect2{
            bgCoord.x,
            bgCoord.y,
            -3,
            ((cellWidth + padding) * (int)sequencer->numSteps) + padding,
            cellHeight + (padding),
            bgColor
        };
        ctx.graphicsWrapper.drawRect(bgRect2);
        bgCoord.y += cellHeight + padding;
    }
}
