#pragma once
#ifndef NKSlider_h__
#define NKSlider_h__
#include "NKBase.h"
#include "NKHandler.h"
#include "NKStyleSlider.h"
class NKSlider : public NKBase, public NKHandler, public NKStyleSlider
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
    void LSetRange(luabridge::LuaRef ref);
    bool CSetRange(void* param);
    void SetValue(float value);
    void LSetValue(luabridge::LuaRef ref);
    bool CSetValue(void* param);
    float GetValue() const;
    bool CGetValue(void* param) const;

    virtual void RegistCommand(const char* classname) override;
public:
    float m_min;
    float m_max;
    float m_value;

public:
    template <class Archive>
    void serialize(Archive& ar, const unsigned int version) {
        if (version >= 13) {
            ar(cereal::base_class<NKBase>(this)
                , cereal::base_class<NKStyleSlider>(this)
                , CEREAL_NVP(m_min)
                , CEREAL_NVP(m_max)
                , CEREAL_NVP(m_value)
            );
        }
        else {
            ar(cereal::base_class<NKBase>(this)
                , cereal::base_class<NKHandler>(this)
                , cereal::base_class<NKStyleSlider>(this)
                , CEREAL_NVP(m_min)
                , CEREAL_NVP(m_max)
                , CEREAL_NVP(m_value)
            );
        }
    }
};
#endif //NKSlider_h__
