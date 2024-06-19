#include "pch.h"
#include "NKStyleEdit.h"
#include "NuklearUI.h"

NKStyleEdit::NKStyleEdit()
{
	m_pComponent = nullptr;
}

NKStyleEdit::NKStyleEdit(nk_context* ctx, nk_style* style)
{
	m_pComponent = new ComponentEdit(&style->edit, &ctx->style.edit);
}

NKStyleEdit::NKStyleEdit(const NKStyleEdit& other)
{
	m_pComponent = new ComponentEdit(*other.m_pComponent);
}

NKStyleEdit::~NKStyleEdit()
{
	delete m_pComponent;
	m_pComponent = nullptr;
}

void NKStyleEdit::EditComponentStyle(nk_context* ctx, NuklearUI* pManager)
{
	if (nk_tree_push(ctx, NK_TREE_TAB, "Edit", NK_MINIMIZED)) {
		m_pComponent->CustomComponentsEditor(ctx, pManager);
		nk_tree_pop(ctx);
	}
}
