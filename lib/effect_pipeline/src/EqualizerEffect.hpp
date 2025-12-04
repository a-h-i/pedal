#pragma once
#include "Effect.hpp"


/**
 * Low-cut (HPF)
 * high-cut (LPF)
 * and output gain.
 * Implemented as two simple 1-pole filters in series, then gain.
 */
class EqualizerEffect : public Effect {
public:
    explicit EqualizerEffect(float sample_rate);

    /**
     * Set the low-cut (high pass) cutoff frequency
     * @param hz
     */
    void set_low_cut_hz(float hz);

    /**
     * Set the high-cut (low-pass) cutoff frequency
     * @param hz
     */
    void set_high_cut_hz(float hz);

    /**
     * Set output gain.
     * @param db [-24, 24]
     */
    void set_gain_db(float db);

    float nyquist() const { return nyquist_; }


    virtual void process_block(float *samples, std::size_t num_samples) override;

private:
    float sample_rate_;
    // Nyquist frequency
    float nyquist_;
    float low_cut_hz_{1000.0f};
    float high_cut_hz_{20000.0f};
    float gain_db_{0.0f};
    float gain_linear_{1.0f};


    // Coeffs/state for 1-pole low-pass used to build HPF
    float hp_lp_a_ = 0.0f; // a0 for LP inside HP
    float hp_lp_b_ = 0.0f; // b1 for LP inside HP
    float hp_lp_z_ = 0.0f; // state for LP inside HP

    // Coeffs/state for 1-pole LPF for high-cut
    float lp_a_ = 0.0f;
    float lp_b_ = 0.0f;
    float lp_z_ = 0.0f;

    void update_low_cut_coeffs();

    void update_high_cut_coeffs();

    void update_gain();
};
