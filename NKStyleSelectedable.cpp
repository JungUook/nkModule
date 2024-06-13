#include "pch.h"
#include "NKStyleSelectedable.h"
#include "NuklearUI.h"

NKStyleSelectedable::NKStyleSelectedable(nk_context* ctx, nk_style* style)
{
	m_pComponent = new ComponentSelectable(&style->selectable, &ctx->style.selectable);
}

NKStyleSelectedable::~NKStyleSelectedable()
{
	delete m_pComponent;
	m_pComponent = nullptr;
}

void NKStyleSelectedable::EditComponentStyle(nk_context* ctx, NuklearUI* pManager)
{
	if (nk_tree_push(ctx, NK_TREE_NODE, "Selectable", NK_MINIMIZED)) {
		m_pComponent->CustomComponentsEditor(ctx, pManager);
		nk_tree_pop(ctx);
	}
}
