#pragma once
#include "BiQuadFilter.hpp"


class BiQuadHighPass : public BiQuadFilter {
    BiQuadHighPass(float sample_rate, float cutoff_hz, float Q);

    void set_params(float cutoff_hz, float Q) override;
};