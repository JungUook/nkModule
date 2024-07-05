#pragma once
#ifndef NKSelectable_h__
#define NKSelectable_h__
#include "NKBase.h"
#include "NKHandler.h"
#include "NKBaseLabel.h"
#include "NKStyleSelectedable.h"
class NKSelectable : public NKBase, public NKHandler, public NKBaseLabel, public NKStyleSelectedable
{
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

    virtual void RegistCommand() override;
public:
    int m_selected;

public:
    template <class Archive>
    void serialize(Archive& ar) {
        ar(cereal::base_class<NKBase>(this)
            , cereal::base_class<NKHandler>(this)
            , cereal::base_class<NKBaseLabel>(this)
            , cereal::base_class<NKStyleSelectedable>(this)
            , m_selected
        );
    }
};
#endif //NKSelectable_h__
