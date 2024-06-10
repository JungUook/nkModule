#pragma once
#ifndef NKMenu_h__
#define NKMenu_h__
#include "NKBase.h"
class NKMenu : public NKBase
{
public:
    NKMenu();
    ~NKMenu();

public:
    void Layout(nk_context* ctx) override;
    void SetLabel(const char* label);
    void AddMenuItem(const char* label, nk_context* ctx);

public:
    char m_label[64];
    std::vector<const char*> m_items;
};
#endif //NKMenu_h__
