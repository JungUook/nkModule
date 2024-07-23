#pragma once
#ifndef NKSelectable_h__
#define NKSelectable_h__
#include "NKBase.h"
#include "NKHandler.h"
#include "NKBaseLabel.h"
#include "NKStyleSelectedable.h"
#include "NKBaseImage.h"
class NKSelectable : public NKBase, public NKHandler, public NKBaseLabel, public NKStyleSelectedable, public NKBaseImage
{
public:
    enum {
        eSELECTABLE_LABEL,
        eSELECTABLE_IMAGELABEL,
    };
public:
    NKSelectable();
    NKSelectable(nk_context* ctx, NuklearUI* pManager);
    NKSelectable(const NKSelectable& other);
    virtual ~NKSelectable();
    

public:
    virtual void LayoutBegin(nk_context* ctx) override;
    virtual void Layout(nk_context* ctx) override;
    virtual void LayoutEnd(nk_context* ctx) override;
    virtual void SafeRenderStart(nk_context* ctx) override;
    virtual void SafeRenderEnd(nk_context* ctx) override;
    virtual void EditInfo(nk_context* ctx) override;
    virtual void EditStyle(nk_context* ctx) override;

    //NKBaseLabel
    virtual void SetLabel(const char* text) override;

    void SetSelected(bool selected);
    void LSetSelected(luabridge::LuaRef ref);
    bool CSetSelected(void* param);
    bool IsSelected() const;
    bool CIsSelected(void* param) const;

    virtual void RegistCommand(const char* classname) override;
public:
    int m_iSelectableType;
    int m_selected;

    int m_iLeft;
    int m_iCenter;
    int m_iRight;
    int m_iTop;
    int m_iMiddle;
    int m_iBottom;
    nk_flags m_fLabelType;
public:
    template <class Archive>
    void serialize(Archive& ar, const unsigned int version) {
        if (version >= 4) {
            ar(cereal::base_class<NKBase>(this)
                , cereal::base_class<NKHandler>(this)
                , cereal::base_class<NKBaseLabel>(this)
                , cereal::base_class<NKStyleSelectedable>(this)
                , cereal::base_class<NKBaseImage>(this)
                , CEREAL_NVP(m_iSelectableType)
                , CEREAL_NVP(m_selected)
                , CEREAL_NVP(m_iLeft)
                , CEREAL_NVP(m_iCenter)
                , CEREAL_NVP(m_iRight)
                , CEREAL_NVP(m_iTop)
                , CEREAL_NVP(m_iMiddle)
                , CEREAL_NVP(m_iBottom)
                , CEREAL_NVP(m_fLabelType)
            );
        }
        else {
            ar(cereal::base_class<NKBase>(this)
                , cereal::base_class<NKHandler>(this)
                , cereal::base_class<NKBaseLabel>(this)
                , cereal::base_class<NKStyleSelectedable>(this)
                , CEREAL_NVP(m_selected)
            );
        }
    }
};
#endif //NKSelectable_h__
