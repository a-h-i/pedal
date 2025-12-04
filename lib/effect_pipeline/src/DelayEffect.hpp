#pragma once
#include <vector>

#include "Effect.hpp"


class DelayEffect : public Effect {
public:
    DelayEffect(float sample_rate, float max_delay_ms);


    /**
     * (0, max_delay_ms) range
     * @param ms
     */
    void set_delay_ms(float ms);

    /**
     * [0,1) range
     * @param fb
     */
    void set_feedback(float fb);


    /**
     * dry/wet mix
     * @param mix 0 = fully dry, 1 = fully wet
     */
    void set_mix(float mix);

    float delay_ms() const { return delay_ms_; }
    float feedback() const { return feedback_; }
    float mix() const { return mix_; }
    float max_delay_ms() const { return max_delay_ms_; }


    void process_block(float *samples, std::size_t num_samples) override;

private:
    float sample_rate_;
    float max_delay_ms_;
    float delay_ms_{400.0f};
    float feedback_{0.4f};
    float mix_{0.4f};

    std::size_t max_delay_samples_{0};
    std::size_t write_idx_{0};
    std::size_t delay_samples_{0};
    std::vector<float> buffer;

    void updateDelaySamples();
};
