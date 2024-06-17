#pragma once
#ifndef NKTree_h__
#define NKTree_h__
#include "NKBase.h"
#include "NKBaseLabel.h"
#include "NKStyleTab.h"

class NKTree : public NKBase, public NKBaseLabel, public NKStyleTab
{
public:
    NKTree(nk_context* ctx, NuklearUI* pManager);
    NKTree(const NKTree& other);
    ~NKTree();

public:
    virtual void Layout(nk_context* ctx) override;
    virtual void EditInfo() override;
    virtual void EditStyle() override;

    void SetState(nk_collapse_states state);
    nk_collapse_states GetState() const;

public:
    nk_tree_type m_treeType;
    nk_collapse_states m_state;
};
#endif //NKTree_h__
