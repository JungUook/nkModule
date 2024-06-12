#pragma once
#ifndef NKSelectable_h__
#define NKSelectable_h__
#include "NKBase.h"
#include "NKHandler.h"
class NKSelectable : public NKBase, public NKHandler
{
public:
    NKSelectable();
    ~NKSelectable();

public:
    virtual void Layout(nk_context* ctx) override;
    virtual void EditInfo() override;

    void SetLabel(const char* label);
    void SetSelected(bool selected);
    bool IsSelected() const;

public:
    char m_label[64];
    int m_selected;
};
#endif //NKSelectable_h__
