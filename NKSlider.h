#pragma once
#ifndef NKSlider_h__
#define NKSlider_h__
#include "NKBase.h"
#include "NKStyleSlider.h"
class NKSlider : public NKBase, public NKStyleSlider
{
public:
    NKSlider();
    NKSlider(nk_context* ctx, NuklearUI* pManager);
    NKSlider(const NKSlider& other);
    virtual ~NKSlider();
    

public:
    virtual void Layout(nk_context* ctx) override;
    virtual void SafeRenderStart(nk_context* ctx) override;
    virtual void SafeRenderEnd(nk_context* ctx) override;
    virtual void EditInfo(nk_context* ctx) override;
    virtual void EditStyle(nk_context* ctx) override;

    void SetRange(float min, float max);
    void SetValue(float value);
    float GetValue() const;

public:
    float m_min;
    float m_max;
    float m_value;

public:
    template <class Archive>
    void serialize(Archive& ar) {
        ar(cereal::base_class<NKBase>(this)
            , cereal::base_class<NKStyleSlider>(this)
            , m_min
            , m_max
            , m_value
        );
    }
};
#endif //NKSlider_h__
