#pragma once
#include <cstddef>


class Effect {
public:
    virtual ~Effect() = default;

    /**
     * process a block of mono sample in-place
     *
     * @param samples array of num_samples floats in [-1.0, 1.0] range.
     * @param num_samples
     */
    virtual void process_block(float *samples, std::size_t num_samples) = 0;

    void bypass(const bool bypass) { bypassed = bypass; }
    bool is_bypassed() const { return bypassed; }

protected:
    bool bypassed{false};
};