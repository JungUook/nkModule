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

    MenuItem() : nameLen(0) {
        std::memset(name, 0, sizeof(name));
    }

    template <class Archive>
    void serialize(Archive& ar) {
        ar(name
            , nameLen
            , data
        );
    }
};

class NKMenu : public NKBase, public NKHandler, public NKBaseLabel, public NKStyleMenuButton
{
public:
    NKMenu();
    NKMenu(nk_context* ctx, NuklearUI* pManager);
    NKMenu(const NKMenu& other);
    virtual ~NKMenu();
    

public:
    virtual void LayoutBegin(nk_context* ctx) override;
    virtual void Layout(nk_context* ctx) override;
    virtual void LayoutEnd(nk_context* ctx) override;
    virtual void SafeRenderStart(nk_context* ctx) override;
    virtual void SafeRenderEnd(nk_context* ctx) override;
    virtual void EditInfo(nk_context* ctx) override;
    virtual void EditStyle(nk_context* ctx) override;
public:
    std::vector<MenuItem> m_items;

public:
    template <class Archive>
    void serialize(Archive& ar) {
        ar(cereal::base_class<NKBase>(this)
            , cereal::base_class<NKHandler>(this)
            , cereal::base_class<NKBaseLabel>(this)
            , cereal::base_class<NKStyleMenuButton>(this)
            , m_items
        );
    }
};
#endif //NKMenu_h__
