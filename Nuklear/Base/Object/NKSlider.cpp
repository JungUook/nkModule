#include "pch.h"
#include "NKSlider.h"

NKSlider::NKSlider() : NKBase(), NKHandler(), NKStyleSlider()
{
    m_type = eSLIDER;
    m_min = 0.0f;
    m_max = 1.0f;
    m_value = 0.0f;
}

NKSlider::NKSlider(nk_context* ctx, NuklearUI* pManager) : NKBase(ctx, pManager), NKHandler(), NKStyleSlider(ctx, &m_style)
{
    m_type = eSLIDER;
    m_min = 0.0f;
    m_max = 1.0f;
    m_value = 0.0f;
    m_sTransform.w = 150.f;
    m_sTransform.h = 40.f;
}

NKSlider::NKSlider(const NKSlider& other) : NKBase(other), NKHandler(other), NKStyleSlider(other, m_ctx, &m_style)
{
    m_type = other.m_type;
    m_min = other.m_min;
    m_max = other.m_max;
    m_value = other.m_value;
}

NKSlider::~NKSlider() {}

void NKSlider::Layout(nk_context* ctx)
{
    if (nk_slider_float(ctx, m_min, &m_value, m_max, 0.01f)) {
        CallEvent(m_pLuaManager);
    }
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

    EditInfoData(ctx, m_pManager, m_pLuaManager);
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
    void** arr = static_cast<void**>(param);

    if (arr) {
        float* min = static_cast<float*>(arr[0]);
        float* max = static_cast<float*>(arr[1]);

        if (min && max) {
            SetRange(*min, *max);
            return true;
        }
        else {
            return false;
        }
    }

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
    float* value = static_cast<float*>(param);
    if (value) {
        SetValue(*value);
        return true;
    }
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

void NKSlider::RegistCommand(const char* classname)
{
    NKBase::RegistCommand(classname);
    MAKE_INTERFACE(m_mapFunc, this, NKSlider::CSetRange, classname);
    MAKE_INTERFACE(m_mapFunc, this, NKSlider::CSetValue, classname);
    MAKE_INTERFACE(m_mapFunc, this, NKSlider::CGetValue, classname);
}
