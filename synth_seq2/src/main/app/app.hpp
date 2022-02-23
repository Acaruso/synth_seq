#pragma once

#include <functional>
#include <unordered_map>

#include "lib/readerwriterqueue.h"

#include "app_context.hpp"
#include "src/main/sequencer/sequencer.hpp"
#include "src/main/ui_system/ui_system.hpp"
#include "src/shared/messages.hpp"
#include "src/shared/shared.hpp"

class App
{
public:
    AppContext context;
    MessageQueue toAudioQueue;
    MessageQueue toMainQueue;
    Sequencer sequencer;
    UiSystem uiSystem;

    App();

    void run();

private:
    unsigned frameTimeMs{10};
    bool getEventMap{false};

    void handleMessagesFromAudioThread();
    void sendMessagesToAudioThread();
    void nextState();
};
