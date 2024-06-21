#include "pch.h"
#include "NKStyleChart.h"
#include "NuklearUI.h"

NKStyleChart::NKStyleChart()
{
	m_pBackground   = nullptr;

	border_color    = nullptr;
	selected_color  = nullptr;
	color		    = nullptr;
	border		    = nullptr;
	rounding	    = nullptr;
	padding		    = nullptr;
	color_factor    = nullptr;
	disabled_factor = nullptr;
	show_markers    = nullptr;

}

NKStyleChart::NKStyleChart(nk_context* ctx, nk_style* style)
{
	m_pBackground = new NKStyleItem(&style->chart.background, &ctx->style.chart.background);

	border_color	= &style->chart.border_color;
	selected_color	= &style->chart.selected_color;
	color			= &style->chart.color;
	border			= &style->chart.border;
	rounding		= &style->chart.rounding;
	padding			= &style->chart.padding;
	color_factor	= &style->chart.color_factor;
	disabled_factor	= &style->chart.disabled_factor;
	show_markers	= &style->chart.show_markers;
}

NKStyleChart::NKStyleChart(const NKStyleChart& other, nk_context* ctx, nk_style* style)
{
	m_pBackground = new NKStyleItem(&style->chart.background, &ctx->style.chart.background);
	*m_pBackground = *other.m_pBackground;

	border_color = &style->chart.border_color;
	selected_color = &style->chart.selected_color;
	color = &style->chart.color;
	border = &style->chart.border;
	rounding = &style->chart.rounding;
	padding = &style->chart.padding;
	color_factor = &style->chart.color_factor;
	disabled_factor = &style->chart.disabled_factor;
	show_markers = &style->chart.show_markers;

	*border_color = *other.border_color;
	*selected_color = *other.selected_color;
	*color = *other.color;
	*border = *other.border;
	*rounding = *other.rounding;
	*padding = *other.padding;
	*color_factor = *other.color_factor;
	*disabled_factor = *other.disabled_factor;
	*show_markers = *other.show_markers;
}

NKStyleChart::~NKStyleChart()
{
	delete m_pBackground;

	m_pBackground	= nullptr;

	border_color    = nullptr;
	selected_color  = nullptr;
	color		    = nullptr;
	border		    = nullptr;
	rounding	    = nullptr;
	padding		    = nullptr;
	color_factor    = nullptr;
	disabled_factor = nullptr;
	show_markers    = nullptr;
}

void NKStyleChart::UpdateComponent(nk_context* ctx, NuklearUI* pManager)
{
	m_pBackground->UpdateComponent(ctx, pManager);
}

void NKStyleChart::EditComponentStyle(nk_context* ctx, NuklearUI* pManager)
{
	if (nk_tree_push(ctx, NK_TREE_TAB, "Chart", NK_MINIMIZED)) {
		if (nk_tree_push(ctx, NK_TREE_NODE, "Background", NK_MINIMIZED)) {
			m_pBackground->ItemEditor(ctx, pManager);
			nk_tree_pop(ctx);
		}
		if (nk_tree_push(ctx, NK_TREE_NODE, "Border color", NK_MINIMIZED)) {
			ColorPicker(ctx, *border_color);
			nk_tree_pop(ctx);
		}
		if (nk_tree_push(ctx, NK_TREE_NODE, "Selected color", NK_MINIMIZED)) {
			ColorPicker(ctx, *selected_color);
			nk_tree_pop(ctx);
		}
		if (nk_tree_push(ctx, NK_TREE_NODE, "Color", NK_MINIMIZED)) {
			ColorPicker(ctx, *color);
			nk_tree_pop(ctx);
		}

		if (nk_tree_push(ctx, NK_TREE_NODE, "Properties", NK_MINIMIZED)) {
			nk_layout_row_dynamic(ctx, 22, 1);
			nk_property_float(ctx, "#border:", 0.f, border, 100.f, 1.f, 1.f);
			nk_property_float(ctx, "#rounding:", 0.f, rounding, 100.f, 1.f, 1.f);

			PropertyVector2(ctx, "padding", *padding, 0.f, 1000.f, 1.f, 1.f);

			nk_property_float(ctx, "#color factor:", 0.f, color_factor, 100.f, 1.f, 1.f);
			nk_property_float(ctx, "#disabled factor:", 0.f, disabled_factor, 100.f, 1.f, 1.f);

			nk_checkbox_label(ctx, "show markers", show_markers);
			nk_tree_pop(ctx);
		}
		nk_tree_pop(ctx);
	}
}
