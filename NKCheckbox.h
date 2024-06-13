#pragma once
#ifndef NKCheckbox_h__
#define NKCheckbox_h__
#include "NKBase.h"
class NKCheckbox : public NKBase
{
public:
    NKCheckbox(nk_context* ctx, NuklearUI* pManager);
    ~NKCheckbox();

public:
    virtual void Layout(nk_context* ctx) override;
    virtual void EditInfo() override;


    void SetLabel(const char* label);
    void SetChecked(bool checked);
    bool IsChecked() const;

public:
    char m_label[64];
    int m_checked;
};
#endif //NKCheckbox_h__
