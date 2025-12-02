#include "EffectChain.hpp"


void EffectChain::add_effect(Effect *effect) {
    if (effect != nullptr && num_effects < MAX_EFFECTS) {
        effects[num_effects++] = effect;
    }
}
