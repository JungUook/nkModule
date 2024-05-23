#pragma once
#ifndef NKSelectable_h__
#define NKSelectable_h__
#include "NKBase.h"
class NKSelectable : public NKBase
{
public:
    NKSelectable();
    ~NKSelectable();

public:
    void Layout(nk_context* ctx) override;
    void SetLabel(const char* label);
    void SetSelected(bool selected);
    bool IsSelected() const;

public:
    char m_label[64];
    int m_selected;
};
#endif //NKSelectable_h__
