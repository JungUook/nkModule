#include "pch.h"
#include "NKStyleMenuButton.h"
#include "NuklearUI.h"

NKStyleMenuButton::NKStyleMenuButton()
{
	m_pComponent = nullptr;
}

NKStyleMenuButton::NKStyleMenuButton(nk_context* ctx, nk_style* style)
{
	m_pComponent = new ComponentButton(&style->button, &ctx->style.menu_button);
}

NKStyleMenuButton::NKStyleMenuButton(const NKStyleMenuButton& other, nk_context* ctx, nk_style* style)
{
	m_pComponent = new ComponentButton(&style->button, &ctx->style.menu_button);
	*m_pComponent = *other.m_pComponent;
}

NKStyleMenuButton::~NKStyleMenuButton()
{
	delete m_pComponent;
	m_pComponent = nullptr;
}

void NKStyleMenuButton::UpdateComponent(nk_context* ctx, NuklearUI* pManager)
{
	m_pComponent->UpdateComponent(ctx, pManager);
}

void NKStyleMenuButton::EditComponentStyle(nk_context* ctx, NuklearUI* pManager)
{
	if (nk_tree_push(ctx, NK_TREE_TAB, "MenuButton", NK_MINIMIZED)) {
		m_pComponent->CustomComponentsEditor(ctx, pManager);
		nk_tree_pop(ctx);
	}
}
