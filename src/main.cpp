#include <Arduino.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>

#define FATAL(x) Serial.print("FATAL: "); Serial.println(x); if (CrashReport) Serial.print(CrashReport)
#define DEBUG(x) Serial.print("DEBUG: "); Serial.println(x)

// Audio objects
AudioSynthWaveform      waveform1;    // Beep generator
AudioInputI2S           micInput;     // SGTL5000 mic/line input
AudioMixer4             mixer1;       // Mix beep + mic
AudioOutputI2S          audioOutput;  // To headphones/lineout
AudioControlSGTL5000    audioShield;  // SGTL5000 control

// Connections:
//  waveform1 -> mixer1 ch0
//  micInput  -> mixer1 ch1
//  mixer1    -> audioOutput (L & R)
AudioConnection patchCord1(waveform1, 0, mixer1, 0);
AudioConnection patchCord2(micInput, 1, mixer1, 1);
AudioConnection patchCord5(micInput, 0, mixer1, 1);
AudioConnection patchCord3(mixer1, 0, audioOutput, 0);
AudioConnection patchCord4(mixer1, 0, audioOutput, 1);

void setup() {
    Serial.begin(115200);
    while (!Serial);
    delay(3000);
    delay(1000);

    AudioMemory(20);

    if (!audioShield.enable()) {
        FATAL("Failed to enable audio shield");
    } else {
        DEBUG("Audio shield enabled");
    }
    audioShield.inputSelect(AUDIO_INPUT_LINEIN);  // using mic; use AUDIO_INPUT_LINEIN if needed
    audioShield.lineInLevel(13);
    audioShield.volume(0.7f);
    audioShield.lineOutLevel(13);

    // Configure mixer: start with beep only
    mixer1.gain(0, 0.7f);   // ch0: beep
    mixer1.gain(1, 0.0f);   // ch1: mic
    mixer1.gain(2, 0.0f);
    mixer1.gain(3, 0.0f);

    // Configure beep
    waveform1.begin(WAVEFORM_SINE);
    waveform1.frequency(1000.0f);  // 1 kHz
    waveform1.amplitude(0.8f);     // audible but not too loud

    // Play beep for 300 ms
    delay(10000);

    // Stop beep
    waveform1.amplitude(0.0f);

    // Now switch to mic passthrough: mute beep, enable mic
    mixer1.gain(0, 0.0f);   // beep off
    mixer1.gain(1, 0.8f);   // mic on

    Serial.println("Beep done, mic passthrough active");
}

void loop() {
    // Nothing needed; audio library runs in the background
}