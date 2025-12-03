#include <Arduino.h>
#include <Audio.h>
#include "debug.h"
#include <Wire.h>
#include <SD.h>


#include "FxProcessor.hpp"
#include "EffectChain.hpp"
#include "TremoloEffect.hpp"


// --------- Our DSP chain ----------
EffectChain effectChain;
TremoloEffect tremoloEffect(AUDIO_SAMPLE_RATE_EXACT);

// --------- Teensy Audio objects ----------

// Codec I/O
AudioInputI2S audioInput; // from SGTL5000
FxProcessor fxProcessor(&effectChain); // our custom node
AudioOutputI2S audioOutput; // back to SGTL5000


// Patch cords (mono: left channel only)
AudioConnection patchCord1(audioInput, 0, fxProcessor, 0);
AudioConnection patchCord2(fxProcessor, 0, audioOutput, 0);

// Audio shield control
AudioControlSGTL5000 audioShield;


void setup() {
    Serial.begin(115200);
    // Give Teensy Audio some memory for audio blocks
    AudioMemory(20);
    // Initialize I2C (for codec control)
    Wire.begin();

    if (!audioShield.enable()) {
        FATAL("Failed to enable audio shield");
    }

    audioShield.inputSelect(AUDIO_INPUT_LINEIN);
    audioShield.volume(0.6f); // overall output volume (headphone/line out)

    tremoloEffect.set_rate(4.0f);
    tremoloEffect.set_depth(0.7f);
    effectChain.add_effect(&tremoloEffect);
}

void loop() {
    // Later: read pots, footswitch, update parameters here.
}
