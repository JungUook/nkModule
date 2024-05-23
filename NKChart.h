#pragma once
#ifndef NKChart_h__
#define NKChart_h__
#include "NKBase.h"
#include <vector>

class NKChart : public NKBase
{
public:
    NKChart();
    ~NKChart();

public:
    void Layout(nk_context* ctx) override;
    void AddValue(float value);
    void Clear();

public:
    std::vector<float> m_values;
    float m_min;
    float m_max;
};
#endif //NKChart_h__
