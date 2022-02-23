#pragma once

#include <algorithm>
#include <vector>

#include "src/main/sequencer/cell.hpp"
#include "src/shared/shared.hpp"

struct Track
{
    SynthSettings synthSettings;
    std::vector<Cell> cells;

    Track()
    {
        synthSettings = makeSynthSettings();
        int numCells = 16;
        cells = std::vector<Cell>(numCells, Cell());
    }

    SynthSettings& getSynthSettings()
    {
        return synthSettings;
    }
};
