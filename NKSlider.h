#pragma once
#ifndef NKSlider_h__
#define NKSlider_h__
#include "NKBase.h"
#include "NKStyleSlider.h"
class NKSlider : public NKBase, public NKStyleSlider
{
public:
    NKSlider(nk_context* ctx, NuklearUI* pManager);
    NKSlider(const NKSlider& other);
    ~NKSlider();

public:
    virtual void Layout(nk_context* ctx) override;
    virtual void EditStyle() override;

    void SetRange(float min, float max);
    void SetValue(float value);
    float GetValue() const;

public:
    float m_min;
    float m_max;
    float m_value;
};
#endif //NKSlider_h__
