#pragma once

#include "src/shared/shared.hpp"

struct Cell
{
    bool on{false};
    SynthSettings synthSettings;

    Cell()
    {
        synthSettings = makeSynthSettings();
    }

    SynthSettings& getSynthSettings()
    {
        return synthSettings;
    }
};
