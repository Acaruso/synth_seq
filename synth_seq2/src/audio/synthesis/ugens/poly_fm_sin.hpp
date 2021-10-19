#pragma once

#include <vector>

#include "sin.hpp"

struct Operator
{
    SinWT carrier;
    SinWT modulator;

    double secondsPerSample{0};
    double modAmount;

    Operator() {}

    Operator(double secondsPerSample)
        : secondsPerSample(secondsPerSample)
    {
        carrier = SinWT(secondsPerSample);
        modulator = SinWT(secondsPerSample);
    }

    bool isOn()
    {
        return carrier.env.on;
    }

    void trigger(
        double a,
        double h,
        double r,
        double ma,
        double mh,
        double mr,
        double modAmount,
        double freq
    ) {
        this->modAmount = modAmount;
        carrier.trigger(a, h, r, freq);
        modulator.trigger(ma, mh, mr, freq);
    }

    double get(double t)
    {
        double modSig = modulator.get(t) * modAmount * 8;
        double carSig = carrier.get(modSig, t);
        return carSig;
    }
};

struct PolyFmSin
{
    double secondsPerSample{0};
    std::vector<Operator> oscs;

    PolyFmSin() {}

    PolyFmSin(int size, double secondsPerSample);

    void trigger(
        double a,
        double h,
        double r,
        double ma,
        double mh,
        double mr,
        double modAmount,
        double freq
    );

    double get(double t);
};
