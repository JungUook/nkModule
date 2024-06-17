#include "pch.h"
#include "NKStyleHeader.h"
#include "NuklearUI.h"

NKStyleHeader::NKStyleHeader(nk_context* ctx, nk_style* style)
{
	m_pComponent = new ComponentHeader(&style->window.header, &ctx->style.window.header);
}

NKStyleHeader::NKStyleHeader(const NKStyleHeader& other)
{
	m_pComponent = new ComponentHeader(*other.m_pComponent);
}

NKStyleHeader::~NKStyleHeader()
{
	delete m_pComponent;
	m_pComponent = nullptr;
}

void NKStyleHeader::EditComponentStyle(nk_context* ctx, NuklearUI* pManager)
{
	if (nk_tree_push(ctx, NK_TREE_TAB, "Header", NK_MINIMIZED)) {
		m_pComponent->CustomComponentsEditor(ctx, pManager);
		nk_tree_pop(ctx);
	}
}
