#include "pch.h"
#include "NKStyleTab.h"
#include "NuklearUI.h"

NKStyleTab::NKStyleTab()
{
	m_pComponent = nullptr;
}

NKStyleTab::NKStyleTab(nk_context* ctx, nk_style* style)
{
	m_pComponent = new ComponentTab(&style->tab, &ctx->style.tab);
}

NKStyleTab::NKStyleTab(const NKStyleTab& other, nk_context* ctx, nk_style* style)
{
	m_pComponent = new ComponentTab(&style->tab, &ctx->style.tab);
	*m_pComponent = *other.m_pComponent;
}

NKStyleTab::~NKStyleTab()
{
	delete m_pComponent;
	m_pComponent = nullptr;
}

void NKStyleTab::UpdateComponent(nk_context* ctx, NuklearUI* pManager)
{
	m_pComponent->UpdateComponent(ctx, pManager);
}

void NKStyleTab::EditComponentStyle(nk_context* ctx, NuklearUI* pManager)
{
	if (nk_tree_push(ctx, NK_TREE_TAB, "Tab", NK_MINIMIZED)) {
		m_pComponent->CustomComponentsEditor(ctx, pManager);
		nk_tree_pop(ctx);
	}
}
