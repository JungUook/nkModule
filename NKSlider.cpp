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
    m_cTransform.x = 0.f;
    m_cTransform.y = 0.f;
    m_cTransform.w = 150.f;
    m_cTransform.h = 40.f;
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

void NKSlider::EditInfo(nk_context* ctx)
{
    nk_layout_row_dynamic(ctx, 44, 1);
    nk_slider_float(ctx, m_min, &m_value, m_max, 0.01f);
    nk_property_float(ctx, "#Value", m_min, &m_value, m_max, 1, 0.01f);

    nk_property_float(ctx, "#min", -10000.f, &m_min, 100000.f, 1.f, 0.01f);
    nk_property_float(ctx, "#max", m_min, &m_max, 200000.f, 1.f, 0.01f);
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

void NKSlider::LSetRange(luabridge::LuaRef ref)
{
    CHECK_LUA_REF(ref);
    float min = ref["min"].cast<float>();
    float max = ref["max"].cast<float>();
    SetRange(min, max);
}

bool NKSlider::CSetRange(void* param)
{
    return false;
}

void NKSlider::SetValue(float value)
{
    m_value = value;
}

void NKSlider::LSetValue(luabridge::LuaRef ref)
{
    CHECK_LUA_REF(ref);
    float value = ref.cast<float>();
    SetValue(value);
}

bool NKSlider::CSetValue(void* param)
{
    return false;
}

float NKSlider::GetValue() const
{
    return m_value;
}

bool NKSlider::CGetValue(void* param) const
{
    float* value = static_cast<float*>(param);

    if (value) {
        (*value) = m_value;
        return true;
    }
    return false;
}

void NKSlider::RegistCommand()
{
    NKBase::RegistCommand();
    MAKE_INTERFACE(m_mapFunc, this, NKSlider::CSetRange, "NKSlider");
    MAKE_INTERFACE(m_mapFunc, this, NKSlider::CSetValue, "NKSlider");
    MAKE_INTERFACE(m_mapFunc, this, NKSlider::CGetValue, "NKSlider");
}
