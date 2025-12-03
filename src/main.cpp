#include <Arduino.h>
#include <Audio.h>
#include "debug.h"
#include <Wire.h>
#include <SD.h>


#include "FxProcessor.hpp"
#include "EffectChain.hpp"
#include "TremoloEffect.hpp"
#include "UiManager.hpp"


// --------- Our DSP chain ----------
EffectChain effect_chain;
TremoloEffect tremolo_effect(AUDIO_SAMPLE_RATE_EXACT);


// --------- UI ---------------
constexpr TremoloControls tremolo_controls{
    .rate_pot_pin = A2, // wire pot wiper here
    .effect = tremolo_effect
};
UIManager ui_manager(tremolo_controls);

// --------- Teensy Audio objects ----------

// Codec I/O
AudioInputI2S audio_input; // from SGTL5000
FxProcessor fx_processor(&effect_chain); // our custom node
AudioOutputI2S audio_output; // back to SGTL5000


// Patch cords (mono: left channel only)
AudioConnection patch_cord_1(audio_input, 0, fx_processor, 0);
AudioConnection patch_cord_2(fx_processor, 0, audio_output, 0);

// Audio shield control
AudioControlSGTL5000 audio_shield;


void setup() {
    Serial.begin(115200);
    // Give Teensy Audio some memory for audio blocks
    AudioMemory(20);
    // Initialize I2C (for codec control)
    Wire.begin();

    if (!audio_shield.enable()) {
        FATAL("Failed to enable audio shield");
    }

    audio_shield.inputSelect(AUDIO_INPUT_LINEIN);
    audio_shield.volume(0.6f); // overall output volume (headphone/line out)

    tremolo_effect.set_rate(4.0f);
    tremolo_effect.set_depth(0.7f);
    effect_chain.add_effect(&tremolo_effect);
}

void loop() {
    ui_manager.update();
    delay(2);
}
