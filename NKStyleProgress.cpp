#include "pch.h"
#include "NKStyleProgress.h"
#include "NuklearUI.h"

NKStyleProgress::NKStyleProgress(nk_context* ctx, nk_style* style)
{
	m_pComponent = new ComponentProgress(&style->progress, &ctx->style.progress);
}

NKStyleProgress::NKStyleProgress(const NKStyleProgress& other)
{
	m_pComponent = new ComponentProgress(*other.m_pComponent);
}

NKStyleProgress::~NKStyleProgress()
{
	delete m_pComponent;
	m_pComponent = nullptr;
}

void NKStyleProgress::EditComponentStyle(nk_context* ctx, NuklearUI* pManager)
{
	if (nk_tree_push(ctx, NK_TREE_TAB, "Progress", NK_MINIMIZED)) {
		m_pComponent->CustomComponentsEditor(ctx, pManager);
		nk_tree_pop(ctx);
	}
}
