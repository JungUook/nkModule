#include "pch.h"
#include "NKStyleWindow.h"
#include "NuklearUI.h"

NKStyleWindow::NKStyleWindow(nk_context* ctx, nk_style* style)
{
	m_pFixedBackground	= new NKStyleItem(&style->window.fixed_background, &ctx->style.window.fixed_background);
	m_pScaler			= new NKStyleItem(&style->window.scaler, &ctx->style.window.scaler);

	m_pBackground		= &style->window.background;
	border				= &style->window.border;
	border_color		= &style->window.border_color;
	rounding			= &style->window.rounding;
	spacing				= &style->window.spacing;
	scrollbar_size		= &style->window.scrollbar_size;
	min_size			= &style->window.min_size;
	padding				= &style->window.padding;
}

NKStyleWindow::~NKStyleWindow()
{
	delete m_pFixedBackground;
	delete m_pScaler;

	m_pFixedBackground = nullptr;
	m_pScaler = nullptr;

	m_pBackground = nullptr;
}

void NKStyleWindow::EditComponentStyle(nk_context* ctx, NuklearUI* pManager)
{
	if (nk_tree_push(ctx, NK_TREE_NODE, "Fixed Background", NK_MINIMIZED)) {
		m_pFixedBackground->ItemEditor(ctx, pManager);
		nk_tree_pop(ctx);
	}
	if (nk_tree_push(ctx, NK_TREE_NODE, "background", NK_MINIMIZED)) {
		ColorPicker(ctx, *m_pBackground);
		nk_tree_pop(ctx);
	}
	if (nk_tree_push(ctx, NK_TREE_NODE, "Scaler", NK_MINIMIZED)) {
		m_pScaler->ItemEditor(ctx, pManager);
		nk_tree_pop(ctx);
	}

	if (nk_tree_push(ctx, NK_TREE_NODE, "Properties", NK_MINIMIZED)) {
		nk_layout_row_dynamic(ctx, 22, 1);
		nk_property_float(ctx, "#border:", 0.f, border, 100.f, 1.f, 1.f);
		ColorPicker(ctx, *border_color);

		nk_property_float(ctx, "#rounding:", 0.f, rounding, 100.f, 1.f, 1.f);
		PropertyVector2(ctx, "padding", *padding, 0.f, 1000.f, 1.f, 1.f);
		PropertyVector2(ctx, "spacing", *spacing, 0.f, 1000.f, 1.f, 1.f);
		PropertyVector2(ctx, "scrollbar_size", *scrollbar_size, 0.f, 1000.f, 1.f, 1.f);
		PropertyVector2(ctx, "min_size", *min_size, 0.f, 1000.f, 1.f, 1.f);
		nk_tree_pop(ctx);
	}
}
