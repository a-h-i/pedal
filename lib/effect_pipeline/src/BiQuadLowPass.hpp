#pragma once
#include "BiQuadFilter.hpp"

class BiQuadLowPass : public BiQuadFilter {
public:
    BiQuadLowPass(float sample_rate, float cutoff_hz, float Q = 0.7071f);

    void set_params(float cutoff_hz, float Q) override;
};