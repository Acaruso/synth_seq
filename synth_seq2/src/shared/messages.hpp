#pragma once

#include <string>
#include <variant>

#include "lib/readerwriterqueue.h"

#include "src/shared/shared.hpp"

struct QuitMessage {};

struct PlayMessage {};

struct StopMessage {};

struct IntMessage
{
    std::string key;
    int value;
    IntMessage(std::string key, int value) : key(key), value(value) {}
};

struct BoolMessage
{
    std::string key;
    bool value;
    BoolMessage(std::string key, bool value) : key(key), value(value) {}
};

struct DoubleMessage
{
    std::string key;
    double value;
    DoubleMessage() {}
    DoubleMessage(std::string key, double value) : key(key), value(value) {}
};

struct StringMessage
{
    std::string key{""};
    std::string value{""};
    StringMessage(std::string key) : key(key) {}
    StringMessage(std::string key, std::string value) : key(key), value(value) {}
};

struct EventMapMessage
{
    EventMap eventMap;
    EventMapMessage(EventMap eventMap) : eventMap(eventMap) {}
};

struct SynthSettingsMessage
{
    SynthSettings synthSettings;
    SynthSettingsMessage(SynthSettings synthSettings) : synthSettings(synthSettings) {}
};

using Message = std::variant<
    QuitMessage,
    PlayMessage,
    StopMessage,
    IntMessage,
    BoolMessage,
    DoubleMessage,
    StringMessage,
    EventMapMessage,
    SynthSettingsMessage
>;

using MessageQueue = moodycamel::ReaderWriterQueue<Message>;
