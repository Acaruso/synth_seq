#pragma once

#include <vector>

#include "sin.hpp"

struct Operator
{
    SinWT carrier;
    SinWT modulator;

    double secondsPerSample{0};
    double modAmount{0};
    double modScale{16};

    Operator() {}

    Operator(double secondsPerSample);

    bool isOn();

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
