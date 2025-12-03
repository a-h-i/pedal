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


UIManager::UIManager(const TremoloControls tremolo_controls) : tremolo_controls(tremolo_controls) {
}


void UIManager::setup() {
    if (setup_done) {
        return;
    }
    DEBUG("Performing UIManager Setup");
    analogReadResolution(ANALOG_RESOLUTION); // 0..4095
    analogReadAveraging(ANALOG_READ_AVG);
    pinMode(tremolo_controls.rate_pot_pin, INPUT);
    pinMode(tremolo_controls.depth_pot_pin, INPUT);
    pinMode(tremolo_controls.bypassed_led_pin, OUTPUT);
    setup_done = true;
}

void UIManager::update() {
    setup();
    updateTremolo();
}


void UIManager::updateTremolo() {
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
