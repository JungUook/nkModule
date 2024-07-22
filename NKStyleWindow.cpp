#include "pch.h"
#include "NKStyleWindow.h"
#include "NuklearUI.h"

NKStyleWindow::NKStyleWindow()
{
	m_pFixedBackground		= nullptr;
	m_pScaler				= nullptr;
	m_pBackground			= nullptr;
	border					= nullptr;
	border_color			= nullptr;
	rounding				= nullptr;
	spacing					= nullptr;
	scrollbar_size			= nullptr;
	min_size				= nullptr;
	padding					= nullptr;

	group_border			= nullptr;
	group_border_color		= nullptr;
	group_padding			= nullptr;

	tooltip_border			= nullptr;
	tooltip_border_color	= nullptr;
	tooltip_padding			= nullptr;

	popup_border			= nullptr;
	popup_border_color		= nullptr;	
	popup_padding			= nullptr;

}

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


	group_border		 = &style->window.group_border;
	group_border_color	 = &style->window.group_border_color;
	group_padding		 = &style->window.group_padding;

	tooltip_border		 = &style->window.tooltip_border;
	tooltip_border_color = &style->window.tooltip_border_color;
	tooltip_padding		 = &style->window.tooltip_padding;

	popup_border		 = &style->window.popup_border;
	popup_border_color	 = &style->window.popup_border_color;
	popup_padding		 = &style->window.popup_padding;
}

NKStyleWindow::NKStyleWindow(const NKStyleWindow& other, nk_context* ctx, nk_style* style)
{
	m_pFixedBackground = new NKStyleItem(&style->window.fixed_background, &ctx->style.window.fixed_background);
	m_pScaler = new NKStyleItem(&style->window.scaler, &ctx->style.window.scaler);
	*m_pFixedBackground = *other.m_pFixedBackground;
	*m_pScaler = *other.m_pScaler;

	m_pBackground = &style->window.background;
	border = &style->window.border;
	border_color = &style->window.border_color;
	rounding = &style->window.rounding;
	spacing = &style->window.spacing;
	scrollbar_size = &style->window.scrollbar_size;
	min_size = &style->window.min_size;
	padding = &style->window.padding;

	group_border = &style->window.group_border;
	group_border_color = &style->window.group_border_color;
	group_padding = &style->window.group_padding;
	tooltip_border = &style->window.tooltip_border;
	tooltip_border_color = &style->window.tooltip_border_color;
	tooltip_padding = &style->window.tooltip_padding;
	popup_border = &style->window.popup_border;
	popup_border_color = &style->window.popup_border_color;
	popup_padding = &style->window.popup_padding;

	*m_pBackground = *other.m_pBackground;
	*border = *other.border;
	*border_color = *other.border_color;
	*rounding = *other.rounding;
	*spacing = *other.spacing;
	*scrollbar_size = *other.scrollbar_size;
	*min_size = *other.min_size;
	*padding = *other.padding;

	*group_border = *other.group_border;
	*group_border_color = *other.group_border_color;
	*group_padding = *other.group_padding;
	*tooltip_border = *other.tooltip_border;
	*tooltip_border_color = *other.tooltip_border_color;
	*tooltip_padding = *other.tooltip_padding;
	*popup_border = *other.popup_border;
	*popup_border_color = *other.popup_border_color;
	*popup_padding = *other.popup_padding;
}

NKStyleWindow::~NKStyleWindow()
{
	delete m_pFixedBackground;
	delete m_pScaler;

	m_pFixedBackground = nullptr;
	m_pScaler = nullptr;

	m_pBackground = nullptr;
}

void NKStyleWindow::UpdateComponent(nk_context* ctx, NuklearUI* pManager)
{
	m_pFixedBackground->UpdateComponent(ctx, pManager);
	m_pScaler->UpdateComponent(ctx, pManager);
}

void NKStyleWindow::EditComponentStyle(nk_context* ctx, NuklearUI* pManager)
{
	if (nk_tree_push(ctx, NK_TREE_TAB, "Window", NK_MINIMIZED)) {
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
			nk_property_float(ctx, "#border:", 0.f, border, 100.f, 0.01f, 0.01f);
			ColorPicker(ctx, *border_color);

			nk_property_float(ctx, "#rounding:", 0.f, rounding, 100.f, 0.01f, 0.01f);
			PropertyVector2(ctx, "padding", *padding, 0.f, 1000.f, 1.f, 1.f);
			PropertyVector2(ctx, "spacing", *spacing, 0.f, 1000.f, 1.f, 1.f);
			PropertyVector2(ctx, "scrollbar_size", *scrollbar_size, 0.f, 1000.f, 1.f, 1.f);
			PropertyVector2(ctx, "min_size", *min_size, 0.f, 1000.f, 1.f, 1.f);
			nk_tree_pop(ctx);
		}
		nk_tree_pop(ctx);
	}
	if (nk_tree_push(ctx, NK_TREE_TAB, "Group", NK_MINIMIZED)) {
		nk_layout_row_dynamic(ctx, 22, 1);
		nk_property_float(ctx, "#border:", 0.f, group_border, 100.f, 0.01f, 0.01f);
		ColorPicker(ctx, *group_border_color);
		PropertyVector2(ctx, "padding", *group_padding, 0.f, 1000.f, 1.f, 1.f);
		nk_tree_pop(ctx);
	}
	if (nk_tree_push(ctx, NK_TREE_TAB, "Tooltip", NK_MINIMIZED)) {
		nk_layout_row_dynamic(ctx, 22, 1);
		nk_property_float(ctx, "#border:", 0.f, tooltip_border, 100.f, 0.01f, 0.01f);
		ColorPicker(ctx, *tooltip_border_color);
		PropertyVector2(ctx, "padding", *tooltip_padding, 0.f, 1000.f, 1.f, 1.f);
		nk_tree_pop(ctx);
	}
	if (nk_tree_push(ctx, NK_TREE_TAB, "Popup", NK_MINIMIZED)) {
		nk_layout_row_dynamic(ctx, 22, 1);
		nk_property_float(ctx, "#border:", 0.f, popup_border, 100.f, 0.01f, 0.01f);
		ColorPicker(ctx, *popup_border_color);
		PropertyVector2(ctx, "padding", *popup_padding, 0.f, 1000.f, 1.f, 1.f);
		nk_tree_pop(ctx);
	}
}
