#include "pch.h"
#include "NKStyleContextualButton.h"
#include "NuklearUI.h"

NKStyleContextualButton::NKStyleContextualButton(nk_context* ctx, nk_style* style)
{
	m_pComponent = new ComponentButton(&style->contextual_button, &ctx->style.contextual_button);
}

NKStyleContextualButton::NKStyleContextualButton(const NKStyleContextualButton& other)
{
	m_pComponent = new ComponentButton(*other.m_pComponent);
}

NKStyleContextualButton::~NKStyleContextualButton()
{
	delete m_pComponent;
	m_pComponent = nullptr;
}

void NKStyleContextualButton::EditComponentStyle(nk_context* ctx, NuklearUI* pManager)
{
	if (nk_tree_push(ctx, NK_TREE_TAB, "ContextualButton", NK_MINIMIZED)) {
		m_pComponent->CustomComponentsEditor(ctx, pManager);
		nk_tree_pop(ctx);
	}
}
