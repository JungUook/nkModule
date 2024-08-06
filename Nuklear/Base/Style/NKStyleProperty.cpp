#include "pch.h"
#include "NKStyleProperty.h"
#include "NuklearUI.h"

NKStyleProperty::NKStyleProperty()
{
	m_pComponent = nullptr;
}

NKStyleProperty::NKStyleProperty(nk_context* ctx, nk_style* style)
{
	m_pComponent = new ComponentProperty(&style->property, &ctx->style.property);
}

NKStyleProperty::NKStyleProperty(const NKStyleProperty& other, nk_context* ctx, nk_style* style)
{
	m_pComponent = new ComponentProperty(&style->property, &ctx->style.property);
	*m_pComponent = *other.m_pComponent;
}

NKStyleProperty::~NKStyleProperty()
{
	delete m_pComponent;
	m_pComponent = nullptr;
}

void NKStyleProperty::UpdateComponent(nk_context* ctx, NuklearUI* pManager)
{
	m_pComponent->UpdateComponent(ctx, pManager);
}

void NKStyleProperty::EditComponentStyle(nk_context* ctx, NuklearUI* pManager)
{
	if (nk_tree_push(ctx, NK_TREE_TAB, "Property", NK_MINIMIZED)) {
		m_pComponent->CustomComponentsEditor(ctx, pManager);
		nk_tree_pop(ctx);
	}
}
