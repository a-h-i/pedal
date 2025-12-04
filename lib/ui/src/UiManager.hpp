#pragma once
#include <Arduino.h>
#include "TremoloEffect.hpp"
#include "DelayEffect.hpp"


struct TremoloControls {
    int rate_pot_pin, depth_pot_pin, bypassed_led_pin;
    TremoloEffect &effect;
};

struct DelayControls {
    int delay_ms_pot_pin, feedback_pot_pin, mix_pot_pin, bypassed_led_pin;
    DelayEffect &effect;
};


class UIManager {
public:
    explicit UIManager(const TremoloControls tremolo_controls, const DelayControls delay_controls);

    void update();

private:
    static constexpr float ADC_MAX = 4095.0f;
    static constexpr int ANALOG_RESOLUTION = 12;
    static constexpr int ANALOG_READ_AVG = 32;
    TremoloControls tremolo_controls;
    DelayControls delay_controls;
    bool setup_done{false};

    void setup();

    void updateTremolo() const;

    void updateDelay() const;
};
