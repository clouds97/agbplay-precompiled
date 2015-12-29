#pragma once

#include <cstdint>

namespace agbplay
{
    enum class CGBType : int { SQ1 = 0, SQ2, WAVE, NOISE };
    enum class EnvState : int { INIT = 0, ATK, DEC, SUS, REL, DEAD };
    enum class WaveDuty : int { D12 = 0, D25, D50, D75 };
    enum class NoisePatt : int { FINE = 0, ROUGH };

    union CGBDef
    {
        uint8_t *wavePtr;
        WaveDuty wd;
        NoisePatt np;
    };

    struct ADSR
    {
        ADSR(uint8_t att, uint8_t dec, uint8_t sus, uint8_t rel);
        ADSR();
        uint8_t att;
        uint8_t dec;
        uint8_t sus;
        uint8_t rel;
    };

    struct Note
    {
        Note(uint8_t midiKey, uint8_t velocity, int8_t length);
        Note();
        uint8_t midiKey;
        uint8_t velocity;
        int8_t length;
    };

    struct SampleInfo
    {
        SampleInfo(int8_t *samplePtr, float midCfreq, bool loopEnabled, uint32_t loopPos, uint32_t endPos);
        SampleInfo();
        int8_t *samplePtr;
        float midCfreq;
        uint32_t loopPos;
        uint32_t endPos;
        bool loopEnabled;
    };
}