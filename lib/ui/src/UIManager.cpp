#include "UiManager.hpp"
#include <cmath>
#include "debug.h"
#include "FxProcessor.hpp"

/**
 * Exponential style mapping for nicer feel
 * low end has finer control, high end bunches up a bit.
 * @param norm
 * @return
 */
static float mapNormToRate(float norm) {
    const float min_hz = 0.3f;
    const float max_hz = 12.0f;
    norm = std::clamp(norm, 0.0f, 1.0f);

    // Exponential-ish mapping:
    // rate = min * (max/min)^x
    const float ratio = max_hz / min_hz;
    return min_hz * std::pow(ratio, norm);
}


UIManager::UIManager(TremoloControls tremolo_controls,
                     DelayControls delay_controls,
                     EqualizerControls equalizer_controls) : tremolo_controls(tremolo_controls),
                                                             delay_controls(delay_controls),
                                                             equalizer_controls(equalizer_controls) {
}


void UIManager::setup() {
    if (setup_done) {
        return;
    }
    DEBUG("Performing UIManager Setup");
    analogReadResolution(ANALOG_RESOLUTION); // 0..4095
    analogReadAveraging(ANALOG_READ_AVG);
    // Tremolo
    pinMode(tremolo_controls.rate_pot_pin, INPUT);
    pinMode(tremolo_controls.depth_pot_pin, INPUT);
    pinMode(tremolo_controls.bypassed_led_pin, OUTPUT);
    // Delay
    pinMode(delay_controls.delay_ms_pot_pin, INPUT);
    pinMode(delay_controls.feedback_pot_pin, INPUT);
    pinMode(delay_controls.mix_pot_pin, INPUT);
    pinMode(delay_controls.bypassed_led_pin, OUTPUT);
    // Equalizer
    pinMode(equalizer_controls.gain_pot_pin, INPUT);
    pinMode(equalizer_controls.low_cut_hz_pot_pin, INPUT);
    pinMode(equalizer_controls.high_cut_hz_pot_pin, INPUT);
    pinMode(equalizer_controls.bypassed_led_pin, OUTPUT);

    setup_done = true;
}

void UIManager::update() {
    setup();
    update_equalizer();
    update_tremolo();
    update_delay();
}


void UIManager::update_tremolo() const {
    // read raw ADC value
    const int raw_rate = analogRead(tremolo_controls.rate_pot_pin);
    const float normalized_rate = raw_rate / ADC_MAX; // 0..1
    // Map [0,1] to a useful tremolo rate range, e.g. 0.3–12 Hz
    const float rate_hz = mapNormToRate(normalized_rate);
    tremolo_controls.effect.set_rate(rate_hz);

    const int raw_depth = analogRead(tremolo_controls.depth_pot_pin);
    const float normalized_depth = raw_depth / ADC_MAX;
    tremolo_controls.effect.set_depth(normalized_depth);
    const bool active = !tremolo_controls.effect.is_bypassed();
    digitalWrite(tremolo_controls.bypassed_led_pin, active ? LOW : HIGH);
}

void UIManager::update_delay() const {
    const int raw_delay_ms = analogRead(delay_controls.delay_ms_pot_pin);
    const float normalized_delay_ms = raw_delay_ms / ADC_MAX;
    delay_controls.effect.set_delay_ms(normalized_delay_ms * delay_controls.effect.max_delay_ms());

    const int raw_feedback = analogRead(delay_controls.feedback_pot_pin);
    const float normalized_feedback = raw_feedback / ADC_MAX;
    delay_controls.effect.set_feedback(normalized_feedback);

    const int raw_mix = analogRead(delay_controls.mix_pot_pin);
    const float normalized_mix = raw_mix / ADC_MAX;
    delay_controls.effect.set_mix(normalized_mix);
    const bool active = !delay_controls.effect.is_bypassed();
    digitalWrite(delay_controls.bypassed_led_pin, active ? LOW : HIGH);
}

void UIManager::update_equalizer() const {
    const int raw_low_cut = analogRead(equalizer_controls.low_cut_hz_pot_pin);
    const float normalized_low_cut = raw_low_cut / ADC_MAX;
    equalizer_controls.effect.set_low_cut_hz(normalized_low_cut * equalizer_controls.effect.nyquist());

    const int raw_high_cut = analogRead(equalizer_controls.high_cut_hz_pot_pin);
    const float normalized_high_cut = raw_high_cut / ADC_MAX;
    equalizer_controls.effect.set_high_cut_hz(normalized_high_cut * equalizer_controls.effect.nyquist());

    const int raw_gain = analogRead(equalizer_controls.gain_pot_pin);
    const float normalized_gain = raw_gain / ADC_MAX;
    const float gain_db = 48 * normalized_gain - 24;
    equalizer_controls.effect.set_gain_db(gain_db);

    const bool active = !equalizer_controls.effect.is_bypassed();
    digitalWrite(equalizer_controls.bypassed_led_pin, active ? LOW : HIGH);
}

