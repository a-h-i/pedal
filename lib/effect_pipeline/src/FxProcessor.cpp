#include "FxProcessor.hpp"
#include "EffectChain.hpp"
#include <array>
#include "debug.h"


FxProcessor::FxProcessor(EffectChain *chain) : AudioStream(1, inputQueueArray), chain(chain) {
}


void FxProcessor::update() {
    // Get a writable audio block from input channel 0
    audio_block_t *block = receiveWritable(0);
    if (!block) {
        DEBUG("FxProcessor receive writable block failed");
        return;
    }

    if (!chain || bypassed) {
        transmit(block, 0);
        release(block);
        return;
    }
    std::array<float, AUDIO_BLOCK_SAMPLES> buffer{};

    // Convert int16 -> float [-1, 1]
    constexpr float inv32768 = 1.0f / 32768.0f;
    for (std::size_t i = 0; i < buffer.size(); ++i) {
        buffer[i] = static_cast<float>(block->data[i]) * inv32768;
    }

    // Run the effect chain
    chain->process_block(buffer);

    // Convert float [-1, 1] -> int16
    for (std::size_t i = 0; i < buffer.size(); ++i) {
        block->data[i] = static_cast<int16_t>(buffer[i] * 32768.0f);
    }
    // Send processed audio downstream
    transmit(block, 0);
    release(block);
}