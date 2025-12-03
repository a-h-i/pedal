#include "TremoloEffect.hpp"

#include <algorithm>
#include <cmath>


TremoloEffect::TremoloEffect(float sampleRate) : sampleRate_(sampleRate) {
    updatePhaseIncrement();
}


void TremoloEffect::set_rate(float rate) {
    rate_ = std::clamp(rate, 0.1f, 20.0f);
    updatePhaseIncrement();
}

void TremoloEffect::set_depth(float depth) {
    depth_ = std::clamp(depth, 0.0f, 1.0f);
}


void TremoloEffect::updatePhaseIncrement() {
    // Δphase per sample = 2π * f / Fs

    phase_increment_ = M_2_PI * rate_ / sampleRate_;
}


void TremoloEffect::process_block(float *samples, std::size_t num_samples) {
    if (!samples || num_samples == 0) {
        return;
    }

    // If this effect is bypassed or depth is zero, do nothing
    if (bypassed || depth_ == 0.0f) {
        return;
    }
    for (std::size_t i = 0; i < num_samples; ++i) {
        // Advance LFO phase
        phase_ += phase_increment_;
        if (phase_ >= M_2_PI) {
            phase_ -= M_2_PI;
        }
        float lfo = std::sin(phase_);
        // Map to [0, 1]
        float lfo01 = 0.5f * (lfo + 1.0f);
        // Gain: between (1 - depth) and 1
        // depth = 0 → gain = 1.0 all the time (no effect)
        // depth = 1 → gain ranges 0..1 (full tremolo)
        float gain = (1.0f - depth_) + depth_ * lfo01;
        samples[i] *= gain;
    }
}