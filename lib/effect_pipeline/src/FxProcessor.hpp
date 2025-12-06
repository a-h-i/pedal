#pragma once
#include <Arduino.h>
#include <AudioStream.h>

class EffectChain;
/**
 * This is a custom node that:
 *  1. Receives a mono Teensy audio block (int16)
 *  2. Converts to float in [-1,1] range
 *  3. Runs the EffectChain
 *  4. Converts back to int16 and outputs
 */
class FxProcessor : public AudioStream {
public:
    explicit FxProcessor(EffectChain *chain);

    virtual ~FxProcessor() = default;

    /**
     * Global effect bypass
     * @param bypass
     */
    void set_bypass(const bool bypass) { bypassed = bypass; };
    bool is_bypassed() const { return bypassed; }


    /**
     * Called by the Teensy audio library in the audio interrupt.
     */
    virtual void update() override;

private:
    audio_block_t *inputQueueArray[1]{nullptr};
    EffectChain *chain{nullptr};
    bool bypassed{false};
};