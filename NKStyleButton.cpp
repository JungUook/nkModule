#include "pch.h"
#include "NKStyleButton.h"
#include "NuklearUI.h"

NKStyleButton::NKStyleButton()
{
	m_pComponent = nullptr;
}

NKStyleButton::NKStyleButton(nk_context* ctx, nk_style* style)
{
	m_pComponent = new ComponentButton(&style->button, &ctx->style.button);
}

NKStyleButton::NKStyleButton(const NKStyleButton& other, nk_context* ctx, nk_style* style)
{
	m_pComponent = new ComponentButton(&style->button, &ctx->style.button);
	*m_pComponent = *other.m_pComponent;
}

NKStyleButton::~NKStyleButton()
{
	delete m_pComponent;
	m_pComponent = nullptr;
}

void NKStyleButton::UpdateComponent(nk_context* ctx, NuklearUI* pManager)
{
	m_pComponent->UpdateComponent(ctx, pManager);
}

void NKStyleButton::EditComponentStyle(nk_context* ctx, NuklearUI* pManager)
{
	if (nk_tree_push(ctx, NK_TREE_TAB, "DefaultButton", NK_MINIMIZED)) {
		m_pComponent->CustomComponentsEditor(ctx, pManager);
		nk_tree_pop(ctx);
	}
}
