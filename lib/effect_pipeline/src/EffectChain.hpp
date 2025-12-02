#pragma once
#include <cstddef>
#include "Effect.hpp"
#include <array>

class EffectChain {
public:
    static constexpr size_t MAX_EFFECTS = 8;

    EffectChain() = default;

    /**
     * Adds an effect to the chain.
     * Order of effect being applied is same as order of calls.
     * @param effect
     */
    void add_effect(Effect *effect);

    /**
     * process a block through all effects in order
     * @param samples array of num_samples floats in [-1.0, 1.0] range.
     * @param num_samples
     */
    template<std::size_t N>
    void process_block(std::array<float, N> &samples) const {
        for (std::size_t i = 0; i < num_effects; i++) {
            if (Effect *fx = effects[i]; fx != nullptr && !fx->is_bypassed()) {
                fx->process_block(samples.data(), samples.size());
            }
        }
    };

    std::size_t size() const { return effects.size(); }
private:
    std::array<Effect*, MAX_EFFECTS> effects{nullptr};
    std::size_t num_effects{0};
};