#include "pch.h"
#include "NKStyleHeader.h"
#include "NuklearUI.h"

NKStyleHeader::NKStyleHeader()
{
	m_pComponent = nullptr;
}

NKStyleHeader::NKStyleHeader(nk_context* ctx, nk_style* style)
{
	m_pComponent = new ComponentHeader(&style->window.header, &ctx->style.window.header);
}

NKStyleHeader::NKStyleHeader(const NKStyleHeader& other, nk_context* ctx, nk_style* style)
{
	m_pComponent = new ComponentHeader(&style->window.header, &ctx->style.window.header);
	*m_pComponent = *other.m_pComponent;
}

NKStyleHeader::~NKStyleHeader()
{
	delete m_pComponent;
	m_pComponent = nullptr;
}

void NKStyleHeader::UpdateComponent(nk_context* ctx, NuklearUI* pManager)
{
	m_pComponent->UpdateComponent(ctx, pManager);
}

void NKStyleHeader::EditComponentStyle(nk_context* ctx, NuklearUI* pManager)
{
	if (nk_tree_push(ctx, NK_TREE_TAB, "Header", NK_MINIMIZED)) {
		m_pComponent->CustomComponentsEditor(ctx, pManager);
		nk_tree_pop(ctx);
	}
}
