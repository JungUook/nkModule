#include "pch.h"
#include "NKStyleMenuButton.h"
#include "NuklearUI.h"

NKStyleMenuButton::NKStyleMenuButton(nk_context* ctx, nk_style* style)
{
	m_pComponent = new ComponentButton(&style->button, &ctx->style.menu_button);
}

NKStyleMenuButton::~NKStyleMenuButton()
{
	delete m_pComponent;
	m_pComponent = nullptr;
}

void NKStyleMenuButton::EditComponentStyle(nk_context* ctx, NuklearUI* pManager)
{
	if (nk_tree_push(ctx, NK_TREE_NODE, "MenuButton", NK_MINIMIZED)) {
		m_pComponent->CustomComponentsEditor(ctx, pManager);
		nk_tree_pop(ctx);
	}
}
