#include "UiManager.hpp"
#include <cmath>


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
    analogReadResolution(12); // 0..4095
    pinMode(tremolo_controls.rate_pot_pin, INPUT);
    setup_done = true;
}

void UIManager::update() {
    setup();
    updateTremolo();
}


void UIManager::updateTremolo() {
    // read raw ADC value
    int raw_rate = analogRead(tremolo_controls.rate_pot_pin);
    float normalized_rate = raw_rate / 4095.0f; // 0..1

    if (!tremolo_initialized) {
        smoothed_tremolo_rate = normalized_rate;
        tremolo_initialized = true;
    }
    // Simple smoothing (one-pole low pass)
    // alpha near 0 → more smoothing, near 1 → less
    constexpr float alpha = 0.15f;
    smoothed_tremolo_rate = smoothed_tremolo_rate + alpha * (normalized_rate - smoothed_tremolo_rate);
    // Map [0,1] to a useful tremolo rate range, e.g. 0.3–12 Hz
    const float rate_hz = mapNormToRate(smoothed_tremolo_rate);

    tremolo_controls.effect.set_rate(rate_hz);
}
