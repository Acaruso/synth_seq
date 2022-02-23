#pragma once

#include "src/main/app/app_context.hpp"
#include "src/main/sequencer/cell.hpp"

class UiSystem
{
public:
    UiSystem(AppContext& context);
    void draw();
    void handleUiEvents();

private:
    AppContext& context;
    Cell copiedCell;
};
