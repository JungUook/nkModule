#include "pch.h"
#include "NKStyleCombo.h"
#include "NuklearUI.h"

NKStyleCombo::NKStyleCombo()
{
	m_pComponent = nullptr;
}

NKStyleCombo::NKStyleCombo(nk_context* ctx, nk_style* style)
{
	m_pComponent = new ComponentCombo(&style->combo, &ctx->style.combo);
}

NKStyleCombo::NKStyleCombo(const NKStyleCombo& other)
{
	m_pComponent = new ComponentCombo(*other.m_pComponent);
}

NKStyleCombo::~NKStyleCombo()
{
	delete m_pComponent;
	m_pComponent = nullptr;
}

void NKStyleCombo::EditComponentStyle(nk_context* ctx, NuklearUI* pManager)
{
	if (nk_tree_push(ctx, NK_TREE_TAB, "Combo", NK_MINIMIZED)) {
		m_pComponent->CustomComponentsEditor(ctx, pManager);
		nk_tree_pop(ctx);
	}
}
