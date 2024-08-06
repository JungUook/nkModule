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
    char functionName[64];
    int functionNameLen;
    char argsName[64];
    int argsNameLen;

    MenuItem() : nameLen(0), functionNameLen(0), argsNameLen(0) {
        std::memset(name, 0, sizeof(name));
        std::memset(functionName, 0, sizeof(functionName));
        std::memset(argsName, 0, sizeof(argsName));
    }

    template <class Archive>
    void serialize(Archive& ar, const unsigned int version) {
        ar(CEREAL_NVP(name)
            , CEREAL_NVP(nameLen)
            , CEREAL_NVP(functionName)
            , CEREAL_NVP(functionNameLen)
            , CEREAL_NVP(argsName)
            , CEREAL_NVP(argsNameLen)
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

    virtual void RegistCommand(const char* classname) override;
public:
    std::vector<MenuItem> m_items;

public:
    template <class Archive>
    void serialize(Archive& ar, const unsigned int version) {
        ar(cereal::base_class<NKBase>(this)
            , cereal::base_class<NKHandler>(this)
            , cereal::base_class<NKBaseLabel>(this)
            , cereal::base_class<NKStyleMenuButton>(this)
            , CEREAL_NVP(m_items)
        );
    }
};
#endif //NKMenu_h__
