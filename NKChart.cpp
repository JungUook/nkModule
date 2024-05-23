#include "pch.h"
#include "NKChart.h"

NKChart::NKChart()
{
    m_type = eCHART;
    m_min = 0.0f;
    m_max = 1.0f;
}

NKChart::~NKChart() {}

void NKChart::Layout(nk_context* ctx)
{
    nk_chart_begin(ctx, NK_CHART_LINES, m_values.size(), m_min, m_max);
    for (float value : m_values)
    {
        nk_chart_push(ctx, value);
    }
    nk_chart_end(ctx);
}

void NKChart::AddValue(float value)
{
    m_values.push_back(value);
}

void NKChart::Clear()
{
    m_values.clear();
}
