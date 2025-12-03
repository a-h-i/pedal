#pragma once
#include <Arduino.h>
#include "TremoloEffect.hpp"


struct TremoloControls {
    int rate_pot_pin;

    TremoloEffect &effect;
};


class UIManager {
public:
    explicit UIManager(const TremoloControls tremolo_controls);
    void update();

private:
    TremoloControls tremolo_controls;
    float smoothed_tremolo_rate{0.0f};
    bool tremolo_initialized{false};
    bool setup_done{false};
    void setup();
    void updateTremolo();
};