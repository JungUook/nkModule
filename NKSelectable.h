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
    virtual void Layout(nk_context* ctx) override;
    virtual void EditInfo() override;
    virtual void EditStyle() override;

    //NKBaseLabel
    virtual void SetLabel(const char* text) override;

    void SetSelected(bool selected);
    bool IsSelected() const;

public:
    int m_selected;

public:
    template <class Archive>
    void serialize(Archive& ar) {
        ar(cereal::base_class<NKBase>(this)
            , cereal::base_class<NKHandler>(this)
            , cereal::base_class<NKBaseLabel>(this)
            , cereal::base_class<NKStyleSelectedable>(this)
        );
    }
};
#endif //NKSelectable_h__
