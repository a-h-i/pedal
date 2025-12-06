#include <Arduino.h>
#include <Audio.h>
#include "debug.h"
#include <Wire.h>
#include <SD.h>


#include "FxProcessor.hpp"
#include "EffectChain.hpp"
#include "TremoloEffect.hpp"
#include "DelayEffect.hpp"
#include "UiManager.hpp"


// --------- Our DSP chain ----------
EffectChain effect_chain;
TremoloEffect tremolo_effect(AUDIO_SAMPLE_RATE_EXACT);
DelayEffect delay_effect(AUDIO_SAMPLE_RATE_EXACT, 1000.0f);
EqualizerEffect equalizer_effect(AUDIO_SAMPLE_RATE_EXACT);

// --------- UI ---------------
constexpr TremoloControls tremolo_controls{
    .rate_pot_pin = A10,
    .depth_pot_pin = A11,
    .bypassed_led_pin = A15,
    .bypass_switch_pin = 32,
    .effect = tremolo_effect
};

constexpr DelayControls delay_controls{
    .delay_ms_pot_pin = 38,
    .feedback_pot_pin = 37,
    .mix_pot_pin = 36,
    .bypassed_led_pin = 35,
    .bypass_switch_pin = 31,
    .effect = delay_effect
};

constexpr EqualizerControls equalizer_controls{
    .low_cut_hz_pot_pin = 26,
    .high_cut_hz_pot_pin = 27,
    .gain_pot_pin = 28,
    .bypassed_led_pin = 29,
    .bypass_switch_pin = 30,
    .effect = equalizer_effect
};

UIManager ui_manager(tremolo_controls, delay_controls, equalizer_controls);

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
    DEBUG("Starting setup");

    // Give Teensy Audio some memory for audio blocks
    AudioMemory(40);
    // Initialize I2C (for codec control)
    Wire.begin();

    if (!audio_shield.enable()) {
        FATAL("Failed to enable audio shield");
    } else {
        DEBUG("Initialized audio Shield");
    }
    audio_shield.inputSelect(AUDIO_INPUT_LINEIN);
    audio_shield.inputLevel(1.0f);
    audio_shield.volume(1.0f); // overall output volume (headphone/line out)
    audio_shield.lineOutLevel(255);
    audio_shield.lineInLevel(255);
    // effects

    equalizer_effect.set_low_cut_hz(80.0f);
    equalizer_effect.set_high_cut_hz(8000.0f);
    equalizer_effect.set_gain_db(0.0f);
    effect_chain.add_effect(&equalizer_effect);


    tremolo_effect.set_rate(5.0f);
    tremolo_effect.set_depth(0.7f);
    effect_chain.add_effect(&tremolo_effect);

    delay_effect.set_delay_ms(400.0f);
    delay_effect.set_feedback(0.5f);
    delay_effect.set_mix(0.5f);
    effect_chain.add_effect(&delay_effect);
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);
}

void loop() {
    ui_manager.update();
}