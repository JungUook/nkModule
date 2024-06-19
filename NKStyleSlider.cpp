#include "pch.h"
#include "NKStyleSlider.h"
#include "NuklearUI.h"

NKStyleSlider::NKStyleSlider()
{
	m_pComponent = nullptr;
}

NKStyleSlider::NKStyleSlider(nk_context* ctx, nk_style* style)
{
	m_pComponent = new ComponentSlider(&style->slider, &ctx->style.slider);
}

NKStyleSlider::NKStyleSlider(const NKStyleSlider& other)
{
	m_pComponent = new ComponentSlider(*other.m_pComponent);
}

NKStyleSlider::~NKStyleSlider()
{
	delete m_pComponent;
	m_pComponent = nullptr;
}

void NKStyleSlider::EditComponentStyle(nk_context* ctx, NuklearUI* pManager)
{
	if (nk_tree_push(ctx, NK_TREE_TAB, "Slider", NK_MINIMIZED)) {
		m_pComponent->CustomComponentsEditor(ctx, pManager);
		nk_tree_pop(ctx);
	}
}
