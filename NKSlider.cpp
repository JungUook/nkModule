#include "pch.h"
#include "NKSlider.h"

NKSlider::NKSlider() : NKBase(), NKStyleSlider()
{
    m_type = eSLIDER;
    m_min = 0.0f;
    m_max = 1.0f;
    m_value = 0.0f;
}

NKSlider::NKSlider(nk_context* ctx, NuklearUI* pManager) : NKBase(ctx, pManager), NKStyleSlider(ctx, &m_style)
{
    m_type = eSLIDER;
    m_min = 0.0f;
    m_max = 1.0f;
    m_value = 0.0f;
}

NKSlider::NKSlider(const NKSlider& other) : NKBase(other), NKStyleSlider(other, m_ctx, &m_style)
{
    m_type = other.m_type;
    m_min = other.m_min;
    m_max = other.m_max;
    m_value = other.m_value;
}

NKSlider::~NKSlider() {}

void NKSlider::Layout(nk_context* ctx)
{
    UpdateComponent(ctx, m_pManager);

    nk_slider_float(ctx, m_min, &m_value, m_max, 0.01f);
}

void NKSlider::SafeRenderStart(nk_context* ctx)
{
    UpdateComponent(ctx, m_pManager);
}

void NKSlider::SafeRenderEnd(nk_context* ctx)
{
}

void NKSlider::EditStyle(nk_context* ctx)
{
    EditComponentStyle(ctx, m_pManager);
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
