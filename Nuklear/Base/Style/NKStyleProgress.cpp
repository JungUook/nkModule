#include "pch.h"
#include "NKStyleProgress.h"
#include "NuklearUI.h"

NKStyleProgress::NKStyleProgress()
{
	m_pComponent = nullptr;
}

NKStyleProgress::NKStyleProgress(nk_context* ctx, nk_style* style)
{
	m_pComponent = new ComponentProgress(&style->progress, &ctx->style.progress);
}

NKStyleProgress::NKStyleProgress(const NKStyleProgress& other, nk_context* ctx, nk_style* style)
{
	m_pComponent = new ComponentProgress(&style->progress, &ctx->style.progress);
	*m_pComponent = *other.m_pComponent;
}

NKStyleProgress::~NKStyleProgress()
{
	delete m_pComponent;
	m_pComponent = nullptr;
}

void NKStyleProgress::UpdateComponent(nk_context* ctx, NuklearUI* pManager)
{
	m_pComponent->UpdateComponent(ctx, pManager);
}

void NKStyleProgress::EditComponentStyle(nk_context* ctx, NuklearUI* pManager)
{
	if (nk_tree_push(ctx, NK_TREE_TAB, "Progress", NK_MINIMIZED)) {
		m_pComponent->CustomComponentsEditor(ctx, pManager);
		nk_tree_pop(ctx);
	}
}
