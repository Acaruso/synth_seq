#include "audio_system.hpp"

#include <cstdio>
#include <iostream>
#include <stdint.h>
#include <string>

#include "src/audio/audio_util.hpp"
#include "src/audio/wasapi_wrapper/init.hpp"

AudioSystem::AudioSystem(
    MessageQueue* toAudioQueue,
    MessageQueue* toMainQueue
)
    : toAudioQueue(toAudioQueue), toMainQueue(toMainQueue)
{
    wasapiWrapper = {};
    init(wasapiWrapper);

    unsigned samplesPerSecond = wasapiWrapper.waveFormat.Format.nSamplesPerSec;
    secondsPerSample = 1.0 / (double)samplesPerSecond;

    bufferSizeBytes = wasapiWrapper.getBufferSizeBytes();
    sampleBuffer = SampleBuffer(bufferSizeBytes);

    bufferSizeFrames = wasapiWrapper.getBufferSizeFrames();

    periodSizeFrames = wasapiWrapper.getPeriodSizeFrames();

    sliceTime = periodSizeFrames * 4;
    leadTime = sliceTime * 2;

    trigs = std::vector<bool>(numTracks, false);
    vSynthSettings = std::vector<SynthSettings>(numTracks, makeSynthSettings());

    initUgens();
}

void AudioSystem::playAudio()
{
    sampleBuffer.zero();

    wasapiWrapper.writeBuffer(sampleBuffer.buffer, bufferSizeFrames);

    wasapiWrapper.startPlaying();

    // main loop:
    while (!quit) {
        wasapiWrapper.waitForSignal();

        handleMessagesFromMainThread();

        unsigned numSamplesToWrite = wasapiWrapper.getNumSamplesToWrite();
        unsigned numFramesToWrite = wasapiWrapper.getNumFramesToWrite();

        fillSampleBuffer(numSamplesToWrite);

        wasapiWrapper.writeBuffer(sampleBuffer.buffer, numFramesToWrite);

        sendMessagesToMainThread();
    }

    wasapiWrapper.stopPlaying();
}

void AudioSystem::handleMessagesFromMainThread()
{
    Message message;

    while (toAudioQueue->try_dequeue(message)) {
        if (std::get_if<QuitMessage>(&message)) {
            std::cout << "audio thread: quitting" << std::endl;
            quit = true;
            break;
        }
        else if (std::get_if<PlayMessage>(&message)) {
            playing = true;
        }
        else if (std::get_if<StopMessage>(&message)) {
            playing = false;
        }
        else if (SynthSettingsMessage* p = std::get_if<SynthSettingsMessage>(&message)) {
            synthSettings = p->synthSettings;
            trig = true;
        }
        else if (EventMapMessage* p = std::get_if<EventMapMessage>(&message)) {
            // merge maps
            eventMap.insert(p->eventMap.begin(), p->eventMap.end());
        }
    }
}

void AudioSystem::sendMessagesToMainThread()
{
    if (playing && futureTransport % sliceTime == 0) {
        toMainQueue->enqueue(
            IntMessage("futureTransport", futureTransport)
        );
    }
}

void AudioSystem::fillSampleBuffer(size_t numSamplesToWrite)
{
    unsigned numChannels = 2;

    for (int i = 0; i < numSamplesToWrite; i += numChannels) {
        setTrigs();

        double sig = audioCallback();

        uint32_t samp = scaleSignal(sig);

        sampleBuffer.buffer[i] = samp;       // L
        sampleBuffer.buffer[i + 1] = samp;   // R

        ++sampleCounter;

        if (playing) {
            if (futureTransport >= leadTime) {
                ++presentTransport;
            }
            ++futureTransport;
        }
        else {
            futureTransport = 0;
            presentTransport = 0;
        }

        // need to unset trigs each sample
        // only want trig to be on for 1 sample
        unsetTrigs();
    }
}

void AudioSystem::setTrigs()
{
    if (futureTransport >= leadTime) {
        std::string key;

        for (int trackIdx = 0; trackIdx < numTracks; trackIdx++) {
            key = makeEventKey(presentTransport, trackIdx);

            if (eventMap.find(key) != eventMap.end()) {
                trigs[trackIdx] = true;
                vSynthSettings[trackIdx] = eventMap[key].synthSettings;
                eventMap.erase(key);
            }
        }
    }
}

void AudioSystem::unsetTrigs()
{
    trig = false;
    trigs.assign(trigs.size(), false);
}

double AudioSystem::getTime()
{
    return double(sampleCounter) * secondsPerSample;
}

AudioSystem::~AudioSystem()
{
    cleanUp(wasapiWrapper);
    sampleBuffer.cleanUp();
}
