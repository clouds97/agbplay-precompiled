#pragma once

#include <cstdint>
#include <cstddef>
#include <memory>

#include "Types.h"
#include "Resampler.h"

#define INVALID_TRACK_IDX 0xFF

#define NOISE_SAMPLING_FREQ 65536.0f

class CGBChannel
{
public: 
    CGBChannel(uint8_t track_idx, ADSR env, Note note, uint8_t vol, int8_t pan, int8_t instPan);
    CGBChannel(const CGBChannel&) = delete;
    CGBChannel& operator=(const CGBChannel&) = delete;
    virtual ~CGBChannel() = default;

    virtual void Process(sample *buffer, size_t numSamples, MixingArgs& args) = 0;
    uint8_t GetTrackIdx() const;
    void SetVol(uint8_t vol, int8_t pan);
    const Note& GetNote() const;
    void Release(bool fastRelease = false);
    virtual void SetPitch(int16_t pitch) = 0;
    bool TickNote(); // returns true if note remains active
    EnvState GetState() const;
    EnvState GetNextState() const;
protected:
    virtual void stepEnvelope();
    void updateVolFade();
    ChnVol getVol();
    enum class Pan { LEFT, CENTER, RIGHT };
    uint32_t pos = 0;
    float freq = 0.0f;
    ADSR env;
    Note note;
    EnvState eState = EnvState::INIT;
    EnvState nextState = EnvState::INIT;
    Pan pan = Pan::CENTER;
    std::unique_ptr<Resampler> rs;
    uint8_t envInterStep = 0;
    uint8_t envLevel = 0;
    uint8_t envPeak = 0;
    uint8_t envSustain = 0;
    uint8_t track_idx;
    const int8_t instPan;
    // these values are always 1 frame behind in order to provide a smooth transition
    Pan fromPan = Pan::CENTER;
    uint8_t fromEnvLevel = 0;
};

class SquareChannel : public CGBChannel
{
public:
    SquareChannel(uint8_t track_idx, WaveDuty wd, ADSR env, Note note, uint8_t vol, int8_t pan, int8_t instPan, int16_t pitch);

    void SetPitch(int16_t pitch) override;
    void Process(sample *buffer, size_t numSamples, MixingArgs& args) override;

    const float *pat = nullptr;
private:
    static bool sampleFetchCallback(std::vector<float>& fetchBuffer, size_t samplesRequired, void *cbdata);
};

class WaveChannel : public CGBChannel
{
public:
    WaveChannel(uint8_t track_idx, const uint8_t *wavePtr, ADSR env, Note note, uint8_t vol, int8_t pan, int8_t instPan, int16_t pitch);

    void SetPitch(int16_t pitch) override;
    void Process(sample *buffer, size_t numSamples, MixingArgs& args) override;
private:
    static bool sampleFetchCallback(std::vector<float>& fetchBuffer, size_t samplesRequired, void *cbdata);
    float waveBuffer[32];
    static uint8_t volLut[16];
};

class NoiseChannel : public CGBChannel
{
public:
    NoiseChannel(uint8_t track_idx, NoisePatt np, ADSR env, Note note, uint8_t vol, int8_t pan, int8_t instPan, int16_t pitch);

    void SetPitch(int16_t pitch) override;
    void Process(sample *buffer, size_t numSamples, MixingArgs& args) override;
private:
    static bool sampleFetchCallback(std::vector<float>& fetchBuffer, size_t samplesRequired, void *cbdata);
    SincResampler srs;
    NoisePatt np;
};
