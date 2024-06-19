#include "pch.h"
#include "NKStyleScrollbarH.h"
#include "NuklearUI.h"

NKStyleScrollbarH::NKStyleScrollbarH()
{
	m_pComponent = nullptr;
}

NKStyleScrollbarH::NKStyleScrollbarH(nk_context* ctx, nk_style* style)
{
	m_pComponent = new ComponentScrollbar(&style->scrollh, &ctx->style.scrollh);
}

NKStyleScrollbarH::NKStyleScrollbarH(const NKStyleScrollbarH& other)
{
	m_pComponent = new ComponentScrollbar(*other.m_pComponent);
}

NKStyleScrollbarH::~NKStyleScrollbarH()
{
	delete m_pComponent;
	m_pComponent = nullptr;
}

void NKStyleScrollbarH::EditComponentStyle(nk_context* ctx, NuklearUI* pManager)
{
	if (nk_tree_push(ctx, NK_TREE_TAB, "Scrollh", NK_MINIMIZED)) {
		m_pComponent->CustomComponentsEditor(ctx, pManager);
		nk_tree_pop(ctx);
	}
}
