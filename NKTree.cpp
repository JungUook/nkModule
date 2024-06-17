#include "pch.h"
#include "NKTree.h"

NKTree::NKTree(nk_context* ctx, NuklearUI* pManager) : NKBase(ctx, pManager), NKBaseLabel(), NKStyleTab(ctx, &m_style)
{
    m_treeType = NK_TREE_NODE;
    m_type = eTREE;
    m_state = NK_MINIMIZED;
}

NKTree::NKTree(const NKTree& other) : NKBase(other), NKBaseLabel(other), NKStyleTab(other)
{
    m_treeType = other.m_treeType;
    m_type = other.m_type;
    m_state = other.m_state;
}

NKTree::~NKTree() {}

void NKTree::Layout(nk_context* ctx)
{
    if (nk_tree_push_id(ctx, m_treeType, m_cContent, m_state, reinterpret_cast<intptr_t>(this)))
    {
        for (auto child = m_pChildList.begin(); child != m_pChildList.end(); ++child)
        {
            (*child)->Update(ctx);
        }
        nk_tree_pop(ctx);
    }
}

void NKTree::EditInfo()
{
    EditLabel(m_ctx, m_pManager);

    if (nk_tree_push(m_ctx, NK_TREE_NODE, "Create UI", NK_MINIMIZED)) {
        if (nk_button_label(m_ctx, "Tree"))
        {
            CreateUI("NKTree");
        }
        nk_tree_pop(m_ctx);
    }
}

void NKTree::EditStyle()
{
    EditComponentStyle(m_ctx, m_pManager);
}

void NKTree::SetState(nk_collapse_states state)
{
    m_state = state;
}

nk_collapse_states NKTree::GetState() const
{
    return m_state;
}
