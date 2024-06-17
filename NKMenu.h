#pragma once
#ifndef NKMenu_h__
#define NKMenu_h__
#include "NKBase.h"
#include "NKHandler.h"
#include "NKBaseLabel.h"
#include "NKStyleMenuButton.h"

struct MenuItem {
    char name[256];
    int nameLen;
    CustomData data;
};

class NKMenu : public NKBase, public NKHandler, public NKBaseLabel, public NKStyleMenuButton
{
public:
    NKMenu(nk_context* ctx, NuklearUI* pManager);
    NKMenu(const NKMenu& other);
    ~NKMenu();

public:
    virtual void Layout(nk_context* ctx) override;
    virtual void EditInfo() override;
    virtual void EditStyle() override;
public:
    std::vector<MenuItem> m_items;
};
#endif //NKMenu_h__
