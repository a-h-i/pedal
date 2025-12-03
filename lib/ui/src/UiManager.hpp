#pragma once
#include <Arduino.h>
#include "TremoloEffect.hpp"


struct TremoloControls {
    int rate_pot_pin, depth_pot_pin;
    TremoloEffect &effect;
};


class UIManager {
public:
    explicit UIManager(const TremoloControls tremolo_controls);

    void update();

private:
    static constexpr float ADC_MAX = 4095.0f;
    static constexpr int ANALOG_RESOLUTION = 12;
    static constexpr int ANALOG_READ_AVG = 32;
    TremoloControls tremolo_controls;
    bool setup_done{false};

    void setup();

    void updateTremolo();
};
