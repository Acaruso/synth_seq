#include "app.hpp"

#include <chrono>
#include <iostream>
#include <thread>

#include "lib/readerwriterqueue.h"

#include "src/audio/audio_entrypoint.hpp"

App::App()
    : uiSystem(context)
{
    toAudioQueue = MessageQueue(16);
    toMainQueue = MessageQueue(16);

    context.toAudioQueue = &toAudioQueue;
    context.toMainQueue = &toMainQueue;
    context.sequencer = &sequencer;

    try {
        context.graphicsWrapper.loadFont(
            "dos",
            "fonts/Perfect-DOS-VGA-437.ttf",
            16,
            9,
            20
        );

        context.graphicsWrapper.loadFont(
            "inconsolata",
            "fonts/Inconsolata-Regular.ttf",
            16,
            8,
            20
        );
    }
    catch(std::exception& ex) {
        std::cout << ex.what() << std::endl;
        throw ex;
    }
}

void App::run()
{
    std::thread audioThread(
        &audioEntrypoint,
        &toAudioQueue,
        &toMainQueue
    );

    while (!context.inputSystem.uiState.quit) {
        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
        context.inputSystem.run();
        uiSystem.handleUiEvents();

        handleMessagesFromAudioThread();

        context.graphicsWrapper.clearWindow();

        uiSystem.draw();

        sendMessagesToAudioThread();

        context.graphicsWrapper.render();

        nextState();

        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        Uint32 dur = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
        Uint32 time_to_delay = dur > frameTimeMs ? 0 : frameTimeMs - dur;
        std::this_thread::sleep_for(std::chrono::milliseconds(time_to_delay));
    }

    std::cout << "main thread: quitting" << std::endl;

    toAudioQueue.enqueue(QuitMessage());

    audioThread.join();

    context.graphicsWrapper.destroyWindow();
    context.graphicsWrapper.quit();
}

void App::handleMessagesFromAudioThread()
{
    Message message;

    while (toMainQueue.try_dequeue(message)) {
        if (IntMessage* p = std::get_if<IntMessage>(&message)) {
            if (p->key == "futureTransport") {
                sequencer.updateTransport(p->value);
                getEventMap = true;
            }
        }
    }
}

void App::sendMessagesToAudioThread()
{
    if (getEventMap) {
        EventMap eventMap = sequencer.getEventMap();
        EventMapMessage message(eventMap);
        toAudioQueue.enqueue(message);
    }
}

void App::nextState()
{
    context.eltId = 0;
    context.inputSystem.nextState();
    getEventMap = false;
}
