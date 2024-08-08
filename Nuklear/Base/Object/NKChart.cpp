#include "pch.h"
#include "NKChart.h"

NKChart::NKChart() : NKBase(), NKStyleChart()
{
    m_type = eCHART;
    m_min = 0.0f;
    m_max = 1.0f;
}

NKChart::NKChart(nk_context* ctx, NuklearUI* pManager) : NKBase(ctx, pManager), NKStyleChart(ctx, &m_style)
{
    m_type = eCHART;
    m_min = 0.0f;
    m_max = 1.0f;
    m_sTransform.w = 150.f;
    m_sTransform.h = 150.f;
}

NKChart::NKChart(const NKChart& other) : NKBase(other), NKStyleChart(other, m_ctx, &m_style)
{
    m_type = other.m_type;
    m_min = other.m_min;
    m_max = other.m_max;
}

NKChart::~NKChart() {}

void NKChart::Layout(nk_context* ctx)
{
    nk_chart_begin(ctx, NK_CHART_LINES, m_values.size(), m_min, m_max);
    for (auto it = m_values.begin(); it != m_values.end(); ++it)
    {
        nk_chart_push(ctx, *it);
    }
    nk_chart_end(ctx);
}

void NKChart::SafeRenderStart(nk_context* ctx)
{
    UpdateComponent(ctx, m_pManager);
}

void NKChart::SafeRenderEnd(nk_context* ctx)
{
}

void NKChart::EditStyle(nk_context* ctx)
{
    NKBase::EditStyle(ctx);
    EditComponentStyle(ctx, m_pManager);
}

void NKChart::AddValue(float value)
{
    m_values.push_back(value);
}

bool NKChart::CAddValue(void* param)
{
    float* value = static_cast<float*>(param);
    if (value) {
        AddValue(*value);
        return true;
    }

    return false;
}

void NKChart::Clear()
{
    m_values.clear();
}

bool NKChart::CClear(void* param)
{
    Clear();
    return true;
}

void NKChart::RegistCommand(const char* classname)
{
    NKBase::RegistCommand(classname);
    MAKE_INTERFACE(m_mapFunc, this, NKChart::CAddValue, classname);
    MAKE_INTERFACE(m_mapFunc, this, NKChart::CClear, classname);
}
