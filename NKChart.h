#pragma once
#ifndef NKChart_h__
#define NKChart_h__
#include "NKBase.h"
#include "NKStyleChart.h"
class NKChart : public NKBase, public NKStyleChart
{
public:
    NKChart(nk_context* ctx, NuklearUI* pManager);
    NKChart(const NKChart& other);
    ~NKChart();

public:
    virtual void Layout(nk_context* ctx) override;
    virtual void EditStyle() override;
    void AddValue(float value);
    void Clear();

public:
    std::vector<float> m_values;
    float m_min;
    float m_max;
};
#endif //NKChart_h__
