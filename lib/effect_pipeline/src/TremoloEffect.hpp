#pragma once
#include "Effect.hpp"


/**
 * Simple tremolo effect
 * amplitude modulation with a sin LFO
 */
class TremoloEffect: public Effect {


public:
    /**
     *
     * @param sampleRate audio sample rate in Hz (e.g., 44100 or AUDIO_SAMPLE_RATE_EXACT)
     */
    explicit TremoloEffect(float sampleRate);


    void set_rate(float rate);

    /**
     *
     * @param depth tremolo depth in [0, 1]. 0 no effect, 1 full modulation down
     */
    void set_depth(float depth);

    float rate() const { return rate_;}
    float depth() const { return depth_;}

    void process_block(float *samples, std::size_t num_samples) override;


private:
    void updatePhaseIncrement();
    float sampleRate_;
    float rate_{4.0f};
    float depth_{0.7f};
    // current LFO phase [0, 2π]
    float phase_{0.0f};
    // precomputed per-sample increment
    float phase_increment_{0.0f};



};