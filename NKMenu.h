#pragma once
#ifndef NKMenu_h__
#define NKMenu_h__
#include "NKBase.h"
#include "NKHandler.h"

struct MenuItem {
    char name[256];
    int nameLen;
    CustomData data;
};

class NKMenu : public NKBase, public NKHandler
{
public:
    NKMenu(nk_context* ctx, NuklearUI* pManager);
    ~NKMenu();

public:
    virtual void Layout(nk_context* ctx) override;
    virtual void EditInfo() override;

    void SetLabel(const char* label);
public:
    char m_label[64];
    std::vector<MenuItem> m_items;
};
#endif //NKMenu_h__
