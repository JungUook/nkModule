#pragma once
#ifndef NKSlider_h__
#define NKSlider_h__
#include "NKBase.h"
class NKSlider : public NKBase
{
public:
    NKSlider();
    ~NKSlider();

public:
    void Layout(nk_context* ctx) override;
    void SetRange(float min, float max);
    void SetValue(float value);
    float GetValue() const;

public:
    float m_min;
    float m_max;
    float m_value;
};
#endif //NKSlider_h__
