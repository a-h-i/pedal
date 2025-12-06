#include "BiQuadFilter.hpp"

BiQuadFilter::BiQuadFilter(float sample_rate, float cutoff_hz, float Q) : sample_rate_(sample_rate),
                                                                          cutoff_hz_(cutoff_hz), Q_(Q) {
}

float BiQuadFilter::process(float sample) {
    const float y = b0_ * sample + b1_ * x1_ + b2_ * x2_
                    - a1_ * y1_ - a2_ * y2_;

    x2_ = x1_;
    x1_ = sample;
    y2_ = y1_;
    y1_ = y;
    return y;
}

void BiQuadFilter::reset(float value) {
    x1_ = x2_ = y1_ = y2_ = value;
}
