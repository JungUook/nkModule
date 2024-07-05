#pragma once
#ifndef NKTree_h__
#define NKTree_h__
#include "NKBase.h"
#include "NKBaseLabel.h"
#include "NKStyleTab.h"

class NKTree : public NKBase, public NKBaseLabel, public NKStyleTab
{
public:
    NKTree();
    NKTree(nk_context* ctx, NuklearUI* pManager);
    NKTree(const NKTree& other);
    virtual ~NKTree();   

public:
    virtual void LayoutBegin(nk_context* ctx) override;
    virtual void Layout(nk_context* ctx) override;
    virtual void LayoutEnd(nk_context* ctx) override;
    virtual void SafeRenderStart(nk_context* ctx) override;
    virtual void SafeRenderEnd(nk_context* ctx) override;
    virtual void EditInfo(nk_context* ctx) override;
    virtual void EditStyle(nk_context* ctx) override;

    void SetState(nk_collapse_states state);
    void LSetState(luabridge::LuaRef ref);
    bool CSetState(void* param);
    nk_collapse_states GetState() const;
    bool CGetState(void* param) const;

    virtual void RegistCommand() override;
public:
    nk_tree_type m_treeType;
    nk_collapse_states m_state;

public:
    template <class Archive>
    void serialize(Archive& ar) {
        ar(cereal::base_class<NKBase>(this)
            , cereal::base_class<NKBaseLabel>(this)
            , cereal::base_class<NKStyleTab>(this)
            , m_treeType
            , m_state
        );
    }
};
#endif //NKTree_h__
