#include "src/audio/audio_system/audio_system.hpp"

#include <iostream>
#include <vector>

#include "src/audio/audio_util.hpp"
#include "src/audio/synthesis/ugens/poly_fm_sin.hpp"
#include "src/audio/synthesis/ugens/smooth.hpp"

void AudioSystem::initUgens()
{
    polyFmSin = PolyFmSin(4, secondsPerSample);
    sins = std::vector<PolyFmSin>(8, PolyFmSin(4, secondsPerSample));
}

double AudioSystem::audioCallback()
{
    double t = getTime();
    double sig = 0.0;

    for (int i = 0; i < numTracks; i++) {
        if (trigs[i]) {
            auto& ss = vSynthSettings[i];
            double freq = mtof(ss["note"]);

            double modAmount = ((double)ss["modAmount"]) / 100.0;

            sins[i].trigger(
                (double)ss["attack"] * 2,
                (double)ss["hold"] * 20,
                (double)ss["release"] * 20,
                (double)ss["modAttack"] * 2,
                (double)ss["modHold"] * 20,
                (double)ss["modRelease"] * 20,
                modAmount,
                freq
            );
        }

        sig += sins[i].get(t);
    }

    if (trig) {
        double freq = mtof(synthSettings["note"]);

        double modAmount = ((double)synthSettings["modAmount"]) / 100.0;

        polyFmSin.trigger(
            (double)synthSettings["attack"] * 2,
            (double)synthSettings["hold"] * 20,
            (double)synthSettings["release"] * 20,
            (double)synthSettings["modAttack"] * 2,
            (double)synthSettings["modHold"] * 20,
            (double)synthSettings["modRelease"] * 20,
            modAmount,
            freq
        );
    }

    sig += polyFmSin.get(t);

    sig = sig * 0.1;

    return sig;
}
