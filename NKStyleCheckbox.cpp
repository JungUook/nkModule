#include "pch.h"
#include "NKStyleCheckbox.h"
#include "NuklearUI.h"

NKStyleCheckbox::NKStyleCheckbox()
{
	m_pComponent = nullptr;
}

NKStyleCheckbox::NKStyleCheckbox(nk_context* ctx, nk_style* style)
{
	m_pComponent = new ComponentToggle(&style->checkbox, &ctx->style.checkbox);
}

NKStyleCheckbox::NKStyleCheckbox(const NKStyleCheckbox& other)
{
	m_pComponent = new ComponentToggle(*other.m_pComponent);
}

NKStyleCheckbox::~NKStyleCheckbox()
{
	delete m_pComponent;
	m_pComponent = nullptr;
}

void NKStyleCheckbox::EditComponentStyle(nk_context* ctx, NuklearUI* pManager)
{
	if (nk_tree_push(ctx, NK_TREE_TAB, "Checkbox", NK_MINIMIZED)) {
		m_pComponent->CustomComponentsEditor(ctx, pManager);
		nk_tree_pop(ctx);
	}
}
