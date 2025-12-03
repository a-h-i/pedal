#pragma once
#include "Effect.hpp"


class DelayEffect : public Effect {
public:
    void process_block(float *samples, std::size_t num_samples) override;
};
