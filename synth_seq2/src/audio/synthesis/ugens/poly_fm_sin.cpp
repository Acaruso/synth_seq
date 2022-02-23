#include "poly_fm_sin.hpp"

Operator::Operator(double secondsPerSample)
    : secondsPerSample(secondsPerSample)
{
    carrier = SinWT(secondsPerSample);
    modulator = SinWT(secondsPerSample);
}

bool Operator::isOn()
{
    return carrier.env.on;
}

void Operator::trigger(
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

double Operator::get(double t)
{
    double modSig = modulator.get(t) * modAmount * modScale;
    double carSig = carrier.get(modSig, t);
    return carSig;
}

PolyFmSin::PolyFmSin(int size, double secondsPerSample)
    : secondsPerSample(secondsPerSample)
{
    for (int i = 0; i < size; i++) {
        oscs.push_back(Operator(secondsPerSample));
    }
}

void PolyFmSin::trigger(
        double a,
        double h,
        double r,
        double ma,
        double mh,
        double mr,
        double modAmount,
        double freq
) {
    for (auto& osc : oscs) {
        if (!osc.isOn()) {
            osc.trigger(a, h, r, ma, mh, mr, modAmount, freq);
            return;
        }
    }
}

double PolyFmSin::get(double t)
{
    double out = 0.0;
    for (auto& osc : oscs) {
        out += osc.get(t);
    }
    return out;
}
