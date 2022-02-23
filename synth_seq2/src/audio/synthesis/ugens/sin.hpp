#pragma once

#include <vector>

#include "src/audio/synthesis/ugens/ahr_env.hpp"

struct Sin
{
    AHREnv env;
    double freq;

    Sin() {};

    void trigger(
        double a,
        double h,
        double r,
        double freq
    );

    double get(double t);
    double get(double theta, double t);
};

struct SinWT
{
    AHREnv env;
    double freq;
    double secondsPerSample;
    double phase{0};

    unsigned size{1024};
    std::vector<double> wavetable;

    SinWT();

    SinWT(double secondsPerSample);

    void trigger(
        double a,
        double h,
        double r,
        double freq
    );

    double get(double t);
    double get(double theta, double t);
};
