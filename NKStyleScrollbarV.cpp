#include "pch.h"
#include "NKStyleScrollbarV.h"
#include "NuklearUI.h"

NKStyleScrollbarV::NKStyleScrollbarV()
{
	m_pComponent = nullptr;
}

NKStyleScrollbarV::NKStyleScrollbarV(nk_context* ctx, nk_style* style)
{
	m_pComponent = new ComponentScrollbar(&style->scrollv, &ctx->style.scrollv);
}

NKStyleScrollbarV::NKStyleScrollbarV(const NKStyleScrollbarV& other)
{
	m_pComponent = new ComponentScrollbar(*other.m_pComponent);
}

NKStyleScrollbarV::~NKStyleScrollbarV()
{
	delete m_pComponent;
	m_pComponent = nullptr;
}

void NKStyleScrollbarV::EditComponentStyle(nk_context* ctx, NuklearUI* pManager)
{
	if (nk_tree_push(ctx, NK_TREE_TAB, "Scrollv", NK_MINIMIZED)) {
		m_pComponent->CustomComponentsEditor(ctx, pManager);
		nk_tree_pop(ctx);
	}
}
