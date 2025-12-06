#include "DelayEffect.hpp"

#include <algorithm>


DelayEffect::DelayEffect(float sample_rate, float max_delay_ms)
    : sample_rate_(sample_rate), max_delay_ms_(max_delay_ms) {
    if (sample_rate <= 0.0f) {
        sample_rate_ = 44100.0f;
    }
    if (max_delay_ms < 1.0f) {
        max_delay_ms_ = 1.0f;
    }
    max_delay_samples_ = static_cast<std::size_t>(
                             sample_rate_ * (max_delay_ms_ / 1000.0f)
                         ) + 1;
    buffer.assign(max_delay_samples_, 0.0f);
    updateDelaySamples();
}


void DelayEffect::set_delay_ms(float ms) {
    delay_ms_ = std::clamp(ms, 0.0f, max_delay_ms_);
    updateDelaySamples();
}

void DelayEffect::set_feedback(float fb) {
    feedback_ = std::clamp(fb, 0.0f, 0.95f);
}

void DelayEffect::set_mix(float mix) {
    mix_ = std::clamp(mix, 0.0f, 1.0f);
}

void DelayEffect::updateDelaySamples() {
    if (delay_ms_ <= 0.0f) {
        delay_samples_ = 0;
        return;
    }
    float samples = sample_rate_ * (delay_ms_ / 1000.0f);
    samples = std::clamp(samples, 1.0f, static_cast<float>(max_delay_samples_ - 1));
    delay_samples_ = static_cast<std::size_t>(samples);
}

void DelayEffect::process_block(float *samples, std::size_t num_samples) {
    if (!samples || num_samples == 0) {
        return;
    }
    if (bypassed || delay_samples_ == 0) {
        return;
    }
    const float dry_gain = 1.0f - mix_;
    const float wet_gain = mix_;

    const std::size_t buf_size = max_delay_samples_;
    std::size_t read_idx;
    for (std::size_t i = 0; i < num_samples; ++i) {
        float in = samples[i];
        // compute read index for this sample
        if (write_idx_ >= delay_samples_) {
            read_idx = write_idx_ - delay_samples_;
        } else {
            read_idx = buf_size + write_idx_ - delay_samples_;
        }

        float delayed = buffer[read_idx];
        float out = dry_gain * in + wet_gain * delayed;
        buffer[write_idx_++] = in + delayed * feedback_;

        if (write_idx_ >= buf_size) {
            write_idx_ = 0;
        }
        samples[i] = out;
    }
}