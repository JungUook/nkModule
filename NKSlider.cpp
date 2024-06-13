#include "pch.h"
#include "NKSlider.h"

NKSlider::NKSlider(nk_context* ctx, NuklearUI* pManager) : NKBase(ctx, pManager)
{
    m_type = eSLIDER;
    m_min = 0.0f;
    m_max = 1.0f;
    m_value = 0.0f;
}

NKSlider::~NKSlider() {}

void NKSlider::Layout(nk_context* ctx)
{
    nk_slider_float(ctx, m_min, &m_value, m_max, 0.01f);
}

void NKSlider::SetRange(float min, float max)
{
    m_min = min;
    m_max = max;
}

void NKSlider::SetValue(float value)
{
    m_value = value;
}

float NKSlider::GetValue() const
{
    return m_value;
}
