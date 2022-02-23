#pragma once

#include <functional>
#include <vector>

#include "lib/readerwriterqueue.h"

#include "src/audio/audio_system/sample_buffer.hpp"
#include "src/audio/synthesis/ugens/poly_fm_sin.hpp"
#include "src/audio/wasapi_wrapper/wasapi_wrapper.hpp"
#include "src/shared/messages.hpp"
#include "src/shared/shared.hpp"

class AudioSystem
{
public:
    MessageQueue* toAudioQueue;
    MessageQueue* toMainQueue;

    AudioSystem(
        MessageQueue* toAudioQueue,
        MessageQueue* toMainQueue
    );

    void playAudio();
    ~AudioSystem();

private:
    WasapiWrapper wasapiWrapper;
    SampleBuffer sampleBuffer;
    unsigned bufferSizeBytes;
    unsigned bufferSizeFrames;
    unsigned periodSizeFrames;

    unsigned sampleCounter{0};

    unsigned sliceTime{0};
    unsigned leadTime{0};
    unsigned presentTransport{0};
    unsigned futureTransport{0};

    bool playing{false};

    double secondsPerSample{0.0};

    bool trig{false};
    SynthSettings synthSettings;

    EventMap eventMap;

    int numTracks{6};
    std::vector<bool> trigs;
    std::vector<SynthSettings> vSynthSettings;

    bool quit{false};

    PolyFmSin polyFmSin;
    std::vector<PolyFmSin> sins;

    void initUgens();
    double audioCallback();

    void handleMessagesFromMainThread();
    void sendMessagesToMainThread();
    void setTrigs();
    void unsetTrigs();
    double getTime();
    void fillSampleBuffer(size_t numSamplesToWrite);
};
