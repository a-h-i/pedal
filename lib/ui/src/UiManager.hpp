#pragma once
#include <Arduino.h>
#include "TremoloEffect.hpp"
#include "DelayEffect.hpp"
#include "EqualizerEffect.hpp"


struct TremoloControls {
    int rate_pot_pin, depth_pot_pin, bypassed_led_pin, bypass_switch_pin;
    TremoloEffect &effect;
};

struct DelayControls {
    int delay_ms_pot_pin, feedback_pot_pin, mix_pot_pin, bypassed_led_pin, bypass_switch_pin;
    DelayEffect &effect;
};

struct EqualizerControls {
    int low_cut_hz_pot_pin, high_cut_hz_pot_pin, gain_pot_pin, bypassed_led_pin, bypass_switch_pin;
    EqualizerEffect &effect;
};


class UIManager {
public:
    explicit UIManager(TremoloControls tremolo_controls, DelayControls delay_controls,
                       EqualizerControls equalizer_controls);

    void update();

private:
    static constexpr float ADC_MAX = 4095.0f;
    static constexpr int ANALOG_RESOLUTION = 12;
    static constexpr int ANALOG_READ_AVG = 32;
    TremoloControls tremolo_controls;
    DelayControls delay_controls;
    EqualizerControls equalizer_controls;
    bool setup_done{false};

    void setup();

    void update_tremolo() const;

    void update_delay() const;

    void update_equalizer() const;
};