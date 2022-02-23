#include "wasapi_wrapper.hpp"

#include "init.hpp"

#include <string>

unsigned WasapiWrapper::getBufferSizeFrames()
{
    unsigned bufferSize;
    HRESULT hr = audioClient->GetBufferSize(&bufferSize);

    if (FAILED(hr)) {
        throw std::runtime_error("ERROR " + std::to_string(hr) + ": GetBufferSize");
    }

    return bufferSize;
}

unsigned WasapiWrapper::getBufferSizeBytes()
{
    unsigned bufferSizeFrames = getBufferSizeFrames();

    unsigned bufferSizeBytes = (
        (
            bufferSizeFrames
            * waveFormat.Format.nChannels
            * waveFormat.Format.wBitsPerSample
        ) / 8
    );

    return bufferSizeBytes;
}

unsigned WasapiWrapper::getCurrentPadding()
{
    unsigned numPaddingFrames = 0;
    HRESULT hr = audioClient->GetCurrentPadding(&numPaddingFrames);

    if (FAILED(hr)) {
        throw std::runtime_error("ERROR " + std::to_string(hr) + ": GetCurrentPadding");
    }

    return numPaddingFrames;
}

unsigned WasapiWrapper::getPeriodSizeFrames()
{
    WAVEFORMATEX waveformat = {};
    WAVEFORMATEX* pWaveFormat = &waveformat;
    unsigned currentPeriodInFrames = 0;

    HRESULT hr = audioClient->GetCurrentSharedModeEnginePeriod(
        &pWaveFormat,
        &currentPeriodInFrames
    );

    if (FAILED(hr)) {
        throw std::runtime_error("ERROR " + toHex(hr) + ": getPeriodSizeFrames");
    }

    return currentPeriodInFrames;
}

void WasapiWrapper::waitForSignal()
{
    WaitForSingleObject(hEvent, INFINITE);
}

// recall that each elt of buffer stores 1 sample
// frame is 2 samples -> 1 for each channel
// so numSamplesToWrite = (2 * numFramesToWrite)
unsigned WasapiWrapper::getNumFramesToWrite()
{
    unsigned bufferSizeFrames = getBufferSizeFrames();
    unsigned numPaddingFrames = getCurrentPadding();
    return bufferSizeFrames - numPaddingFrames;
}

unsigned WasapiWrapper::getNumSamplesToWrite()
{
    return getNumFramesToWrite() * 2;
}

void WasapiWrapper::writeBuffer(uint32_t* source, unsigned numFramesToWrite)
{
    HRESULT hr;
    BYTE *dest = NULL;

    unsigned bufferSizeFrames = getBufferSizeFrames();

    unsigned bufferSizeBytes = getBufferSizeBytes();

    // after this call, dest will point to location in buffer to write to
    hr = renderClient->GetBuffer(numFramesToWrite, &dest);

    if (FAILED(hr)) {
        throw std::runtime_error("ERROR " + std::to_string(hr) + ": GetBuffer");
    }

    size_t size = sizeof(uint32_t) * 2 * numFramesToWrite;
    memcpy(dest, source, size);

    hr = renderClient->ReleaseBuffer(numFramesToWrite, 0);

    if (FAILED(hr)) {
        throw std::runtime_error("ERROR " + std::to_string(hr) + ": ReleaseBuffer");
    }
}

void WasapiWrapper::startPlaying()
{
    HRESULT hr = audioClient->Start();

    if (FAILED(hr)) {
        throw std::runtime_error("ERROR " + std::to_string(hr) + ": Start");
    }
}

void WasapiWrapper::stopPlaying()
{
    HRESULT hr = audioClient->Stop();

    if (FAILED(hr)) {
        throw std::runtime_error("ERROR " + std::to_string(hr) + ": Stop");
    }
}
