#pragma once
#ifndef NKCheckbox_h__
#define NKCheckbox_h__
#include "NKBase.h"
#include "NKBaseLabel.h"
#include "NKStyleCheckbox.h"
#include "NKHandler.h"
class NKCheckbox : public NKBase, public NKBaseLabel, public NKHandler, public NKStyleCheckbox
{
public:
    NKCheckbox();
    NKCheckbox(nk_context* ctx, NuklearUI* pManager);
    NKCheckbox(const NKCheckbox& other);
    virtual ~NKCheckbox();
    

public:
    virtual void LayoutBegin(nk_context* ctx) override;
    virtual void Layout(nk_context* ctx) override;
    virtual void LayoutEnd(nk_context* ctx) override;
    virtual void SafeRenderStart(nk_context* ctx) override;
    virtual void SafeRenderEnd(nk_context* ctx) override;
    virtual void EditInfo(nk_context* ctx) override;
    virtual void EditStyle(nk_context* ctx) override;

    void SetChecked(bool checked);
    void LSetChecked(luabridge::LuaRef ref);
    bool CSetChecked(void* param);
    bool IsChecked() const;
    bool CIsChecked(void* param) const;

    virtual void RegistCommand(const char* classname) override;
public:
    int m_checked;

public:
    template <class Archive>
    void serialize(Archive& ar, const unsigned int version) {
        if (version >= 13) {
            ar(cereal::base_class<NKBase>(this)
                , cereal::base_class<NKBaseLabel>(this)
                , cereal::base_class<NKHandler>(this)
                , cereal::base_class<NKStyleCheckbox>(this)
                , CEREAL_NVP(m_checked)
            );
        }
        else {
            ar(cereal::base_class<NKBase>(this)
                , cereal::base_class<NKBaseLabel>(this)
                , cereal::base_class<NKStyleCheckbox>(this)
                , CEREAL_NVP(m_checked)
            );
        }
    }
};
#endif //NKCheckbox_h__
