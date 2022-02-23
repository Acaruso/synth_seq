#pragma once

#include <cstring>
#include <stdint.h>

class SampleBuffer
{
public:
    uint32_t* buffer;
    unsigned numSamples;

    SampleBuffer() {}

    SampleBuffer(unsigned bufferSizeBytes) {
        numSamples = bufferSizeBytes / sizeof(uint32_t);
        buffer = new uint32_t[numSamples];
    }

    void zero()
    {
        memset(buffer, 0, numSamples * sizeof(uint32_t));
    }

    void cleanUp()
    {
        delete[] buffer;
    }
};
