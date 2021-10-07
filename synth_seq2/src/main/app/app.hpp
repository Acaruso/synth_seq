#pragma once

#include <functional>

#include "app_context.hpp"

class App
{
public:
    AppContext context;

    App(
        std::function<void(AppContext& context)> setup,
        std::function<void(AppContext& context)> callback
    );

    void run();

private:
    unsigned frameTimeMs{10};
    std::function<void(AppContext& context)> setup;
    std::function<void(AppContext& context)> callback;

    void handleMessagesFromAudioThread();
    void nextState();
};
