#include "EqualizerEffect.hpp"

#include <algorithm>
#include <cmath>


EqualizerEffect::EqualizerEffect(float sample_rate) : sample_rate_(sample_rate) {
    if (sample_rate <= 0.0f) {
        sample_rate_ = 44100.0f;
    }
    nyquist_ = 0.5f * sample_rate_;

    update_low_cut_coeffs();
    update_high_cut_coeffs();
    update_gain();
}


void EqualizerEffect::set_low_cut_hz(float hz) {
    low_cut_hz_ = std::clamp(hz, 0.0f, nyquist_);
    update_low_cut_coeffs();
}

void EqualizerEffect::set_high_cut_hz(float hz) {
    high_cut_hz_ = std::clamp(hz, 0.0f, nyquist_);
    update_high_cut_coeffs();
}

void EqualizerEffect::set_gain_db(float db) {
    gain_db_ = std::clamp(db, -24.0f, 24.0f);
    update_gain();
}

void EqualizerEffect::update_gain() {
    gain_linear_ = std::pow(10.0f, gain_db_ / 20.0f);
}


// For a 1-pole LPF we use:
// y[n] = a * x[n] + b * y[n-1]
// with a = 1 - x, b = x, x = exp(-2π fc / fs)

// Low-cut (HPF) is implemented as:
// LP of input: lp = LP(x)
// HP output:   hp = x - lp


void EqualizerEffect::update_low_cut_coeffs() {
    if (low_cut_hz_ == 0.0f) {
        hp_lp_a_ = 0.0f;
        hp_lp_b_ = 0.0f;
        return;
    }
    const float x = std::exp(-2.0f * M_PI * low_cut_hz_ / sample_rate_);
    hp_lp_a_ = 1.0f - x;
    hp_lp_b_ = x;
}

void EqualizerEffect::update_high_cut_coeffs() {
    if (high_cut_hz_ == 0.0f) {
        lp_a_ = 0.0f;
        lp_b_ = 0.0f;
        return;
    }
    const float x = std::exp(-2.0f * M_PI * high_cut_hz_ / sample_rate_);
    lp_a_ = x;
    lp_b_ = 1.0f - x;
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
            // first run an internal 1-pole LP
            float lp = hp_lp_a_ * y + hp_lp_b_ * hp_lp_z_;
            hp_lp_z_ = lp;
            y = y - lp;
        }
        if (high_cut_enabled) {
            float lp = lp_a_ * y + lp_b_ * lp_z_;
            lp_z_ = lp;
            y = lp;
        }

        if (gain_enabled) {
            y *= gain_linear_;
        }
        samples[i] = y;
    }
}
