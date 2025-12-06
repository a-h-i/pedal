#include "BiQuadHighPass.hpp"

#include <cmath>


BiQuadHighPass::BiQuadHighPass(float sample_rate, float cutoff_hz, float Q) : BiQuadFilter(sample_rate, cutoff_hz, Q) {
    set_params(cutoff_hz, Q);
}

void BiQuadHighPass::set_params(float cutoff_hz, float Q) {
    cutoff_hz_ = cutoff_hz;
    Q_ = Q;
    const float w0 = 2.0f * M_PI * cutoff_hz_ / sample_rate_;
    const float cos_w0 = std::cos(w0);
    const float sin_w0 = std::sin(w0);
    const float alpha = sin_w0 / (2.0f * Q_);

    float b0 = (1.0f + cos_w0) * 0.5f;
    float b1 = -(1.0f + cos_w0);
    float b2 = (1.0f + cos_w0) * 0.5f;
    float a0 = 1.0f + alpha;
    float a1 = -2.0f * cos_w0;
    float a2 = 1.0f - alpha;


    b0_ = b0 / a0;
    b1_ = b1 / a0;
    b2_ = b2 / a0;
    a1_ = a1 / a0;
    a2_ = a2 / a0;
}
