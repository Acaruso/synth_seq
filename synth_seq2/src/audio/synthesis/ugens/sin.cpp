#include "sin.hpp"

#include <cmath>
#include <cstdio>

#include "src/audio/synthesis/consts.hpp"

void Sin::trigger(
    double a,
    double h,
    double r,
    double freq
) {
    this->freq = freq;
    env.trigger(a, h, r);
}

double Sin::get(double t)
{
    return get(0, t);
}

double Sin::get(double theta, double t)
{
    double envSig = env.get(t);

    double sinSig = sin(twoPi * freq * t + theta);
    double outSig = sinSig * envSig;
    return outSig;
}

SinWT::SinWT()
{
    double phase = 0.0;
    double delta = 1.0 / (double)size;

    // phase ranges from 0 to 1
    // multiply by twoPi to make it range from 0 to twoPi

    for (unsigned i = 0; i < size; i++) {
        wavetable.push_back(sin(phase * twoPi));
        phase += delta;
    }

    wavetable.push_back(0.0);
}

SinWT::SinWT(double secondsPerSample)
    : secondsPerSample(secondsPerSample)
{
    double phase = 0.0;
    double delta = 1.0 / (double)size;

    for (unsigned i = 0; i < size; i++) {
        wavetable.push_back(sin(phase * twoPi));
        phase += delta;
    }

    wavetable.push_back(0.0);
}

void SinWT::trigger(
    double a,
    double h,
    double r,
    double freq
) {
    this->freq = freq;
    env.trigger(a, h, r);
}

double SinWT::get(double t)
{
    return get(0, t);
}

double SinWT::get(double theta, double t)
{
    int i = (int)phase;
    double frac = phase - i;

    // linear interpolation
    double sinSig = wavetable[i] + (frac * (wavetable[i + 1] - wavetable[i]));

    double envSig = env.get(t);
    double outSig = sinSig * envSig;

    // get next phase
    phase += ((double)size * freq * secondsPerSample) + theta;

    // phase = phase % wavetable size
    double dSize = (double)size;

    while (phase >= dSize) {
        phase -= dSize;
    }

    while (phase < 0) {
        phase += dSize;
    }

    return outSig;
}
