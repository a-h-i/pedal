#include "EqualizerEffect.hpp"

#include <algorithm>
#include <cmath>


EqualizerEffect::EqualizerEffect(float sample_rate) : sample_rate_(sample_rate),
                                                      low_pass_(sample_rate, 0.75 * sample_rate),
                                                      high_pass_(sample_rate, 0.5 * sample_rate) {
    if (sample_rate <= 0.0f) {
        sample_rate_ = 44100.0f;
    }
    nyquist_ = 0.5f * sample_rate_;
    update_gain();
}


void EqualizerEffect::set_low_cut_hz(float hz) {
    low_cut_hz_ = std::clamp(hz, 0.0f, nyquist_);
    low_pass_.set_params(low_cut_hz_, 0.7071f);
}

void EqualizerEffect::set_high_cut_hz(float hz) {
    high_cut_hz_ = std::clamp(hz, 0.0f, nyquist_);
    high_pass_.set_params(high_cut_hz_, 0.7071f);
}

void EqualizerEffect::set_gain_db(float db) {
    gain_db_ = std::clamp(db, -24.0f, 24.0f);
    update_gain();
}

void EqualizerEffect::update_gain() {
    gain_linear_ = std::pow(10.0f, gain_db_ / 20.0f);
}


void EqualizerEffect::process_block(float *samples, std::size_t num_samples) {
    if (!samples || num_samples == 0 || bypassed) {
        return;
    }
    const bool low_cut_enabled = low_cut_hz_ > 0.0f;
    const bool high_cut_enabled = high_cut_hz_ > 0.0f;
    const bool gain_enabled = std::abs(gain_linear_) > 1e-4f;
    if (!low_cut_enabled && !high_cut_enabled && !gain_enabled) {
        return;
    }
    for (std::size_t i = 0; i < num_samples; ++i) {
        float y = samples[i];

        if (low_cut_enabled) {
            y = low_pass_.process(y);
        }
        if (high_cut_enabled) {
            y = high_pass_.process(y);
        }

        if (gain_enabled) {
            y *= gain_linear_;
        }
        samples[i] = y;
    }
}