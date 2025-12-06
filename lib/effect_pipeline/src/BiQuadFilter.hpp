#pragma once


class BiQuadFilter {
public:
    BiQuadFilter(float sample_rate, float cutoff_hz, float Q = 0.7071f);

    virtual ~BiQuadFilter() = default;

    virtual void set_params(float cutoff_hz, float Q) = 0;

    float process(float sample);

    void reset(float value = 0.0f);

protected:
    float sample_rate_, cutoff_hz_, Q_;
    float b0_ = 0, b1_ = 0, b2_ = 0, a1_ = 0, a2_ = 0;
    // State
    float x1_ = 0, x2_ = 0, y1_ = 0, y2_ = 0;
};
