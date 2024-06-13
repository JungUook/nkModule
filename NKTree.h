#pragma once
#ifndef NKTree_h__
#define NKTree_h__
#include "NKBase.h"
class NKTree : public NKBase
{
public:
    NKTree(nk_context* ctx, NuklearUI* pManager);
    ~NKTree();

public:
    void Layout(nk_context* ctx) override;
    void SetLabel(const char* label);
    void SetState(nk_collapse_states state);
    nk_collapse_states GetState() const;

public:
    char m_label[64];
    nk_collapse_states m_state;
};
#endif //NKTree_h__
