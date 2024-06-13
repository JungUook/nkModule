#include "pch.h"
#include "NKTree.h"

NKTree::NKTree(nk_context* ctx, NuklearUI* pManager) : NKBase(ctx, pManager)
{
    m_type = eTREE;
    m_state = NK_MINIMIZED;
    memset(m_label, 0, sizeof(m_label));
}

NKTree::~NKTree() {}

void NKTree::Layout(nk_context* ctx)
{
    if (nk_tree_push(ctx, NK_TREE_TAB, m_label, m_state))
    {
        for (auto& child : m_pChildList)
        {
            child->Update(ctx);
        }
        nk_tree_pop(ctx);
    }
}

void NKTree::SetLabel(const char* label)
{
    strcpy_s(m_label, label);
}

void NKTree::SetState(nk_collapse_states state)
{
    m_state = state;
}

nk_collapse_states NKTree::GetState() const
{
    return m_state;
}
