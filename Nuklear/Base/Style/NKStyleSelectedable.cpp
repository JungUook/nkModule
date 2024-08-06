#include "pch.h"
#include "NKStyleSelectedable.h"
#include "NuklearUI.h"

NKStyleSelectedable::NKStyleSelectedable()
{
	m_pComponent = nullptr;
}

NKStyleSelectedable::NKStyleSelectedable(nk_context* ctx, nk_style* style)
{
	m_pComponent = new ComponentSelectable(&style->selectable, &ctx->style.selectable);
}

NKStyleSelectedable::NKStyleSelectedable(const NKStyleSelectedable& other, nk_context* ctx, nk_style* style)
{
	m_pComponent = new ComponentSelectable(&style->selectable, &ctx->style.selectable);
	*m_pComponent = *other.m_pComponent;
}

NKStyleSelectedable::~NKStyleSelectedable()
{
	delete m_pComponent;
	m_pComponent = nullptr;
}

void NKStyleSelectedable::UpdateComponent(nk_context* ctx, NuklearUI* pManager)
{
	m_pComponent->UpdateComponent(ctx, pManager);
}

void NKStyleSelectedable::EditComponentStyle(nk_context* ctx, NuklearUI* pManager)
{
	if (nk_tree_push(ctx, NK_TREE_TAB, "Selectable", NK_MINIMIZED)) {
		m_pComponent->CustomComponentsEditor(ctx, pManager);
		nk_tree_pop(ctx);
	}
}
