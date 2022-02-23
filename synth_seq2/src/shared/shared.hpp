#pragma once

#include <iostream>
#include <string>
#include <unordered_map>

using SynthSettings = std::unordered_map<std::string, int>;

inline SynthSettings makeSynthSettings()
{
    SynthSettings synthSettings;

    synthSettings["volume"] = 100;
    synthSettings["modAmount"] = 0;
    synthSettings["attack"] = 2;
    synthSettings["hold"] = 20;
    synthSettings["release"] = 10;
    synthSettings["modAttack"] = 2;
    synthSettings["modHold"] = 20;
    synthSettings["modRelease"] = 10;
    synthSettings["note"] = 60;

    return synthSettings;
}

struct Event
{
    unsigned sample;
    int track;
    SynthSettings synthSettings;
    Event() {}
    Event(unsigned sample, int track, SynthSettings synthSettings)
        : sample(sample), track(track), synthSettings(synthSettings)
    {}
};

inline std::string makeEventKey(unsigned sample, int track)
{
    return std::to_string(sample) + "," + std::to_string(track);
}

using EventMap = std::unordered_map<std::string, Event>;

inline void printEventMap(EventMap map)
{
    for (auto& elt : map) {
        auto& s = elt.second.synthSettings;
        std::cout << "key: " << elt.first << std::endl;
        std::cout << "note: " << s["note"] << std::endl;
        std::cout << std::endl;
    }
}
