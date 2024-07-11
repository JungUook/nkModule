#pragma once
#ifndef NKChart_h__
#define NKChart_h__
#include "NKBase.h"
#include "NKStyleChart.h"
class NKChart : public NKBase, public NKStyleChart
{
public:
    NKChart();
    NKChart(nk_context* ctx, NuklearUI* pManager);
    NKChart(const NKChart& other);
    virtual ~NKChart();
    

public:
    virtual void Layout(nk_context* ctx) override;
    virtual void SafeRenderStart(nk_context* ctx) override;
    virtual void SafeRenderEnd(nk_context* ctx) override;
    virtual void EditStyle(nk_context* ctx) override;
    void AddValue(float value);
    bool CAddValue(void* param);
    void Clear();
    bool CClear(void* param);

    virtual void RegistCommand(const char* classname) override;
public:
    std::vector<float> m_values;
    float m_min;
    float m_max;

public:
    template <class Archive>
    void serialize(Archive& ar, const unsigned int version) {
        ar(cereal::base_class<NKBase>(this)
            , cereal::base_class<NKStyleChart>(this)
            , CEREAL_NVP(m_values)
            , CEREAL_NVP(m_min)
            , CEREAL_NVP(m_max)
        );
    }
};
#endif //NKChart_h__
