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
    NKSelectable(nk_context* ctx, NuklearUI* pManager);
    NKSelectable(const NKSelectable& other);
    ~NKSelectable();

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
};
#endif //NKSelectable_h__
