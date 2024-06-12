#include "pch.h"
#include "NKStyle.h"
#include "NuklearUI.h"

NKStyle::NKStyle()
{
	m_font = nullptr;
	m_pParentStyle = nullptr;
	m_followParentStyle = nk_true;
}

NKStyle::NKStyle(const NKStyle& other)
{
	m_style = other.m_style;
	m_font = other.m_font;
}

NKStyle::~NKStyle()
{
}

void NKStyle::InitializeStyle(nk_context* ctx, NuklearUI* pManager)
{
	m_font = pManager->GetFont();
	m_style = ctx->style;
	m_pParentStyle = nullptr;
}

void NKStyle::InitializeStyle(nk_font* font, nk_style& parentStyle, nk_style* parent_of_parentStyle)
{
	m_font = font;
	m_style = parentStyle;
	m_pParentStyle = parent_of_parentStyle != nullptr ? parent_of_parentStyle : &parentStyle;
}

void NKStyle::InitializeStyle(nk_context* ctx)
{
	//header
	m_sHeader.Init(
		&m_style.window.header.normal,
		&ctx->style.window.header.normal,
		&m_style.window.header.hover,
		&ctx->style.window.header.hover,
		&m_style.window.header.active,
		&ctx->style.window.header.active
	);
	m_sButton_close.Init(
		&m_style.window.header.close_button,
		&ctx->style.window.header.close_button
	);
	m_sButton_minimize.Init(
		&m_style.window.header.minimize_button,
		&ctx->style.window.header.minimize_button
	);

	//window
	m_sBackground.Init(&m_style.window.fixed_background, &ctx->style.window.fixed_background);
	m_sScaler.Init(&m_style.window.scaler, &ctx->style.window.scaler);

	//component
	m_sButton_default.Init(&m_style.button, &ctx->style.button);
	m_sButton_contextual.Init(&m_style.contextual_button, &ctx->style.contextual_button);
	m_sButton_menu.Init(&m_style.menu_button, &ctx->style.menu_button);
	m_sToggle_option.Init(&m_style.option, &ctx->style.option);
	m_sToggle_checkbox.Init(&m_style.checkbox, &ctx->style.checkbox);
	m_sSelectable.Init(&m_style.selectable, &ctx->style.selectable);
	m_sSlider.Init(&m_style.slider, &ctx->style.slider);
	m_sProgress.Init(&m_style.progress, &ctx->style.progress);
	m_sProperty.Init(&m_style.property, &ctx->style.property);
	m_sEdit.Init(&m_style.edit, &ctx->style.edit);

	m_sChart.Init(&m_style.chart.background, &ctx->style.chart.background);

	m_sScrollbarh.Init(&m_style.scrollh, &ctx->style.scrollh);
	m_sScrollbarv.Init(&m_style.scrollv, &ctx->style.scrollv);
	m_sTab.Init(&m_style.tab, &ctx->style.tab);
	m_sCombo.Init(&m_style.combo, &ctx->style.combo);
}

void NKStyle::StyleUpdateStart(nk_context* ctx, nk_style& original, NKStyle* pParent)
{
	original = ctx->style;
	ctx->style = pParent != nullptr && m_followParentStyle ? *m_pParentStyle : m_style;
}

void NKStyle::StyleUpdateEnd(nk_context* ctx, nk_style& original)
{
	ctx->style = original;
}

void NKStyle::SetStyle(nk_style* style)
{
	CHECK_PTR(style);
	m_style = *style;
}

void NKStyle::Setfont(nk_font* font)
{
	CHECK_PTR(font);
	m_font = font;
}

void NKStyle::SetBackground(NuklearUI* pManager, int SID)
{
	struct nk_image* img = pManager->SearchImage(SID);

	if (img)
	{
		m_style.window.fixed_background = nk_style_item_image(*img);
	}
}


void NKStyle::FollowParentStyle(nk_context* ctx, NKStyle* pParent)
{
	nk_layout_row_dynamic(ctx, 22, 1);
	nk_checkbox_label(ctx, "follow_parent_style", &m_followParentStyle);

	if (m_followParentStyle) {
		if (pParent != nullptr) {

			if (pParent->m_pParentStyle != nullptr) {
				m_pParentStyle = pParent->m_pParentStyle;
			}
			else {
				m_pParentStyle = &pParent->m_style;
			}

		}
		else {
			m_pParentStyle = nullptr;
		}
	}
	else {
		m_pParentStyle = nullptr;
	}
}

void NKStyle::HeaderEditor(nk_context* ctx, NuklearUI* pManager)
{
	if (nk_tree_push(ctx, NK_TREE_TAB, "header", NK_MINIMIZED)) {
		if (nk_tree_push(ctx, NK_TREE_NODE, "header_bg", NK_MINIMIZED)) {
			if (nk_tree_push(ctx, NK_TREE_NODE, "normal", NK_MINIMIZED)) {
				ItemEditor(ctx, pManager, m_sHeader.normal);
				nk_tree_pop(ctx);
			}
			if (nk_tree_push(ctx, NK_TREE_NODE, "hover", NK_MINIMIZED)) {
				ItemEditor(ctx, pManager, m_sHeader.hover);
				nk_tree_pop(ctx);
			}
			if (nk_tree_push(ctx, NK_TREE_NODE, "active", NK_MINIMIZED)) {
				ItemEditor(ctx, pManager, m_sHeader.active);
				nk_tree_pop(ctx);
			}
			nk_tree_pop(ctx);
		}

		HeaderCloseButtonEditor(ctx, pManager);
		HeaderMinimizeButtonEditor(ctx, pManager);

		if (nk_tree_push(ctx, NK_TREE_NODE, "label", NK_MINIMIZED)) {
			if (nk_tree_push(ctx, NK_TREE_NODE, "normal", NK_MINIMIZED)) {
				ColorPicker(ctx, m_style.window.header.label_normal);
				nk_tree_pop(ctx);
			}
			if (nk_tree_push(ctx, NK_TREE_NODE, "hover", NK_MINIMIZED)) {
				ColorPicker(ctx, m_style.window.header.label_hover);
				nk_tree_pop(ctx);
			}
			if (nk_tree_push(ctx, NK_TREE_NODE, "active", NK_MINIMIZED)) {
				ColorPicker(ctx, m_style.window.header.label_active);
				nk_tree_pop(ctx);
			}
			nk_tree_pop(ctx);
		}
		if (nk_tree_push(ctx, NK_TREE_NODE, "align", NK_MINIMIZED)) {
			if (nk_option_label(ctx, "left", m_style.window.header.align == NK_HEADER_LEFT)) m_style.window.header.align = NK_HEADER_LEFT;
			if (nk_option_label(ctx, "right", m_style.window.header.align == NK_HEADER_RIGHT)) m_style.window.header.align = NK_HEADER_RIGHT;
			nk_tree_pop(ctx);
		}
		if (nk_tree_push(ctx, NK_TREE_NODE, "padding", NK_MINIMIZED)) {
			PropertyVector2(ctx, "Vector2", m_style.window.header.padding, 0.f, 1000.f, 1.f, 1.f);
			nk_tree_pop(ctx);
		}
		if (nk_tree_push(ctx, NK_TREE_NODE, "label_padding", NK_MINIMIZED)) {
			PropertyVector2(ctx, "Vector2", m_style.window.header.label_padding, 0.f, 1000.f, 1.f, 1.f);
			nk_tree_pop(ctx);
		}
		if (nk_tree_push(ctx, NK_TREE_NODE, "spacing", NK_MINIMIZED)) {
			PropertyVector2(ctx, "Vector2", m_style.window.header.spacing, 0.f, 1000.f, 1.f, 1.f);
			nk_tree_pop(ctx);
		}
		nk_tree_pop(ctx);
	}
}
void NKStyle::HeaderCloseButtonEditor(nk_context* ctx, NuklearUI* pManager)
{
	if (nk_tree_push(ctx, NK_TREE_NODE, "close_button", NK_MINIMIZED)) {
		if (nk_tree_push(ctx, NK_TREE_NODE, "symbol", NK_MINIMIZED)) {
			if (nk_option_label(ctx, "NONE", m_style.window.header.close_symbol == NK_SYMBOL_NONE)) 			m_style.window.header.close_symbol = NK_SYMBOL_NONE;
			if (nk_option_label(ctx, "X", m_style.window.header.close_symbol == NK_SYMBOL_X)) 				m_style.window.header.close_symbol = NK_SYMBOL_X;
			if (nk_option_label(ctx, "UNDERSCORE", m_style.window.header.close_symbol == NK_SYMBOL_UNDERSCORE)) 		m_style.window.header.close_symbol = NK_SYMBOL_UNDERSCORE;
			if (nk_option_label(ctx, "CIRCLE_SOLID", m_style.window.header.close_symbol == NK_SYMBOL_CIRCLE_SOLID)) 	m_style.window.header.close_symbol = NK_SYMBOL_CIRCLE_SOLID;
			if (nk_option_label(ctx, "CIRCLE_OUTLINE", m_style.window.header.close_symbol == NK_SYMBOL_CIRCLE_OUTLINE))  m_style.window.header.close_symbol = NK_SYMBOL_CIRCLE_OUTLINE;
			if (nk_option_label(ctx, "RECT_SOLID", m_style.window.header.close_symbol == NK_SYMBOL_RECT_SOLID)) 		m_style.window.header.close_symbol = NK_SYMBOL_RECT_SOLID;
			if (nk_option_label(ctx, "RECT_OUTLINE", m_style.window.header.close_symbol == NK_SYMBOL_RECT_OUTLINE)) 	m_style.window.header.close_symbol = NK_SYMBOL_RECT_OUTLINE;
			if (nk_option_label(ctx, "TRIANGLE_UP", m_style.window.header.close_symbol == NK_SYMBOL_TRIANGLE_UP)) 	m_style.window.header.close_symbol = NK_SYMBOL_TRIANGLE_UP;
			if (nk_option_label(ctx, "TRIANGLE_DOWN", m_style.window.header.close_symbol == NK_SYMBOL_TRIANGLE_DOWN)) 	m_style.window.header.close_symbol = NK_SYMBOL_TRIANGLE_DOWN;
			if (nk_option_label(ctx, "TRIANGLE_LEFT", m_style.window.header.close_symbol == NK_SYMBOL_TRIANGLE_LEFT)) 	m_style.window.header.close_symbol = NK_SYMBOL_TRIANGLE_LEFT;
			if (nk_option_label(ctx, "TRIANGLE_RIGHT", m_style.window.header.close_symbol == NK_SYMBOL_TRIANGLE_RIGHT))  m_style.window.header.close_symbol = NK_SYMBOL_TRIANGLE_RIGHT;
			if (nk_option_label(ctx, "PLUS", m_style.window.header.close_symbol == NK_SYMBOL_PLUS)) 			m_style.window.header.close_symbol = NK_SYMBOL_PLUS;
			if (nk_option_label(ctx, "MINUS", m_style.window.header.close_symbol == NK_SYMBOL_MINUS)) 			m_style.window.header.close_symbol = NK_SYMBOL_MINUS;
			if (nk_option_label(ctx, "MAX", m_style.window.header.close_symbol == NK_SYMBOL_MAX)) 			m_style.window.header.close_symbol = NK_SYMBOL_MAX;
			nk_tree_pop(ctx);
		}

		CustomComponentsEditor(ctx, pManager, m_sButton_close, m_style.window.header.close_button, eTreeHeaderCloseButton);
		nk_tree_pop(ctx);
	}
}
void NKStyle::HeaderMinimizeButtonEditor(nk_context* ctx, NuklearUI* pManager)
{
	if (nk_tree_push(ctx, NK_TREE_NODE, "minimize_button", NK_MINIMIZED)) {
		if (nk_tree_push(ctx, NK_TREE_NODE, "symbol", NK_MINIMIZED)) {
			if (nk_option_label(ctx, "NONE", m_style.window.header.minimize_symbol == NK_SYMBOL_NONE)) 			m_style.window.header.minimize_symbol = NK_SYMBOL_NONE;
			if (nk_option_label(ctx, "X", m_style.window.header.minimize_symbol == NK_SYMBOL_X)) 				m_style.window.header.minimize_symbol = NK_SYMBOL_X;
			if (nk_option_label(ctx, "UNDERSCORE", m_style.window.header.minimize_symbol == NK_SYMBOL_UNDERSCORE)) 	m_style.window.header.minimize_symbol = NK_SYMBOL_UNDERSCORE;
			if (nk_option_label(ctx, "CIRCLE_SOLID", m_style.window.header.minimize_symbol == NK_SYMBOL_CIRCLE_SOLID)) 	m_style.window.header.minimize_symbol = NK_SYMBOL_CIRCLE_SOLID;
			if (nk_option_label(ctx, "CIRCLE_OUTLINE", m_style.window.header.minimize_symbol == NK_SYMBOL_CIRCLE_OUTLINE))	m_style.window.header.minimize_symbol = NK_SYMBOL_CIRCLE_OUTLINE;
			if (nk_option_label(ctx, "RECT_SOLID", m_style.window.header.minimize_symbol == NK_SYMBOL_RECT_SOLID)) 	m_style.window.header.minimize_symbol = NK_SYMBOL_RECT_SOLID;
			if (nk_option_label(ctx, "RECT_OUTLINE", m_style.window.header.minimize_symbol == NK_SYMBOL_RECT_OUTLINE)) 	m_style.window.header.minimize_symbol = NK_SYMBOL_RECT_OUTLINE;
			if (nk_option_label(ctx, "TRIANGLE_UP", m_style.window.header.minimize_symbol == NK_SYMBOL_TRIANGLE_UP)) 	m_style.window.header.minimize_symbol = NK_SYMBOL_TRIANGLE_UP;
			if (nk_option_label(ctx, "TRIANGLE_DOWN", m_style.window.header.minimize_symbol == NK_SYMBOL_TRIANGLE_DOWN)) 	m_style.window.header.minimize_symbol = NK_SYMBOL_TRIANGLE_DOWN;
			if (nk_option_label(ctx, "TRIANGLE_LEFT", m_style.window.header.minimize_symbol == NK_SYMBOL_TRIANGLE_LEFT)) 	m_style.window.header.minimize_symbol = NK_SYMBOL_TRIANGLE_LEFT;
			if (nk_option_label(ctx, "TRIANGLE_RIGHT", m_style.window.header.minimize_symbol == NK_SYMBOL_TRIANGLE_RIGHT))	m_style.window.header.minimize_symbol = NK_SYMBOL_TRIANGLE_RIGHT;
			if (nk_option_label(ctx, "PLUS", m_style.window.header.minimize_symbol == NK_SYMBOL_PLUS)) 			m_style.window.header.minimize_symbol = NK_SYMBOL_PLUS;
			if (nk_option_label(ctx, "MINUS", m_style.window.header.minimize_symbol == NK_SYMBOL_MINUS)) 			m_style.window.header.minimize_symbol = NK_SYMBOL_MINUS;
			if (nk_option_label(ctx, "MAX", m_style.window.header.minimize_symbol == NK_SYMBOL_MAX)) 			m_style.window.header.minimize_symbol = NK_SYMBOL_MAX;
			nk_tree_pop(ctx);
		}
		CustomComponentsEditor(ctx, pManager, m_sButton_minimize, m_style.window.header.minimize_button, eTreeHeaderMinimizeButton);
		nk_tree_pop(ctx);
	}

	if (nk_tree_push(ctx, NK_TREE_NODE, "maximize_symbol", NK_MINIMIZED)) {
		if (nk_option_label(ctx, "NONE", m_style.window.header.maximize_symbol == NK_SYMBOL_NONE)) 			m_style.window.header.maximize_symbol = NK_SYMBOL_NONE;
		if (nk_option_label(ctx, "X", m_style.window.header.maximize_symbol == NK_SYMBOL_X)) 				m_style.window.header.maximize_symbol = NK_SYMBOL_X;
		if (nk_option_label(ctx, "UNDERSCORE", m_style.window.header.maximize_symbol == NK_SYMBOL_UNDERSCORE)) 	m_style.window.header.maximize_symbol = NK_SYMBOL_UNDERSCORE;
		if (nk_option_label(ctx, "CIRCLE_SOLID", m_style.window.header.maximize_symbol == NK_SYMBOL_CIRCLE_SOLID)) 	m_style.window.header.maximize_symbol = NK_SYMBOL_CIRCLE_SOLID;
		if (nk_option_label(ctx, "CIRCLE_OUTLINE", m_style.window.header.maximize_symbol == NK_SYMBOL_CIRCLE_OUTLINE))	m_style.window.header.maximize_symbol = NK_SYMBOL_CIRCLE_OUTLINE;
		if (nk_option_label(ctx, "RECT_SOLID", m_style.window.header.maximize_symbol == NK_SYMBOL_RECT_SOLID)) 	m_style.window.header.maximize_symbol = NK_SYMBOL_RECT_SOLID;
		if (nk_option_label(ctx, "RECT_OUTLINE", m_style.window.header.maximize_symbol == NK_SYMBOL_RECT_OUTLINE)) 	m_style.window.header.maximize_symbol = NK_SYMBOL_RECT_OUTLINE;
		if (nk_option_label(ctx, "TRIANGLE_UP", m_style.window.header.maximize_symbol == NK_SYMBOL_TRIANGLE_UP)) 	m_style.window.header.maximize_symbol = NK_SYMBOL_TRIANGLE_UP;
		if (nk_option_label(ctx, "TRIANGLE_DOWN", m_style.window.header.maximize_symbol == NK_SYMBOL_TRIANGLE_DOWN)) 	m_style.window.header.maximize_symbol = NK_SYMBOL_TRIANGLE_DOWN;
		if (nk_option_label(ctx, "TRIANGLE_LEFT", m_style.window.header.maximize_symbol == NK_SYMBOL_TRIANGLE_LEFT)) 	m_style.window.header.maximize_symbol = NK_SYMBOL_TRIANGLE_LEFT;
		if (nk_option_label(ctx, "TRIANGLE_RIGHT", m_style.window.header.maximize_symbol == NK_SYMBOL_TRIANGLE_RIGHT))	m_style.window.header.maximize_symbol = NK_SYMBOL_TRIANGLE_RIGHT;
		if (nk_option_label(ctx, "PLUS", m_style.window.header.maximize_symbol == NK_SYMBOL_PLUS)) 			m_style.window.header.maximize_symbol = NK_SYMBOL_PLUS;
		if (nk_option_label(ctx, "MINUS", m_style.window.header.maximize_symbol == NK_SYMBOL_MINUS)) 			m_style.window.header.maximize_symbol = NK_SYMBOL_MINUS;
		if (nk_option_label(ctx, "MAX", m_style.window.header.maximize_symbol == NK_SYMBOL_MAX)) 			m_style.window.header.maximize_symbol = NK_SYMBOL_MAX;
		nk_tree_pop(ctx);
	}

}

void NKStyle::WindowEditor(nk_context* ctx, NuklearUI* pManager)
{
	FixedBackgroundEditor(ctx, pManager);
	BackgroundEditor(ctx);
	ScalerEditor(ctx, pManager);
	PropertiesEditor(ctx);
	PopupEditor(ctx);
	ComboEditor(ctx);
	ContextualEditor(ctx);
	MenuEditor(ctx);
	GroupEditor(ctx);
	TooltipEditor(ctx);
}
void NKStyle::FixedBackgroundEditor(nk_context* ctx, NuklearUI* pManager)
{
	if (nk_tree_push(ctx, NK_TREE_TAB, "fixed_background", NK_MINIMIZED)) {
		ItemEditor(ctx, pManager, m_sBackground);
		nk_tree_pop(ctx);
	}
}
void NKStyle::BackgroundEditor(nk_context* ctx)
{
	if (nk_tree_push(ctx, NK_TREE_TAB, "background", NK_MINIMIZED)) {
		ColorPicker(ctx, m_style.window.background);
		nk_tree_pop(ctx);
	}
}
void NKStyle::ScalerEditor(nk_context* ctx, NuklearUI* pManager)
{
	if (nk_tree_push(ctx, NK_TREE_TAB, "scaler", NK_MINIMIZED)) {
		ItemEditor(ctx, pManager, m_sScaler);
		nk_tree_pop(ctx);
	}
}
void NKStyle::PropertiesEditor(nk_context* ctx)
{
	if (nk_tree_push(ctx, NK_TREE_TAB, "properties", NK_MINIMIZED)) {
		nk_layout_row_dynamic(ctx, 22, 1);
		nk_property_float(ctx, "#border:", 0.f, &m_style.window.border, 100.f, 1.f, 1.f);
		ColorPicker(ctx, m_style.window.border_color);

		nk_property_float(ctx, "#rounding:", 0.f, &m_style.window.rounding, 100.f, 1.f, 1.f);
		PropertyVector2(ctx, "padding", m_style.window.padding, 0.f, 1000.f, 1.f, 1.f);
		PropertyVector2(ctx, "spacing", m_style.window.spacing, 0.f, 1000.f, 1.f, 1.f);
		PropertyVector2(ctx, "scrollbar_size", m_style.window.scrollbar_size, 0.f, 1000.f, 1.f, 1.f);
		PropertyVector2(ctx, "min_size", m_style.window.min_size, 0.f, 1000.f, 1.f, 1.f);
		nk_tree_pop(ctx);
	}
}
void NKStyle::PopupEditor(nk_context* ctx)
{
	if (nk_tree_push(ctx, NK_TREE_TAB, "popup", NK_MINIMIZED)) {
		nk_layout_row_dynamic(ctx, 22, 1);
		nk_property_float(ctx, "#Border:", 0.f, &m_style.window.popup_border, 100.f, 1.f, 1.f);
		ColorPicker(ctx, m_style.window.popup_border_color);
		nk_tree_pop(ctx);
	}
}
void NKStyle::ComboEditor(nk_context* ctx)
{
	if (nk_tree_push(ctx, NK_TREE_TAB, "combo", NK_MINIMIZED)) {
		nk_layout_row_dynamic(ctx, 22, 1);
		nk_property_float(ctx, "#Border:", 0.f, &m_style.window.combo_border, 100.f, 1.f, 1.f);
		ColorPicker(ctx, m_style.window.combo_border_color);
		nk_tree_pop(ctx);
	}
}
void NKStyle::ContextualEditor(nk_context* ctx)
{
	if (nk_tree_push(ctx, NK_TREE_TAB, "contextual", NK_MINIMIZED)) {
		nk_layout_row_dynamic(ctx, 22, 1);
		nk_property_float(ctx, "#Border:", 0.f, &m_style.window.contextual_border, 100.f, 1.f, 1.f);
		ColorPicker(ctx, m_style.window.contextual_border_color);
		nk_tree_pop(ctx);
	}
}
void NKStyle::MenuEditor(nk_context* ctx)
{
	if (nk_tree_push(ctx, NK_TREE_TAB, "menu", NK_MINIMIZED)) {
		nk_layout_row_dynamic(ctx, 22, 1);
		nk_property_float(ctx, "#Border:", 0.f, &m_style.window.menu_border, 100.f, 1.f, 1.f);
		ColorPicker(ctx, m_style.window.menu_border_color);
		nk_tree_pop(ctx);
	}
}
void NKStyle::GroupEditor(nk_context* ctx)
{
	if (nk_tree_push(ctx, NK_TREE_TAB, "group", NK_MINIMIZED)) {
		nk_layout_row_dynamic(ctx, 22, 1);
		nk_property_float(ctx, "#Border:", 0.f, &m_style.window.group_border, 100.f, 1.f, 1.f);
		ColorPicker(ctx, m_style.window.group_border_color);
		nk_tree_pop(ctx);
	}
}
void NKStyle::TooltipEditor(nk_context* ctx)
{
	if (nk_tree_push(ctx, NK_TREE_TAB, "tooltip", NK_MINIMIZED)) {
		nk_layout_row_dynamic(ctx, 22, 1);
		nk_property_float(ctx, "#Border:", 0.f, &m_style.window.tooltip_border, 100.f, 1.f, 1.f);
		ColorPicker(ctx, m_style.window.tooltip_border_color);
		nk_tree_pop(ctx);
	}
}

void NKStyle::ComponentEditor(nk_context* ctx, NuklearUI* pManager)
{
	TextComponentEditor(ctx, pManager);
	ButtonComponentEditor(ctx, pManager);
	ContextualButtonComponentEditor(ctx, pManager);
	MenuButtonComponentEditor(ctx, pManager);
	OptionComponentEditor(ctx, pManager);
	CheckboxComponentEditor(ctx, pManager);
	SelectableComponentEditor(ctx, pManager);
	SliderComponentEditor(ctx, pManager);
	ProgressComponentEditor(ctx, pManager);
	PropertyComponentEditor(ctx, pManager);
	EditComponentEditor(ctx, pManager);
	ChartComponentEditor(ctx, pManager);
	ScrollhComponentEditor(ctx, pManager);
	ScrollvComponentEditor(ctx, pManager);
	TabComponentEditor(ctx, pManager);
	ComboComponentEditor(ctx, pManager);
}

void NKStyle::TextComponentEditor(nk_context* ctx, NuklearUI* pManager)
{
	if (nk_tree_push(ctx, NK_TREE_TAB, "Text", NK_MINIMIZED)) {
		ColorPicker(ctx, m_style.text.color);
		PropertyVector2(ctx, "padding", m_style.text.padding, 0.f, 100.f, 1.f, 0.1f);
		nk_property_float(ctx, "#color_factor:", 0.f, &m_style.text.color_factor, 1.f, 0.01f, 0.01f);
		nk_property_float(ctx, "#disabled_factor:", 0.f, &m_style.text.disabled_factor, 1.f, 0.01f, 0.01f);
		nk_tree_pop(ctx);
	}
}
void NKStyle::ButtonComponentEditor(nk_context* ctx, NuklearUI* pManager)
{
	if (nk_tree_push(ctx, NK_TREE_NODE, "DefaultButton", NK_MINIMIZED)) {
		CustomComponentsEditor(ctx, pManager, m_sButton_default, m_style.button, eTreeDefaultButton);
		nk_tree_pop(ctx);
	}
}
void NKStyle::ContextualButtonComponentEditor(nk_context* ctx, NuklearUI* pManager)
{
	if (nk_tree_push(ctx, NK_TREE_NODE, "ContextualButton", NK_MINIMIZED)) {
		CustomComponentsEditor(ctx, pManager, m_sButton_contextual, m_style.contextual_button, eTreeContextualButton);
		nk_tree_pop(ctx);
	}
}
void NKStyle::MenuButtonComponentEditor(nk_context* ctx, NuklearUI* pManager)
{
	if (nk_tree_push(ctx, NK_TREE_NODE, "MenuButton", NK_MINIMIZED)) {
		CustomComponentsEditor(ctx, pManager, m_sButton_menu, m_style.menu_button, eTreeContextualButton);
		nk_tree_pop(ctx);
	}
}
void NKStyle::OptionComponentEditor(nk_context* ctx, NuklearUI* pManager)
{
	if (nk_tree_push(ctx, NK_TREE_NODE, "Option", NK_MINIMIZED)) {
		CustomComponentsEditor(ctx, pManager, m_sToggle_option, m_style.option, eTreeOptionToggle);
		nk_tree_pop(ctx);
	}
}
void NKStyle::CheckboxComponentEditor(nk_context* ctx, NuklearUI* pManager)
{
	if (nk_tree_push(ctx, NK_TREE_NODE, "Checkbox", NK_MINIMIZED)) {
		CustomComponentsEditor(ctx, pManager, m_sToggle_checkbox, m_style.checkbox, eTreeCheckboxToggle);
		nk_tree_pop(ctx);
	}
}
void NKStyle::SelectableComponentEditor(nk_context* ctx, NuklearUI* pManager)
{
	if (nk_tree_push(ctx, NK_TREE_NODE, "Selectable", NK_MINIMIZED)) {
		CustomComponentsEditor(ctx, pManager, m_sSelectable, m_style.selectable, eTreeSelectable);
		nk_tree_pop(ctx);
	}
}
void NKStyle::SliderComponentEditor(nk_context* ctx, NuklearUI* pManager)
{
	if (nk_tree_push(ctx, NK_TREE_NODE, "Slider", NK_MINIMIZED)) {
		CustomComponentsEditor(ctx, pManager, m_sSlider, m_style.slider, eTreeSlider);
		nk_tree_pop(ctx);
	}
}
void NKStyle::ProgressComponentEditor(nk_context* ctx, NuklearUI* pManager)
{
	if (nk_tree_push(ctx, NK_TREE_NODE, "Progress", NK_MINIMIZED)) {
		CustomComponentsEditor(ctx, pManager, m_sProgress, m_style.progress, eTreeSelectable);
		nk_tree_pop(ctx);
	}
}
void NKStyle::PropertyComponentEditor(nk_context* ctx, NuklearUI* pManager)
{
	if (nk_tree_push(ctx, NK_TREE_NODE, "Property", NK_MINIMIZED)) {
		CustomComponentsEditor(ctx, pManager, m_sProperty, m_style.property, eTreeProperty);
		nk_tree_pop(ctx);
	}
}
void NKStyle::EditComponentEditor(nk_context* ctx, NuklearUI* pManager)
{
	if (nk_tree_push(ctx, NK_TREE_NODE, "Edit", NK_MINIMIZED)) {
		CustomComponentsEditor(ctx, pManager, m_sEdit, m_style.edit, eTreeEdit);
		nk_tree_pop(ctx);
	}
}
void NKStyle::ChartComponentEditor(nk_context* ctx, NuklearUI* pManager)
{
	if (nk_tree_push(ctx, NK_TREE_NODE, "Chart", NK_MINIMIZED)) {
		CustomComponentsEditor(ctx, pManager, m_sChart, m_style.chart, eTreeChart);
		nk_tree_pop(ctx);
	}
}
void NKStyle::ScrollhComponentEditor(nk_context* ctx, NuklearUI* pManager)
{
	if (nk_tree_push(ctx, NK_TREE_NODE, "Scrollh", NK_MINIMIZED)) {
		CustomComponentsEditor(ctx, pManager, m_sScrollbarh, m_style.scrollh, eTreeScrollh);
		nk_tree_pop(ctx);
	}
}
void NKStyle::ScrollvComponentEditor(nk_context* ctx, NuklearUI* pManager)
{
	if (nk_tree_push(ctx, NK_TREE_NODE, "Scrollv", NK_MINIMIZED)) {
		CustomComponentsEditor(ctx, pManager, m_sScrollbarv, m_style.scrollv, eTreeScrollv);
		nk_tree_pop(ctx);
	}
}
void NKStyle::TabComponentEditor(nk_context* ctx, NuklearUI* pManager)
{
	if (nk_tree_push(ctx, NK_TREE_NODE, "Tab", NK_MINIMIZED)) {
		CustomComponentsEditor(ctx, pManager, m_sTab, m_style.tab, eTreeTab);
		nk_tree_pop(ctx);
	}
}
void NKStyle::ComboComponentEditor(nk_context* ctx, NuklearUI* pManager)
{
	if (nk_tree_push(ctx, NK_TREE_NODE, "Combo", NK_MINIMIZED)) {
		CustomComponentsEditor(ctx, pManager, m_sCombo, m_style.combo, eTreeCombo);
		nk_tree_pop(ctx);
	}
}

void NKStyle::CustomComponentsEditor(nk_context* ctx, NuklearUI* pManager, ComponentButton& tpi, struct nk_style_button& button, int id)
{
	int tree_index = 100;
	int tree_index_in = 200;
	int tree_index_in_in = 300;

	if (nk_tree_push_id(ctx, NK_TREE_NODE, "background", NK_MINIMIZED, id + tree_index++)) {

		if (nk_tree_push_id(ctx, NK_TREE_NODE, "normal", NK_MINIMIZED, id + tree_index_in++)) {
			ItemEditor(ctx, pManager, tpi.normal);
			nk_tree_pop(ctx);
		}
		if (nk_tree_push_id(ctx, NK_TREE_NODE, "hover", NK_MINIMIZED, id + tree_index_in++)) {
			ItemEditor(ctx, pManager, tpi.hover);
			nk_tree_pop(ctx);
		}
		if (nk_tree_push_id(ctx, NK_TREE_NODE, "active", NK_MINIMIZED, id + tree_index_in++)) {
			ItemEditor(ctx, pManager, tpi.active);
			nk_tree_pop(ctx);
		}
		if (nk_tree_push_id(ctx, NK_TREE_NODE, "color_factor", NK_MINIMIZED, id + tree_index_in++)) {
			nk_layout_row_dynamic(ctx, 22, 1);
			nk_property_float(ctx, "#color:", 0.f, &button.color_factor_background, 1.f, 0.01f, 0.01f);
			nk_tree_pop(ctx);
		}
		nk_tree_pop(ctx);
	}

	if (nk_tree_push_id(ctx, NK_TREE_NODE, "border", NK_MINIMIZED, id + tree_index++)) {
		nk_layout_row_dynamic(ctx, 22, 1);
		nk_property_float(ctx, "#Border:", 0.f, &button.border, 9.f, 1.f, 0.1f);
		ColorPicker(ctx, button.border_color);
		nk_tree_pop(ctx);
	}


	if (nk_tree_push_id(ctx, NK_TREE_NODE, "text", NK_MINIMIZED, id + tree_index++)) {

		if (nk_tree_push_id(ctx, NK_TREE_NODE, "background", NK_MINIMIZED, id + tree_index_in++)) {
			ColorPicker(ctx, button.text_background);
			nk_tree_pop(ctx);
		}
		if (nk_tree_push_id(ctx, NK_TREE_NODE, "normal", NK_MINIMIZED, id + tree_index_in++)) {
			ColorPicker(ctx, button.text_normal);
			nk_tree_pop(ctx);
		}
		if (nk_tree_push_id(ctx, NK_TREE_NODE, "hover", NK_MINIMIZED, id + tree_index_in++)) {
			ColorPicker(ctx, button.text_hover);
			nk_tree_pop(ctx);
		}
		if (nk_tree_push_id(ctx, NK_TREE_NODE, "active", NK_MINIMIZED, id + tree_index_in++)) {
			ColorPicker(ctx, button.text_active);
			nk_tree_pop(ctx);
		}

		if (nk_tree_push_id(ctx, NK_TREE_NODE, "align", NK_MINIMIZED, id + tree_index_in++)) {
			if (nk_option_label(ctx, "left", button.text_alignment == NK_HEADER_LEFT))  button.text_alignment = NK_HEADER_LEFT;
			if (nk_option_label(ctx, "center", button.text_alignment == NK_TEXT_CENTERED))  button.text_alignment = NK_TEXT_CENTERED;
			if (nk_option_label(ctx, "right", button.text_alignment == NK_HEADER_RIGHT))  button.text_alignment = NK_HEADER_RIGHT;
			nk_tree_pop(ctx);
		}
		if (nk_tree_push_id(ctx, NK_TREE_NODE, "color_factor", NK_MINIMIZED, id + tree_index_in++)) {
			nk_layout_row_dynamic(ctx, 22, 1);
			nk_property_float(ctx, "#color:", 0.f, &button.color_factor_text, 1.f, 0.01f, 0.01f);
			nk_tree_pop(ctx);
		}
		nk_tree_pop(ctx);
	}

	if (nk_tree_push_id(ctx, NK_TREE_NODE, "properties", NK_MINIMIZED, id + tree_index++)) {
		nk_layout_row_dynamic(ctx, 22, 1);
		nk_property_float(ctx, "#Rounding:", 0.f, &button.rounding, 100.f, 1.f, 0.1f);

		PropertyVector2(ctx, "padding", button.padding, 0.f, 1000.f, 1.f, 1.f);
		PropertyVector2(ctx, "image_padding", button.image_padding, 0.f, 1000.f, 1.f, 1.f);
		PropertyVector2(ctx, "touch_padding", button.touch_padding, 0.f, 1000.f, 1.f, 1.f);

		nk_layout_row_dynamic(ctx, 22, 1);
		nk_label(ctx, "disabled_factor", NK_TEXT_LEFT);
		nk_property_float(ctx, "#value:", 0.f, &button.disabled_factor, 1.f, 0.01f, 0.01f);
		nk_tree_pop(ctx);
	}
}
void NKStyle::CustomComponentsEditor(nk_context* ctx, NuklearUI* pManager, ComponentToggle& tpi, struct nk_style_toggle& toggle, int id)
{
	int tree_index = 100;
	int tree_index_in = 200;
	int tree_index_in_in = 300;

	if (nk_tree_push_id(ctx, NK_TREE_NODE, "background", NK_MINIMIZED, id + tree_index++)) {

		if (nk_tree_push_id(ctx, NK_TREE_NODE, "normal", NK_MINIMIZED, id + tree_index_in++)) {
			ItemEditor(ctx, pManager, tpi.normal);
			nk_tree_pop(ctx);
		}
		if (nk_tree_push_id(ctx, NK_TREE_NODE, "hover", NK_MINIMIZED, id + tree_index_in++)) {
			ItemEditor(ctx, pManager, tpi.hover);
			nk_tree_pop(ctx);
		}
		if (nk_tree_push_id(ctx, NK_TREE_NODE, "active", NK_MINIMIZED, id + tree_index_in++)) {
			ItemEditor(ctx, pManager, tpi.active);
			nk_tree_pop(ctx);
		}
		nk_tree_pop(ctx);
	}

	if (nk_tree_push_id(ctx, NK_TREE_NODE, "border", NK_MINIMIZED, id + tree_index++)) {
		nk_layout_row_dynamic(ctx, 22, 1);
		nk_property_float(ctx, "#Border:", 0.f, &toggle.border, 9.f, 1.f, 0.1f);
		ColorPicker(ctx, toggle.border_color);
		nk_tree_pop(ctx);
	}

	if (nk_tree_push_id(ctx, NK_TREE_NODE, "cursor", NK_MINIMIZED, id + tree_index++)) {

		if (nk_tree_push_id(ctx, NK_TREE_NODE, "normal", NK_MINIMIZED, id + tree_index_in++)) {
			ItemEditor(ctx, pManager, tpi.cursor_normal);
			nk_tree_pop(ctx);
		}
		if (nk_tree_push_id(ctx, NK_TREE_NODE, "hover", NK_MINIMIZED, id + tree_index_in++)) {
			ItemEditor(ctx, pManager, tpi.cursor_hover);
			nk_tree_pop(ctx);
		}
		nk_tree_pop(ctx);
	}

	if (nk_tree_push_id(ctx, NK_TREE_NODE, "text", NK_MINIMIZED, id + tree_index++)) {

		if (nk_tree_push_id(ctx, NK_TREE_NODE, "background", NK_MINIMIZED, id + tree_index_in++)) {
			ColorPicker(ctx, toggle.text_background);
			nk_tree_pop(ctx);
		}
		if (nk_tree_push_id(ctx, NK_TREE_NODE, "normal", NK_MINIMIZED, id + tree_index_in++)) {
			ColorPicker(ctx, toggle.text_normal);
			nk_tree_pop(ctx);
		}
		if (nk_tree_push_id(ctx, NK_TREE_NODE, "hover", NK_MINIMIZED, id + tree_index_in++)) {
			ColorPicker(ctx, toggle.text_hover);
			nk_tree_pop(ctx);
		}
		if (nk_tree_push_id(ctx, NK_TREE_NODE, "active", NK_MINIMIZED, id + tree_index_in++)) {
			ColorPicker(ctx, toggle.text_active);
			nk_tree_pop(ctx);
		}

		if (nk_tree_push_id(ctx, NK_TREE_NODE, "align", NK_MINIMIZED, id + tree_index_in++)) {
			if (nk_option_label(ctx, "left", toggle.text_alignment == NK_HEADER_LEFT))  toggle.text_alignment = NK_HEADER_LEFT;
			if (nk_option_label(ctx, "center", toggle.text_alignment == NK_TEXT_CENTERED))  toggle.text_alignment = NK_TEXT_CENTERED;
			if (nk_option_label(ctx, "right", toggle.text_alignment == NK_HEADER_RIGHT))  toggle.text_alignment = NK_HEADER_RIGHT;
			nk_tree_pop(ctx);
		}
		nk_tree_pop(ctx);
	}

	if (nk_tree_push_id(ctx, NK_TREE_NODE, "properties", NK_MINIMIZED, id + tree_index++)) {
		nk_layout_row_dynamic(ctx, 22, 1);

		PropertyVector2(ctx, "padding", toggle.padding, 0.f, 1000.f, 1.f, 1.f);
		PropertyVector2(ctx, "touch_padding", toggle.touch_padding, 0.f, 1000.f, 1.f, 1.f);

		nk_layout_row_dynamic(ctx, 22, 1);
		nk_label(ctx, "spacing", NK_TEXT_LEFT);
		nk_property_float(ctx, "#value:", 0.f, &toggle.spacing, 100.f, 1.f, 0.1f);
		nk_label(ctx, "color_factor", NK_TEXT_LEFT);
		nk_property_float(ctx, "#value:", 0.f, &toggle.color_factor, 1.f, 0.01f, 0.01f);
		nk_label(ctx, "disabled_factor", NK_TEXT_LEFT);
		nk_property_float(ctx, "#value:", 0.f, &toggle.disabled_factor, 1.f, 0.01f, 0.01f);
		nk_tree_pop(ctx);
	}
}
void NKStyle::CustomComponentsEditor(nk_context* ctx, NuklearUI* pManager, ComponentSelectable& tpi, struct nk_style_selectable& selectable, int id)
{
	int tree_index = 100;
	int tree_index_in = 200;
	int tree_index_in_in = 300;

	if (nk_tree_push_id(ctx, NK_TREE_NODE, "background(inactive)", NK_MINIMIZED, id + tree_index++)) {

		if (nk_tree_push_id(ctx, NK_TREE_NODE, "normal", NK_MINIMIZED, id + tree_index_in++)) {
			ItemEditor(ctx, pManager, tpi.normal);
			nk_tree_pop(ctx);
		}
		if (nk_tree_push_id(ctx, NK_TREE_NODE, "hover", NK_MINIMIZED, id + tree_index_in++)) {
			ItemEditor(ctx, pManager, tpi.hover);
			nk_tree_pop(ctx);
		}
		if (nk_tree_push_id(ctx, NK_TREE_NODE, "pressed", NK_MINIMIZED, id + tree_index_in++)) {
			ItemEditor(ctx, pManager, tpi.pressed);
			nk_tree_pop(ctx);
		}
		nk_tree_pop(ctx);
	}

	if (nk_tree_push_id(ctx, NK_TREE_NODE, "background(active)", NK_MINIMIZED, id + tree_index++)) {

		if (nk_tree_push_id(ctx, NK_TREE_NODE, "normal", NK_MINIMIZED, id + tree_index_in++)) {
			ItemEditor(ctx, pManager, tpi.normal_active);
			nk_tree_pop(ctx);
		}
		if (nk_tree_push_id(ctx, NK_TREE_NODE, "hover", NK_MINIMIZED, id + tree_index_in++)) {
			ItemEditor(ctx, pManager, tpi.hover_active);
			nk_tree_pop(ctx);
		}
		if (nk_tree_push_id(ctx, NK_TREE_NODE, "pressed", NK_MINIMIZED, id + tree_index_in++)) {
			ItemEditor(ctx, pManager, tpi.pressed_active);
			nk_tree_pop(ctx);
		}
		nk_tree_pop(ctx);
	}

	if (nk_tree_push_id(ctx, NK_TREE_NODE, "text(inactive)", NK_MINIMIZED, id + tree_index++)) {

		if (nk_tree_push_id(ctx, NK_TREE_NODE, "normal", NK_MINIMIZED, id + tree_index_in++)) {
			ColorPicker(ctx, selectable.text_normal);
			nk_tree_pop(ctx);
		}
		if (nk_tree_push_id(ctx, NK_TREE_NODE, "hover", NK_MINIMIZED, id + tree_index_in++)) {
			ColorPicker(ctx, selectable.text_hover);
			nk_tree_pop(ctx);
		}
		if (nk_tree_push_id(ctx, NK_TREE_NODE, "pressed", NK_MINIMIZED, id + tree_index_in++)) {
			ColorPicker(ctx, selectable.text_pressed);
			nk_tree_pop(ctx);
		}
		nk_tree_pop(ctx);
	}

	if (nk_tree_push_id(ctx, NK_TREE_NODE, "text(active)", NK_MINIMIZED, id + tree_index++)) {

		if (nk_tree_push_id(ctx, NK_TREE_NODE, "normal", NK_MINIMIZED, id + tree_index_in++)) {
			ColorPicker(ctx, selectable.text_normal_active);
			nk_tree_pop(ctx);
		}
		if (nk_tree_push_id(ctx, NK_TREE_NODE, "hover", NK_MINIMIZED, id + tree_index_in++)) {
			ColorPicker(ctx, selectable.text_hover_active);
			nk_tree_pop(ctx);
		}
		if (nk_tree_push_id(ctx, NK_TREE_NODE, "pressed", NK_MINIMIZED, id + tree_index_in++)) {
			ColorPicker(ctx, selectable.text_pressed_active);
			nk_tree_pop(ctx);
		}
		if (nk_tree_push_id(ctx, NK_TREE_NODE, "background", NK_MINIMIZED, id + tree_index_in++)) {
			ColorPicker(ctx, selectable.text_background);
			nk_tree_pop(ctx);
		}

		if (nk_tree_push_id(ctx, NK_TREE_NODE, "align", NK_MINIMIZED, id + tree_index_in++)) {
			if (nk_option_label(ctx, "left", selectable.text_alignment == NK_HEADER_LEFT))  selectable.text_alignment = NK_HEADER_LEFT;
			if (nk_option_label(ctx, "center", selectable.text_alignment == NK_TEXT_CENTERED))  selectable.text_alignment = NK_TEXT_CENTERED;
			if (nk_option_label(ctx, "right", selectable.text_alignment == NK_HEADER_RIGHT))  selectable.text_alignment = NK_HEADER_RIGHT;
			nk_tree_pop(ctx);
		}
		nk_tree_pop(ctx);
	}

	if (nk_tree_push_id(ctx, NK_TREE_NODE, "properties", NK_MINIMIZED, id + tree_index++)) {
		nk_layout_row_dynamic(ctx, 22, 1);
		nk_label(ctx, "rounding", NK_TEXT_LEFT);
		nk_property_float(ctx, "#value:", 0.f, &selectable.rounding, 100.f, 1.f, 0.1f);

		PropertyVector2(ctx, "padding", selectable.padding, 0.f, 1000.f, 1.f, 1.f);
		PropertyVector2(ctx, "touch_padding", selectable.touch_padding, 0.f, 1000.f, 1.f, 1.f);
		PropertyVector2(ctx, "image_padding", selectable.image_padding, 0.f, 1000.f, 1.f, 1.f);

		nk_layout_row_dynamic(ctx, 22, 1);
		nk_label(ctx, "color_factor", NK_TEXT_LEFT);
		nk_property_float(ctx, "#value:", 0.f, &selectable.color_factor, 1.f, 0.01f, 0.01f);
		nk_label(ctx, "disabled_factor", NK_TEXT_LEFT);
		nk_property_float(ctx, "#value:", 0.f, &selectable.disabled_factor, 1.f, 0.01f, 0.01f);
		nk_tree_pop(ctx);
	}
}
void NKStyle::CustomComponentsEditor(nk_context* ctx, NuklearUI* pManager, ComponentSlider& tpi, struct nk_style_slider& slider, int id)
{
	int tree_index = 100;
	int tree_index_in = 200;
	int tree_index_in_in = 300;

	if (nk_tree_push_id(ctx, NK_TREE_NODE, "background", NK_MINIMIZED, id + tree_index++)) {

		if (nk_tree_push_id(ctx, NK_TREE_NODE, "normal", NK_MINIMIZED, id + tree_index_in++)) {
			ItemEditor(ctx, pManager, tpi.normal);
			nk_tree_pop(ctx);
		}
		if (nk_tree_push_id(ctx, NK_TREE_NODE, "hover", NK_MINIMIZED, id + tree_index_in++)) {
			ItemEditor(ctx, pManager, tpi.hover);
			nk_tree_pop(ctx);
		}
		if (nk_tree_push_id(ctx, NK_TREE_NODE, "active", NK_MINIMIZED, id + tree_index_in++)) {
			ItemEditor(ctx, pManager, tpi.active);
			nk_tree_pop(ctx);
		}
		nk_tree_pop(ctx);
	}

	if (nk_tree_push_id(ctx, NK_TREE_NODE, "background(bar)", NK_MINIMIZED, id + tree_index++)) {

		if (nk_tree_push_id(ctx, NK_TREE_NODE, "normal", NK_MINIMIZED, id + tree_index_in++)) {
			ColorPicker(ctx, slider.bar_normal);
			nk_tree_pop(ctx);
		}
		if (nk_tree_push_id(ctx, NK_TREE_NODE, "hover", NK_MINIMIZED, id + tree_index_in++)) {
			ColorPicker(ctx, slider.bar_hover);
			nk_tree_pop(ctx);
		}
		if (nk_tree_push_id(ctx, NK_TREE_NODE, "active", NK_MINIMIZED, id + tree_index_in++)) {
			ColorPicker(ctx, slider.bar_active);
			nk_tree_pop(ctx);
		}
		if (nk_tree_push_id(ctx, NK_TREE_NODE, "filled", NK_MINIMIZED, id + tree_index_in++)) {
			ColorPicker(ctx, slider.bar_filled);
			nk_tree_pop(ctx);
		}
		nk_tree_pop(ctx);
	}

	if (nk_tree_push_id(ctx, NK_TREE_NODE, "cursor", NK_MINIMIZED, id + tree_index++)) {

		if (nk_tree_push_id(ctx, NK_TREE_NODE, "normal", NK_MINIMIZED, id + tree_index_in++)) {
			ItemEditor(ctx, pManager, tpi.cursor_normal);
			nk_tree_pop(ctx);
		}
		if (nk_tree_push_id(ctx, NK_TREE_NODE, "hover", NK_MINIMIZED, id + tree_index_in++)) {
			ItemEditor(ctx, pManager, tpi.cursor_hover);
			nk_tree_pop(ctx);
		}
		if (nk_tree_push_id(ctx, NK_TREE_NODE, "active", NK_MINIMIZED, id + tree_index_in++)) {
			ItemEditor(ctx, pManager, tpi.cursor_active);
			nk_tree_pop(ctx);
		}
		nk_tree_pop(ctx);
	}

	if (nk_tree_push_id(ctx, NK_TREE_NODE, "properties", NK_MINIMIZED, id + tree_index++)) {
		nk_layout_row_dynamic(ctx, 22, 1);
		nk_label(ctx, "border", NK_TEXT_LEFT);
		nk_property_float(ctx, "#value:", 0.f, &slider.border, 100.f, 1.f, 0.1f);
		nk_label(ctx, "rounding", NK_TEXT_LEFT);
		nk_property_float(ctx, "#value:", 0.f, &slider.rounding, 100.f, 1.f, 0.1f);

		PropertyVector2(ctx, "padding", slider.padding, 0.f, 1000.f, 1.f, 1.f);
		PropertyVector2(ctx, "spacing", slider.spacing, 0.f, 1000.f, 1.f, 1.f);
		PropertyVector2(ctx, "cursor_size", slider.cursor_size, 0.f, 1000.f, 1.f, 1.f);

		nk_layout_row_dynamic(ctx, 22, 1);
		nk_label(ctx, "color_factor", NK_TEXT_LEFT);
		nk_property_float(ctx, "#value:", 0.f, &slider.color_factor, 1.f, 0.01f, 0.01f);
		nk_label(ctx, "disabled_factor", NK_TEXT_LEFT);
		nk_property_float(ctx, "#value:", 0.f, &slider.disabled_factor, 1.f, 0.01f, 0.01f);
		nk_tree_pop(ctx);
	}


	if (nk_tree_push_id(ctx, NK_TREE_NODE, "optional buttons", NK_MINIMIZED, id + tree_index++)) {

		nk_checkbox_label(ctx, "Show_buttons", &slider.show_buttons);

		if (nk_tree_push_id(ctx, NK_TREE_NODE, "inc_button", NK_MINIMIZED, id + tree_index_in++)) {
			if (nk_tree_push_id(ctx, NK_TREE_NODE, "symbol", NK_MINIMIZED, id + tree_index_in_in++)) {
				if (nk_option_label(ctx, "NONE", slider.inc_symbol == NK_SYMBOL_NONE)) 			slider.inc_symbol = NK_SYMBOL_NONE;
				if (nk_option_label(ctx, "X", slider.inc_symbol == NK_SYMBOL_X)) 				slider.inc_symbol = NK_SYMBOL_X;
				if (nk_option_label(ctx, "UNDERSCORE", slider.inc_symbol == NK_SYMBOL_UNDERSCORE)) 	slider.inc_symbol = NK_SYMBOL_UNDERSCORE;
				if (nk_option_label(ctx, "CIRCLE_SOLID", slider.inc_symbol == NK_SYMBOL_CIRCLE_SOLID)) 	slider.inc_symbol = NK_SYMBOL_CIRCLE_SOLID;
				if (nk_option_label(ctx, "CIRCLE_OUTLINE", slider.inc_symbol == NK_SYMBOL_CIRCLE_OUTLINE))	slider.inc_symbol = NK_SYMBOL_CIRCLE_OUTLINE;
				if (nk_option_label(ctx, "RECT_SOLID", slider.inc_symbol == NK_SYMBOL_RECT_SOLID)) 	slider.inc_symbol = NK_SYMBOL_RECT_SOLID;
				if (nk_option_label(ctx, "RECT_OUTLINE", slider.inc_symbol == NK_SYMBOL_RECT_OUTLINE)) 	slider.inc_symbol = NK_SYMBOL_RECT_OUTLINE;
				if (nk_option_label(ctx, "TRIANGLE_UP", slider.inc_symbol == NK_SYMBOL_TRIANGLE_UP)) 	slider.inc_symbol = NK_SYMBOL_TRIANGLE_UP;
				if (nk_option_label(ctx, "TRIANGLE_DOWN", slider.inc_symbol == NK_SYMBOL_TRIANGLE_DOWN)) 	slider.inc_symbol = NK_SYMBOL_TRIANGLE_DOWN;
				if (nk_option_label(ctx, "TRIANGLE_LEFT", slider.inc_symbol == NK_SYMBOL_TRIANGLE_LEFT)) 	slider.inc_symbol = NK_SYMBOL_TRIANGLE_LEFT;
				if (nk_option_label(ctx, "TRIANGLE_RIGHT", slider.inc_symbol == NK_SYMBOL_TRIANGLE_RIGHT))	slider.inc_symbol = NK_SYMBOL_TRIANGLE_RIGHT;
				if (nk_option_label(ctx, "PLUS", slider.inc_symbol == NK_SYMBOL_PLUS)) 			slider.inc_symbol = NK_SYMBOL_PLUS;
				if (nk_option_label(ctx, "MINUS", slider.inc_symbol == NK_SYMBOL_MINUS)) 			slider.inc_symbol = NK_SYMBOL_MINUS;
				if (nk_option_label(ctx, "MAX", slider.inc_symbol == NK_SYMBOL_MAX)) 			slider.inc_symbol = NK_SYMBOL_MAX;
				nk_tree_pop(ctx);
			}
			CustomComponentsEditor(ctx, pManager, tpi.inc_button, slider.inc_button, id + tree_index++);
			nk_tree_pop(ctx);
		}

		if (nk_tree_push_id(ctx, NK_TREE_NODE, "dec_button", NK_MINIMIZED, id + tree_index_in++)) {
			if (nk_tree_push_id(ctx, NK_TREE_NODE, "symbol", NK_MINIMIZED, id + tree_index_in_in++)) {
				if (nk_option_label(ctx, "NONE", slider.dec_symbol == NK_SYMBOL_NONE)) 			slider.dec_symbol = NK_SYMBOL_NONE;
				if (nk_option_label(ctx, "X", slider.dec_symbol == NK_SYMBOL_X)) 				slider.dec_symbol = NK_SYMBOL_X;
				if (nk_option_label(ctx, "UNDERSCORE", slider.dec_symbol == NK_SYMBOL_UNDERSCORE)) 	slider.dec_symbol = NK_SYMBOL_UNDERSCORE;
				if (nk_option_label(ctx, "CIRCLE_SOLID", slider.dec_symbol == NK_SYMBOL_CIRCLE_SOLID)) 	slider.dec_symbol = NK_SYMBOL_CIRCLE_SOLID;
				if (nk_option_label(ctx, "CIRCLE_OUTLINE", slider.dec_symbol == NK_SYMBOL_CIRCLE_OUTLINE))	slider.dec_symbol = NK_SYMBOL_CIRCLE_OUTLINE;
				if (nk_option_label(ctx, "RECT_SOLID", slider.dec_symbol == NK_SYMBOL_RECT_SOLID)) 	slider.dec_symbol = NK_SYMBOL_RECT_SOLID;
				if (nk_option_label(ctx, "RECT_OUTLINE", slider.dec_symbol == NK_SYMBOL_RECT_OUTLINE)) 	slider.dec_symbol = NK_SYMBOL_RECT_OUTLINE;
				if (nk_option_label(ctx, "TRIANGLE_UP", slider.dec_symbol == NK_SYMBOL_TRIANGLE_UP)) 	slider.dec_symbol = NK_SYMBOL_TRIANGLE_UP;
				if (nk_option_label(ctx, "TRIANGLE_DOWN", slider.dec_symbol == NK_SYMBOL_TRIANGLE_DOWN)) 	slider.dec_symbol = NK_SYMBOL_TRIANGLE_DOWN;
				if (nk_option_label(ctx, "TRIANGLE_LEFT", slider.dec_symbol == NK_SYMBOL_TRIANGLE_LEFT)) 	slider.dec_symbol = NK_SYMBOL_TRIANGLE_LEFT;
				if (nk_option_label(ctx, "TRIANGLE_RIGHT", slider.dec_symbol == NK_SYMBOL_TRIANGLE_RIGHT))	slider.dec_symbol = NK_SYMBOL_TRIANGLE_RIGHT;
				if (nk_option_label(ctx, "PLUS", slider.dec_symbol == NK_SYMBOL_PLUS)) 			slider.dec_symbol = NK_SYMBOL_PLUS;
				if (nk_option_label(ctx, "MINUS", slider.dec_symbol == NK_SYMBOL_MINUS)) 			slider.dec_symbol = NK_SYMBOL_MINUS;
				if (nk_option_label(ctx, "MAX", slider.dec_symbol == NK_SYMBOL_MAX)) 			slider.dec_symbol = NK_SYMBOL_MAX;
				nk_tree_pop(ctx);
			}
			CustomComponentsEditor(ctx, pManager, tpi.dec_button, slider.dec_button, id + tree_index_in_in++);
			nk_tree_pop(ctx);
		}
		nk_tree_pop(ctx);
	}
}
void NKStyle::CustomComponentsEditor(nk_context* ctx, NuklearUI* pManager, ComponentProgress& tpi, struct nk_style_progress& progress, int id)
{
	int tree_index = 100;
	int tree_index_in = 200;
	int tree_index_in_in = 300;

	if (nk_tree_push_id(ctx, NK_TREE_NODE, "background", NK_MINIMIZED, id + tree_index++)) {

		if (nk_tree_push_id(ctx, NK_TREE_NODE, "normal", NK_MINIMIZED, id + tree_index_in++)) {
			ItemEditor(ctx, pManager, tpi.normal);
			nk_tree_pop(ctx);
		}
		if (nk_tree_push_id(ctx, NK_TREE_NODE, "hover", NK_MINIMIZED, id + tree_index_in++)) {
			ItemEditor(ctx, pManager, tpi.hover);
			nk_tree_pop(ctx);
		}
		if (nk_tree_push_id(ctx, NK_TREE_NODE, "active", NK_MINIMIZED, id + tree_index_in++)) {
			ItemEditor(ctx, pManager, tpi.active);
			nk_tree_pop(ctx);
		}
		if (nk_tree_push_id(ctx, NK_TREE_NODE, "border_color", NK_MINIMIZED, id + tree_index_in++)) {
			ColorPicker(ctx, progress.border_color);
			nk_tree_pop(ctx);
		}
		nk_tree_pop(ctx);
	}

	if (nk_tree_push_id(ctx, NK_TREE_NODE, "cursor", NK_MINIMIZED, id + tree_index++)) {

		if (nk_tree_push_id(ctx, NK_TREE_NODE, "normal", NK_MINIMIZED, id + tree_index_in++)) {
			ItemEditor(ctx, pManager, tpi.cursor_normal);
			nk_tree_pop(ctx);
		}
		if (nk_tree_push_id(ctx, NK_TREE_NODE, "hover", NK_MINIMIZED, id + tree_index_in++)) {
			ItemEditor(ctx, pManager, tpi.cursor_hover);
			nk_tree_pop(ctx);
		}
		if (nk_tree_push_id(ctx, NK_TREE_NODE, "active", NK_MINIMIZED, id + tree_index_in++)) {
			ItemEditor(ctx, pManager, tpi.cursor_active);
			nk_tree_pop(ctx);
		}
		if (nk_tree_push_id(ctx, NK_TREE_NODE, "border_color", NK_MINIMIZED, id + tree_index_in++)) {
			ColorPicker(ctx, progress.cursor_border_color);
			nk_tree_pop(ctx);
		}
		nk_tree_pop(ctx);
	}

	if (nk_tree_push_id(ctx, NK_TREE_NODE, "properties", NK_MINIMIZED, id + tree_index++)) {
		nk_layout_row_dynamic(ctx, 22, 1);
		nk_label(ctx, "rounding", NK_TEXT_LEFT);
		nk_property_float(ctx, "#value:", 0.f, &progress.rounding, 100.f, 1.f, 0.1f);
		nk_label(ctx, "border", NK_TEXT_LEFT);
		nk_property_float(ctx, "#value:", 0.f, &progress.border, 100.f, 1.f, 0.1f);
		nk_label(ctx, "cursor_border", NK_TEXT_LEFT);
		nk_property_float(ctx, "#value:", 0.f, &progress.cursor_border, 100.f, 1.f, 0.1f);
		nk_label(ctx, "cursor_rounding", NK_TEXT_LEFT);
		nk_property_float(ctx, "#value:", 0.f, &progress.cursor_rounding, 100.f, 1.f, 0.1f);

		PropertyVector2(ctx, "padding", progress.padding, 0.f, 1000.f, 1.f, 1.f);

		nk_layout_row_dynamic(ctx, 22, 1);
		nk_label(ctx, "color_factor", NK_TEXT_LEFT);
		nk_property_float(ctx, "#value:", 0.f, &progress.color_factor, 1.f, 0.01f, 0.01f);
		nk_label(ctx, "disabled_factor", NK_TEXT_LEFT);
		nk_property_float(ctx, "#value:", 0.f, &progress.disabled_factor, 1.f, 0.01f, 0.01f);
		nk_tree_pop(ctx);
	}
}
void NKStyle::CustomComponentsEditor(nk_context* ctx, NuklearUI* pManager, ComponentProperty& tpi, struct nk_style_property& property, int id)
{
	int tree_index = 100;
	int tree_index_in = 200;
	int tree_index_in_in = 300;

	if (nk_tree_push_id(ctx, NK_TREE_NODE, "background", NK_MINIMIZED, id + tree_index++)) {

		if (nk_tree_push_id(ctx, NK_TREE_NODE, "normal", NK_MINIMIZED, id + tree_index_in++)) {
			ItemEditor(ctx, pManager, tpi.normal);
			nk_tree_pop(ctx);
		}
		if (nk_tree_push_id(ctx, NK_TREE_NODE, "hover", NK_MINIMIZED, id + tree_index_in++)) {
			ItemEditor(ctx, pManager, tpi.hover);
			nk_tree_pop(ctx);
		}
		if (nk_tree_push_id(ctx, NK_TREE_NODE, "active", NK_MINIMIZED, id + tree_index_in++)) {
			ItemEditor(ctx, pManager, tpi.active);
			nk_tree_pop(ctx);
		}
		if (nk_tree_push_id(ctx, NK_TREE_NODE, "border_color", NK_MINIMIZED, id + tree_index_in++)) {
			ColorPicker(ctx, property.border_color);
			nk_tree_pop(ctx);
		}
		nk_tree_pop(ctx);
	}

	if (nk_tree_push_id(ctx, NK_TREE_NODE, "label", NK_MINIMIZED, id + tree_index++)) {

		if (nk_tree_push_id(ctx, NK_TREE_NODE, "normal", NK_MINIMIZED, id + tree_index_in++)) {
			ColorPicker(ctx, property.label_normal);
			nk_tree_pop(ctx);
		}
		if (nk_tree_push_id(ctx, NK_TREE_NODE, "hover", NK_MINIMIZED, id + tree_index_in++)) {
			ColorPicker(ctx, property.label_hover);
			nk_tree_pop(ctx);
		}
		if (nk_tree_push_id(ctx, NK_TREE_NODE, "active", NK_MINIMIZED, id + tree_index_in++)) {
			ColorPicker(ctx, property.label_active);
			nk_tree_pop(ctx);
		}
		nk_tree_pop(ctx);
	}

	if (nk_tree_push_id(ctx, NK_TREE_NODE, "properties", NK_MINIMIZED, id + tree_index++)) {
		nk_layout_row_dynamic(ctx, 22, 1);
		nk_label(ctx, "rounding", NK_TEXT_LEFT);
		nk_property_float(ctx, "#value:", 0.f, &property.rounding, 100.f, 1.f, 0.1f);
		nk_label(ctx, "border", NK_TEXT_LEFT);
		nk_property_float(ctx, "#value:", 0.f, &property.border, 100.f, 1.f, 0.1f);
		PropertyVector2(ctx, "padding", property.padding, 0.f, 1000.f, 1.f, 1.f);
		nk_layout_row_dynamic(ctx, 22, 1);
		nk_label(ctx, "color_factor", NK_TEXT_LEFT);
		nk_property_float(ctx, "#value:", 0.f, &property.color_factor, 1.f, 0.01f, 0.01f);
		nk_label(ctx, "disabled_factor", NK_TEXT_LEFT);
		nk_property_float(ctx, "#value:", 0.f, &property.disabled_factor, 1.f, 0.01f, 0.01f);
		nk_tree_pop(ctx);
	}


	if (nk_tree_push_id(ctx, NK_TREE_NODE, "edit", NK_MINIMIZED, id + tree_index++)) {
		CustomComponentsEditor(ctx, pManager, tpi.edit, property.edit, id + tree_index_in_in + 100);
		CustomComponentsEditor(ctx, pManager, tpi.inc_button, property.inc_button, id + tree_index_in_in + 200);
		CustomComponentsEditor(ctx, pManager, tpi.dec_button, property.dec_button, id + tree_index_in_in + 300);
		nk_tree_pop(ctx);
	}
}
void NKStyle::CustomComponentsEditor(nk_context* ctx, NuklearUI* pManager, ComponentEdit& tpi, struct nk_style_edit& edit, int id)
{
	int tree_index = 100;
	int tree_index_in = 200;
	int tree_index_in_in = 300;

	if (nk_tree_push_id(ctx, NK_TREE_NODE, "background", NK_MINIMIZED, id + tree_index++)) {

		if (nk_tree_push_id(ctx, NK_TREE_NODE, "normal", NK_MINIMIZED, id + tree_index_in++)) {
			ItemEditor(ctx, pManager, tpi.normal);
			nk_tree_pop(ctx);
		}
		if (nk_tree_push_id(ctx, NK_TREE_NODE, "hover", NK_MINIMIZED, id + tree_index_in++)) {
			ItemEditor(ctx, pManager, tpi.hover);
			nk_tree_pop(ctx);
		}
		if (nk_tree_push_id(ctx, NK_TREE_NODE, "active", NK_MINIMIZED, id + tree_index_in++)) {
			ItemEditor(ctx, pManager, tpi.active);
			nk_tree_pop(ctx);
		}
		if (nk_tree_push_id(ctx, NK_TREE_NODE, "border_color", NK_MINIMIZED, id + tree_index_in++)) {
			ColorPicker(ctx, edit.border_color);
			nk_tree_pop(ctx);
		}
		CustomComponentsEditor(ctx, pManager, tpi.scrollbar, edit.scrollbar, id + tree_index_in + 100);
		nk_tree_pop(ctx);
	}

	if (nk_tree_push_id(ctx, NK_TREE_NODE, "cursor", NK_MINIMIZED, id + tree_index++)) {

		if (nk_tree_push_id(ctx, NK_TREE_NODE, "normal", NK_MINIMIZED, id + tree_index_in++)) {
			ColorPicker(ctx, edit.cursor_normal);
			nk_tree_pop(ctx);
		}
		if (nk_tree_push_id(ctx, NK_TREE_NODE, "hover", NK_MINIMIZED, id + tree_index_in++)) {
			ColorPicker(ctx, edit.cursor_hover);
			nk_tree_pop(ctx);
		}
		if (nk_tree_push_id(ctx, NK_TREE_NODE, "text_normal", NK_MINIMIZED, id + tree_index_in++)) {
			ColorPicker(ctx, edit.cursor_text_normal);
			nk_tree_pop(ctx);
		}
		if (nk_tree_push_id(ctx, NK_TREE_NODE, "text_hover", NK_MINIMIZED, id + tree_index_in++)) {
			ColorPicker(ctx, edit.cursor_text_hover);
			nk_tree_pop(ctx);
		}
		nk_tree_pop(ctx);
	}

	if (nk_tree_push_id(ctx, NK_TREE_NODE, "text(unselected)", NK_MINIMIZED, id + tree_index++)) {

		if (nk_tree_push_id(ctx, NK_TREE_NODE, "normal", NK_MINIMIZED, id + tree_index_in++)) {
			ColorPicker(ctx, edit.text_normal);
			nk_tree_pop(ctx);
		}
		if (nk_tree_push_id(ctx, NK_TREE_NODE, "hover", NK_MINIMIZED, id + tree_index_in++)) {
			ColorPicker(ctx, edit.text_hover);
			nk_tree_pop(ctx);
		}
		if (nk_tree_push_id(ctx, NK_TREE_NODE, "active", NK_MINIMIZED, id + tree_index_in++)) {
			ColorPicker(ctx, edit.text_active);
			nk_tree_pop(ctx);
		}
		nk_tree_pop(ctx);
	}

	if (nk_tree_push_id(ctx, NK_TREE_NODE, "text(selected)", NK_MINIMIZED, id + tree_index++)) {

		if (nk_tree_push_id(ctx, NK_TREE_NODE, "normal", NK_MINIMIZED, id + tree_index_in++)) {
			ColorPicker(ctx, edit.selected_normal);
			nk_tree_pop(ctx);
		}
		if (nk_tree_push_id(ctx, NK_TREE_NODE, "hover", NK_MINIMIZED, id + tree_index_in++)) {
			ColorPicker(ctx, edit.selected_hover);
			nk_tree_pop(ctx);
		}
		if (nk_tree_push_id(ctx, NK_TREE_NODE, "text_normal", NK_MINIMIZED, id + tree_index_in++)) {
			ColorPicker(ctx, edit.selected_text_normal);
			nk_tree_pop(ctx);
		}
		if (nk_tree_push_id(ctx, NK_TREE_NODE, "text_hover", NK_MINIMIZED, id + tree_index_in++)) {
			ColorPicker(ctx, edit.selected_text_hover);
			nk_tree_pop(ctx);
		}
		nk_tree_pop(ctx);
	}

	if (nk_tree_push_id(ctx, NK_TREE_NODE, "properties", NK_MINIMIZED, id + tree_index++)) {
		nk_layout_row_dynamic(ctx, 22, 1);
		nk_label(ctx, "rounding", NK_TEXT_LEFT);
		nk_property_float(ctx, "#value:", 0.f, &edit.rounding, 100.f, 1.f, 0.1f);
		nk_label(ctx, "border", NK_TEXT_LEFT);
		nk_property_float(ctx, "#value:", 0.f, &edit.border, 100.f, 1.f, 0.1f);
		nk_label(ctx, "cursor_size", NK_TEXT_LEFT);
		nk_property_float(ctx, "#value:", 0.f, &edit.cursor_size, 100.f, 1.f, 0.1f);

		PropertyVector2(ctx, "scrollbar_size", edit.scrollbar_size, 0.f, 1000.f, 1.f, 1.f);
		PropertyVector2(ctx, "padding", edit.padding, 0.f, 1000.f, 1.f, 1.f);

		nk_layout_row_dynamic(ctx, 22, 1);
		nk_label(ctx, "row_padding", NK_TEXT_LEFT);
		nk_property_float(ctx, "#value:", 0.f, &edit.row_padding, 100.f, 1.f, 0.1f);
		nk_label(ctx, "color_factor", NK_TEXT_LEFT);
		nk_property_float(ctx, "#value:", 0.f, &edit.color_factor, 1.f, 0.01f, 0.01f);
		nk_label(ctx, "disabled_factor", NK_TEXT_LEFT);
		nk_property_float(ctx, "#value:", 0.f, &edit.disabled_factor, 1.f, 0.01f, 0.01f);
		nk_tree_pop(ctx);
	}
}
void NKStyle::CustomComponentsEditor(nk_context* ctx, NuklearUI* pManager, NKStyleItem& nsi, nk_style_chart& chart, int id)
{

	int tree_index = 100;
	int tree_index_in = 200;
	int tree_index_in_in = 300;

	if (nk_tree_push_id(ctx, NK_TREE_NODE, "colors", NK_MINIMIZED, id + tree_index++)) {

		if (nk_tree_push_id(ctx, NK_TREE_NODE, "background", NK_MINIMIZED, id + tree_index_in++)) {
			ItemEditor(ctx, pManager, nsi);
			nk_tree_pop(ctx);
		}

		if (nk_tree_push_id(ctx, NK_TREE_NODE, "border", NK_MINIMIZED, id + tree_index_in++)) {
			ColorPicker(ctx, chart.border_color);
			nk_tree_pop(ctx);
		}
		if (nk_tree_push_id(ctx, NK_TREE_NODE, "selected", NK_MINIMIZED, id + tree_index_in++)) {
			ColorPicker(ctx, chart.selected_color);
			nk_tree_pop(ctx);
		}
		if (nk_tree_push_id(ctx, NK_TREE_NODE, "color", NK_MINIMIZED, id + tree_index_in++)) {
			ColorPicker(ctx, chart.color);
			nk_tree_pop(ctx);
		}
		nk_tree_pop(ctx);
	}

	if (nk_tree_push_id(ctx, NK_TREE_NODE, "properties", NK_MINIMIZED, id + tree_index++)) {
		nk_layout_row_dynamic(ctx, 22, 1);
		nk_label(ctx, "rounding", NK_TEXT_LEFT);
		nk_property_float(ctx, "#value:", 0.f, &chart.rounding, 100.f, 1.f, 0.1f);
		nk_label(ctx, "border", NK_TEXT_LEFT);
		nk_property_float(ctx, "#value:", 0.f, &chart.border, 100.f, 1.f, 0.1f);

		PropertyVector2(ctx, "padding", chart.padding, 0.f, 1000.f, 1.f, 1.f);

		nk_layout_row_dynamic(ctx, 22, 1);
		nk_label(ctx, "color_factor", NK_TEXT_LEFT);
		nk_property_float(ctx, "#value:", 0.f, &chart.color_factor, 1.f, 0.01f, 0.01f);
		nk_label(ctx, "disabled_factor", NK_TEXT_LEFT);
		nk_property_float(ctx, "#value:", 0.f, &chart.disabled_factor, 1.f, 0.01f, 0.01f);

		nk_checkbox_label(ctx, "show_markers", &chart.show_markers);

		nk_tree_pop(ctx);
	}
}
void NKStyle::CustomComponentsEditor(nk_context* ctx, NuklearUI* pManager, ComponentScrollbar& tpi, struct nk_style_scrollbar& scrollbar, int id)
{
	int tree_index = 100;
	int tree_index_in = 200;
	int tree_index_in_in = 300;

	if (nk_tree_push_id(ctx, NK_TREE_NODE, "background", NK_MINIMIZED, id + tree_index++)) {

		if (nk_tree_push_id(ctx, NK_TREE_NODE, "normal", NK_MINIMIZED, id + tree_index_in++)) {
			ItemEditor(ctx, pManager, tpi.normal);
			nk_tree_pop(ctx);
		}
		if (nk_tree_push_id(ctx, NK_TREE_NODE, "hover", NK_MINIMIZED, id + tree_index_in++)) {
			ItemEditor(ctx, pManager, tpi.hover);
			nk_tree_pop(ctx);
		}
		if (nk_tree_push_id(ctx, NK_TREE_NODE, "active", NK_MINIMIZED, id + tree_index_in++)) {
			ItemEditor(ctx, pManager, tpi.active);
			nk_tree_pop(ctx);
		}
		if (nk_tree_push_id(ctx, NK_TREE_NODE, "border_color", NK_MINIMIZED, id + tree_index_in++)) {
			ColorPicker(ctx, scrollbar.border_color);
			nk_tree_pop(ctx);
		}
		nk_tree_pop(ctx);
	}

	if (nk_tree_push_id(ctx, NK_TREE_NODE, "cursor", NK_MINIMIZED, id + tree_index++)) {

		if (nk_tree_push_id(ctx, NK_TREE_NODE, "normal", NK_MINIMIZED, id + tree_index_in++)) {
			ItemEditor(ctx, pManager, tpi.cursor_normal);
			nk_tree_pop(ctx);
		}
		if (nk_tree_push_id(ctx, NK_TREE_NODE, "hover", NK_MINIMIZED, id + tree_index_in++)) {
			ItemEditor(ctx, pManager, tpi.cursor_hover);
			nk_tree_pop(ctx);
		}
		if (nk_tree_push_id(ctx, NK_TREE_NODE, "active", NK_MINIMIZED, id + tree_index_in++)) {
			ItemEditor(ctx, pManager, tpi.cursor_active);
			nk_tree_pop(ctx);
		}
		if (nk_tree_push_id(ctx, NK_TREE_NODE, "border_color", NK_MINIMIZED, id + tree_index_in++)) {
			ColorPicker(ctx, scrollbar.cursor_border_color);
			nk_tree_pop(ctx);
		}
		nk_tree_pop(ctx);
	}

	if (nk_tree_push_id(ctx, NK_TREE_NODE, "properties", NK_MINIMIZED, id + tree_index++)) {
		nk_layout_row_dynamic(ctx, 22, 1);
		nk_label(ctx, "rounding", NK_TEXT_LEFT);
		nk_property_float(ctx, "#value:", 0.f, &scrollbar.rounding, 100.f, 1.f, 0.1f);
		nk_label(ctx, "border", NK_TEXT_LEFT);
		nk_property_float(ctx, "#value:", 0.f, &scrollbar.border, 100.f, 1.f, 0.1f);
		nk_label(ctx, "border_cursor", NK_TEXT_LEFT);
		nk_property_float(ctx, "#value:", 0.f, &scrollbar.border_cursor, 100.f, 1.f, 0.1f);
		nk_label(ctx, "rounding_cursor", NK_TEXT_LEFT);
		nk_property_float(ctx, "#value:", 0.f, &scrollbar.rounding_cursor, 100.f, 1.f, 0.1f);

		PropertyVector2(ctx, "padding", scrollbar.padding, 0.f, 1000.f, 1.f, 1.f);

		nk_layout_row_dynamic(ctx, 22, 1);
		nk_label(ctx, "color_factor", NK_TEXT_LEFT);
		nk_property_float(ctx, "#value:", 0.f, &scrollbar.color_factor, 1.f, 0.01f, 0.01f);
		nk_label(ctx, "disabled_factor", NK_TEXT_LEFT);
		nk_property_float(ctx, "#value:", 0.f, &scrollbar.disabled_factor, 1.f, 0.01f, 0.01f);
		nk_tree_pop(ctx);
	}
	if (nk_tree_push_id(ctx, NK_TREE_NODE, "optional buttons", NK_MINIMIZED, id + tree_index++)) {

		nk_checkbox_label(ctx, "Show_buttons", &scrollbar.show_buttons);

		if (nk_tree_push_id(ctx, NK_TREE_NODE, "inc_button", NK_MINIMIZED, id + tree_index_in++)) {
			if (nk_tree_push_id(ctx, NK_TREE_NODE, "symbol", NK_MINIMIZED, id + tree_index_in_in++)) {
				if (nk_option_label(ctx, "NONE", scrollbar.inc_symbol == NK_SYMBOL_NONE)) 			scrollbar.inc_symbol = NK_SYMBOL_NONE;
				if (nk_option_label(ctx, "X", scrollbar.inc_symbol == NK_SYMBOL_X)) 				scrollbar.inc_symbol = NK_SYMBOL_X;
				if (nk_option_label(ctx, "UNDERSCORE", scrollbar.inc_symbol == NK_SYMBOL_UNDERSCORE)) 		scrollbar.inc_symbol = NK_SYMBOL_UNDERSCORE;
				if (nk_option_label(ctx, "CIRCLE_SOLID", scrollbar.inc_symbol == NK_SYMBOL_CIRCLE_SOLID)) 	scrollbar.inc_symbol = NK_SYMBOL_CIRCLE_SOLID;
				if (nk_option_label(ctx, "CIRCLE_OUTLINE", scrollbar.inc_symbol == NK_SYMBOL_CIRCLE_OUTLINE))	scrollbar.inc_symbol = NK_SYMBOL_CIRCLE_OUTLINE;
				if (nk_option_label(ctx, "RECT_SOLID", scrollbar.inc_symbol == NK_SYMBOL_RECT_SOLID)) 		scrollbar.inc_symbol = NK_SYMBOL_RECT_SOLID;
				if (nk_option_label(ctx, "RECT_OUTLINE", scrollbar.inc_symbol == NK_SYMBOL_RECT_OUTLINE)) 	scrollbar.inc_symbol = NK_SYMBOL_RECT_OUTLINE;
				if (nk_option_label(ctx, "TRIANGLE_UP", scrollbar.inc_symbol == NK_SYMBOL_TRIANGLE_UP)) 	scrollbar.inc_symbol = NK_SYMBOL_TRIANGLE_UP;
				if (nk_option_label(ctx, "TRIANGLE_DOWN", scrollbar.inc_symbol == NK_SYMBOL_TRIANGLE_DOWN)) 	scrollbar.inc_symbol = NK_SYMBOL_TRIANGLE_DOWN;
				if (nk_option_label(ctx, "TRIANGLE_LEFT", scrollbar.inc_symbol == NK_SYMBOL_TRIANGLE_LEFT)) 	scrollbar.inc_symbol = NK_SYMBOL_TRIANGLE_LEFT;
				if (nk_option_label(ctx, "TRIANGLE_RIGHT", scrollbar.inc_symbol == NK_SYMBOL_TRIANGLE_RIGHT))	scrollbar.inc_symbol = NK_SYMBOL_TRIANGLE_RIGHT;
				if (nk_option_label(ctx, "PLUS", scrollbar.inc_symbol == NK_SYMBOL_PLUS)) 			scrollbar.inc_symbol = NK_SYMBOL_PLUS;
				if (nk_option_label(ctx, "MINUS", scrollbar.inc_symbol == NK_SYMBOL_MINUS)) 			scrollbar.inc_symbol = NK_SYMBOL_MINUS;
				if (nk_option_label(ctx, "MAX", scrollbar.inc_symbol == NK_SYMBOL_MAX)) 			scrollbar.inc_symbol = NK_SYMBOL_MAX;
				nk_tree_pop(ctx);
			}
			CustomComponentsEditor(ctx, pManager, tpi.inc_button, scrollbar.inc_button, id + tree_index++);
			nk_tree_pop(ctx);
		}

		if (nk_tree_push_id(ctx, NK_TREE_NODE, "dec_button", NK_MINIMIZED, id + tree_index_in++)) {
			if (nk_tree_push_id(ctx, NK_TREE_NODE, "symbol", NK_MINIMIZED, id + tree_index_in_in++)) {
				if (nk_option_label(ctx, "NONE", scrollbar.dec_symbol == NK_SYMBOL_NONE)) 			scrollbar.dec_symbol = NK_SYMBOL_NONE;
				if (nk_option_label(ctx, "X", scrollbar.dec_symbol == NK_SYMBOL_X)) 				scrollbar.dec_symbol = NK_SYMBOL_X;
				if (nk_option_label(ctx, "UNDERSCORE", scrollbar.dec_symbol == NK_SYMBOL_UNDERSCORE)) 		scrollbar.dec_symbol = NK_SYMBOL_UNDERSCORE;
				if (nk_option_label(ctx, "CIRCLE_SOLID", scrollbar.dec_symbol == NK_SYMBOL_CIRCLE_SOLID)) 	scrollbar.dec_symbol = NK_SYMBOL_CIRCLE_SOLID;
				if (nk_option_label(ctx, "CIRCLE_OUTLINE", scrollbar.dec_symbol == NK_SYMBOL_CIRCLE_OUTLINE))	scrollbar.dec_symbol = NK_SYMBOL_CIRCLE_OUTLINE;
				if (nk_option_label(ctx, "RECT_SOLID", scrollbar.dec_symbol == NK_SYMBOL_RECT_SOLID)) 		scrollbar.dec_symbol = NK_SYMBOL_RECT_SOLID;
				if (nk_option_label(ctx, "RECT_OUTLINE", scrollbar.dec_symbol == NK_SYMBOL_RECT_OUTLINE)) 	scrollbar.dec_symbol = NK_SYMBOL_RECT_OUTLINE;
				if (nk_option_label(ctx, "TRIANGLE_UP", scrollbar.dec_symbol == NK_SYMBOL_TRIANGLE_UP)) 	scrollbar.dec_symbol = NK_SYMBOL_TRIANGLE_UP;
				if (nk_option_label(ctx, "TRIANGLE_DOWN", scrollbar.dec_symbol == NK_SYMBOL_TRIANGLE_DOWN)) 	scrollbar.dec_symbol = NK_SYMBOL_TRIANGLE_DOWN;
				if (nk_option_label(ctx, "TRIANGLE_LEFT", scrollbar.dec_symbol == NK_SYMBOL_TRIANGLE_LEFT)) 	scrollbar.dec_symbol = NK_SYMBOL_TRIANGLE_LEFT;
				if (nk_option_label(ctx, "TRIANGLE_RIGHT", scrollbar.dec_symbol == NK_SYMBOL_TRIANGLE_RIGHT))	scrollbar.dec_symbol = NK_SYMBOL_TRIANGLE_RIGHT;
				if (nk_option_label(ctx, "PLUS", scrollbar.dec_symbol == NK_SYMBOL_PLUS)) 			scrollbar.dec_symbol = NK_SYMBOL_PLUS;
				if (nk_option_label(ctx, "MINUS", scrollbar.dec_symbol == NK_SYMBOL_MINUS)) 			scrollbar.dec_symbol = NK_SYMBOL_MINUS;
				if (nk_option_label(ctx, "MAX", scrollbar.dec_symbol == NK_SYMBOL_MAX)) 			scrollbar.dec_symbol = NK_SYMBOL_MAX;
				nk_tree_pop(ctx);
			}
			CustomComponentsEditor(ctx, pManager, tpi.dec_button, scrollbar.dec_button, id + tree_index_in_in++);
			nk_tree_pop(ctx);
		}
		nk_tree_pop(ctx);
	}
}
void NKStyle::CustomComponentsEditor(nk_context* ctx, NuklearUI* pManager, ComponentTab& tpi, struct nk_style_tab& tab, int id)
{
	int tree_index = 100;
	int tree_index_in = 200;
	int tree_index_in_in = 300;

	if (nk_tree_push_id(ctx, NK_TREE_NODE, "bg_color", NK_MINIMIZED, id + tree_index++)) {

		if (nk_tree_push_id(ctx, NK_TREE_NODE, "background", NK_MINIMIZED, id + tree_index_in++)) {
			ItemEditor(ctx, pManager, tpi.background);
			nk_tree_pop(ctx);
		}
		if (nk_tree_push_id(ctx, NK_TREE_NODE, "border", NK_MINIMIZED, id + tree_index_in++)) {
			ColorPicker(ctx, tab.border_color);
			nk_tree_pop(ctx);
		}
		if (nk_tree_push_id(ctx, NK_TREE_NODE, "text", NK_MINIMIZED, id + tree_index_in++)) {
			ColorPicker(ctx, tab.text);
			nk_tree_pop(ctx);
		}
		nk_tree_pop(ctx);
	}
	if (nk_tree_push(ctx, NK_TREE_NODE, "button", NK_MINIMIZED)) {
		if (nk_tree_push(ctx, NK_TREE_NODE, "sym_minimize", NK_MINIMIZED)) {
			if (nk_option_label(ctx, "NONE", tab.sym_minimize == NK_SYMBOL_NONE)) 				tab.sym_minimize = NK_SYMBOL_NONE;
			if (nk_option_label(ctx, "X", tab.sym_minimize == NK_SYMBOL_X)) 					tab.sym_minimize = NK_SYMBOL_X;
			if (nk_option_label(ctx, "UNDERSCORE", tab.sym_minimize == NK_SYMBOL_UNDERSCORE)) 			tab.sym_minimize = NK_SYMBOL_UNDERSCORE;
			if (nk_option_label(ctx, "CIRCLE_SOLID", tab.sym_minimize == NK_SYMBOL_CIRCLE_SOLID)) 		tab.sym_minimize = NK_SYMBOL_CIRCLE_SOLID;
			if (nk_option_label(ctx, "CIRCLE_OUTLINE", tab.sym_minimize == NK_SYMBOL_CIRCLE_OUTLINE))		tab.sym_minimize = NK_SYMBOL_CIRCLE_OUTLINE;
			if (nk_option_label(ctx, "RECT_SOLID", tab.sym_minimize == NK_SYMBOL_RECT_SOLID)) 			tab.sym_minimize = NK_SYMBOL_RECT_SOLID;
			if (nk_option_label(ctx, "RECT_OUTLINE", tab.sym_minimize == NK_SYMBOL_RECT_OUTLINE)) 		tab.sym_minimize = NK_SYMBOL_RECT_OUTLINE;
			if (nk_option_label(ctx, "TRIANGLE_UP", tab.sym_minimize == NK_SYMBOL_TRIANGLE_UP)) 		tab.sym_minimize = NK_SYMBOL_TRIANGLE_UP;
			if (nk_option_label(ctx, "TRIANGLE_DOWN", tab.sym_minimize == NK_SYMBOL_TRIANGLE_DOWN))		tab.sym_minimize = NK_SYMBOL_TRIANGLE_DOWN;
			if (nk_option_label(ctx, "TRIANGLE_LEFT", tab.sym_minimize == NK_SYMBOL_TRIANGLE_LEFT))		tab.sym_minimize = NK_SYMBOL_TRIANGLE_LEFT;
			if (nk_option_label(ctx, "TRIANGLE_RIGHT", tab.sym_minimize == NK_SYMBOL_TRIANGLE_RIGHT))		tab.sym_minimize = NK_SYMBOL_TRIANGLE_RIGHT;
			if (nk_option_label(ctx, "PLUS", tab.sym_minimize == NK_SYMBOL_PLUS)) 				tab.sym_minimize = NK_SYMBOL_PLUS;
			if (nk_option_label(ctx, "MINUS", tab.sym_minimize == NK_SYMBOL_MINUS)) 				tab.sym_minimize = NK_SYMBOL_MINUS;
			if (nk_option_label(ctx, "MAX", tab.sym_minimize == NK_SYMBOL_MAX)) 				tab.sym_minimize = NK_SYMBOL_MAX;
			nk_tree_pop(ctx);
		}
		CustomComponentsEditor(ctx, pManager, tpi.tab_minimize_button, tab.tab_minimize_button, id + tree_index_in_in + 100);
		CustomComponentsEditor(ctx, pManager, tpi.node_minimize_button, tab.node_minimize_button, id + tree_index_in_in + 200);


		if (nk_tree_push(ctx, NK_TREE_NODE, "sym_maximize", NK_MINIMIZED)) {
			if (nk_option_label(ctx, "NONE", tab.sym_maximize == NK_SYMBOL_NONE)) 				tab.sym_maximize = NK_SYMBOL_NONE;
			if (nk_option_label(ctx, "X", tab.sym_maximize == NK_SYMBOL_X)) 					tab.sym_maximize = NK_SYMBOL_X;
			if (nk_option_label(ctx, "UNDERSCORE", tab.sym_maximize == NK_SYMBOL_UNDERSCORE)) 			tab.sym_maximize = NK_SYMBOL_UNDERSCORE;
			if (nk_option_label(ctx, "CIRCLE_SOLID", tab.sym_maximize == NK_SYMBOL_CIRCLE_SOLID)) 		tab.sym_maximize = NK_SYMBOL_CIRCLE_SOLID;
			if (nk_option_label(ctx, "CIRCLE_OUTLINE", tab.sym_maximize == NK_SYMBOL_CIRCLE_OUTLINE))		tab.sym_maximize = NK_SYMBOL_CIRCLE_OUTLINE;
			if (nk_option_label(ctx, "RECT_SOLID", tab.sym_maximize == NK_SYMBOL_RECT_SOLID)) 			tab.sym_maximize = NK_SYMBOL_RECT_SOLID;
			if (nk_option_label(ctx, "RECT_OUTLINE", tab.sym_maximize == NK_SYMBOL_RECT_OUTLINE)) 		tab.sym_maximize = NK_SYMBOL_RECT_OUTLINE;
			if (nk_option_label(ctx, "TRIANGLE_UP", tab.sym_maximize == NK_SYMBOL_TRIANGLE_UP)) 		tab.sym_maximize = NK_SYMBOL_TRIANGLE_UP;
			if (nk_option_label(ctx, "TRIANGLE_DOWN", tab.sym_maximize == NK_SYMBOL_TRIANGLE_DOWN))		tab.sym_maximize = NK_SYMBOL_TRIANGLE_DOWN;
			if (nk_option_label(ctx, "TRIANGLE_LEFT", tab.sym_maximize == NK_SYMBOL_TRIANGLE_LEFT))		tab.sym_maximize = NK_SYMBOL_TRIANGLE_LEFT;
			if (nk_option_label(ctx, "TRIANGLE_RIGHT", tab.sym_maximize == NK_SYMBOL_TRIANGLE_RIGHT))		tab.sym_maximize = NK_SYMBOL_TRIANGLE_RIGHT;
			if (nk_option_label(ctx, "PLUS", tab.sym_maximize == NK_SYMBOL_PLUS)) 				tab.sym_maximize = NK_SYMBOL_PLUS;
			if (nk_option_label(ctx, "MINUS", tab.sym_maximize == NK_SYMBOL_MINUS)) 				tab.sym_maximize = NK_SYMBOL_MINUS;
			if (nk_option_label(ctx, "MAX", tab.sym_maximize == NK_SYMBOL_MAX)) 				tab.sym_maximize = NK_SYMBOL_MAX;
			nk_tree_pop(ctx);
		}
		CustomComponentsEditor(ctx, pManager, tpi.node_maximize_button, tab.node_maximize_button, id + tree_index_in_in + 300);
		CustomComponentsEditor(ctx, pManager, tpi.node_minimize_button, tab.node_minimize_button, id + tree_index_in_in + 400);
		nk_tree_pop(ctx);
	}

	if (nk_tree_push_id(ctx, NK_TREE_NODE, "properties", NK_MINIMIZED, id + tree_index++)) {
		nk_layout_row_dynamic(ctx, 22, 1);
		nk_label(ctx, "rounding", NK_TEXT_LEFT);
		nk_property_float(ctx, "#value:", 0.f, &tab.rounding, 100.f, 1.f, 0.1f);
		nk_label(ctx, "border", NK_TEXT_LEFT);
		nk_property_float(ctx, "#value:", 0.f, &tab.border, 100.f, 1.f, 0.1f);
		nk_label(ctx, "indent", NK_TEXT_LEFT);
		nk_property_float(ctx, "#value:", 0.f, &tab.indent, 100.f, 1.f, 0.1f);

		PropertyVector2(ctx, "padding", tab.padding, 0.f, 1000.f, 1.f, 1.f);
		PropertyVector2(ctx, "spacing", tab.spacing, 0.f, 1000.f, 1.f, 1.f);

		nk_layout_row_dynamic(ctx, 22, 1);
		nk_label(ctx, "color_factor", NK_TEXT_LEFT);
		nk_property_float(ctx, "#value:", 0.f, &tab.color_factor, 1.f, 0.01f, 0.01f);
		nk_label(ctx, "disabled_factor", NK_TEXT_LEFT);
		nk_property_float(ctx, "#value:", 0.f, &tab.disabled_factor, 1.f, 0.01f, 0.01f);

		nk_tree_pop(ctx);
	}
}
void NKStyle::CustomComponentsEditor(nk_context* ctx, NuklearUI* pManager, ComponentCombo& tpi, struct nk_style_combo& combo, int id)
{
	int tree_index = 100;
	int tree_index_in = 200;
	int tree_index_in_in = 300;

	if (nk_tree_push_id(ctx, NK_TREE_NODE, "background", NK_MINIMIZED, id + tree_index++)) {

		if (nk_tree_push_id(ctx, NK_TREE_NODE, "normal", NK_MINIMIZED, id + tree_index_in++)) {
			ItemEditor(ctx, pManager, tpi.normal);
			nk_tree_pop(ctx);
		}
		if (nk_tree_push_id(ctx, NK_TREE_NODE, "hover", NK_MINIMIZED, id + tree_index_in++)) {
			ItemEditor(ctx, pManager, tpi.hover);
			nk_tree_pop(ctx);
		}
		if (nk_tree_push_id(ctx, NK_TREE_NODE, "active", NK_MINIMIZED, id + tree_index_in++)) {
			ItemEditor(ctx, pManager, tpi.active);
			nk_tree_pop(ctx);
		}
		if (nk_tree_push_id(ctx, NK_TREE_NODE, "border", NK_MINIMIZED, id + tree_index_in++)) {
			ColorPicker(ctx, combo.border_color);
			nk_tree_pop(ctx);
		}
		nk_tree_pop(ctx);
	}

	if (nk_tree_push_id(ctx, NK_TREE_NODE, "label", NK_MINIMIZED, id + tree_index++)) {
		if (nk_tree_push_id(ctx, NK_TREE_NODE, "normal", NK_MINIMIZED, id + tree_index_in++)) {
			ColorPicker(ctx, combo.label_normal);
			nk_tree_pop(ctx);
		}
		if (nk_tree_push_id(ctx, NK_TREE_NODE, "hover", NK_MINIMIZED, id + tree_index_in++)) {
			ColorPicker(ctx, combo.label_hover);
			nk_tree_pop(ctx);
		}
		if (nk_tree_push_id(ctx, NK_TREE_NODE, "active", NK_MINIMIZED, id + tree_index_in++)) {
			ColorPicker(ctx, combo.label_active);
			nk_tree_pop(ctx);
		}
		nk_tree_pop(ctx);
	}

	if (nk_tree_push_id(ctx, NK_TREE_NODE, "symbol", NK_MINIMIZED, id + tree_index++)) {
		if (nk_tree_push_id(ctx, NK_TREE_NODE, "normal", NK_MINIMIZED, id + tree_index_in++)) {
			ColorPicker(ctx, combo.symbol_normal);
			nk_tree_pop(ctx);
		}
		if (nk_tree_push_id(ctx, NK_TREE_NODE, "hover", NK_MINIMIZED, id + tree_index_in++)) {
			ColorPicker(ctx, combo.symbol_hover);
			nk_tree_pop(ctx);
		}
		if (nk_tree_push_id(ctx, NK_TREE_NODE, "active", NK_MINIMIZED, id + tree_index_in++)) {
			ColorPicker(ctx, combo.symbol_active);
			nk_tree_pop(ctx);
		}
		nk_tree_pop(ctx);
	}

	if (nk_tree_push(ctx, NK_TREE_NODE, "button", NK_MINIMIZED)) {

		CustomComponentsEditor(ctx, pManager, tpi.button, combo.button, id + tree_index_in_in + 100);

		if (nk_tree_push(ctx, NK_TREE_NODE, "sym_normal", NK_MINIMIZED)) {
			if (nk_option_label(ctx, "NONE", combo.sym_normal == NK_SYMBOL_NONE)) 				combo.sym_normal = NK_SYMBOL_NONE;
			if (nk_option_label(ctx, "X", combo.sym_normal == NK_SYMBOL_X)) 					combo.sym_normal = NK_SYMBOL_X;
			if (nk_option_label(ctx, "UNDERSCORE", combo.sym_normal == NK_SYMBOL_UNDERSCORE)) 			combo.sym_normal = NK_SYMBOL_UNDERSCORE;
			if (nk_option_label(ctx, "CIRCLE_SOLID", combo.sym_normal == NK_SYMBOL_CIRCLE_SOLID)) 		combo.sym_normal = NK_SYMBOL_CIRCLE_SOLID;
			if (nk_option_label(ctx, "CIRCLE_OUTLINE", combo.sym_normal == NK_SYMBOL_CIRCLE_OUTLINE))		combo.sym_normal = NK_SYMBOL_CIRCLE_OUTLINE;
			if (nk_option_label(ctx, "RECT_SOLID", combo.sym_normal == NK_SYMBOL_RECT_SOLID)) 			combo.sym_normal = NK_SYMBOL_RECT_SOLID;
			if (nk_option_label(ctx, "RECT_OUTLINE", combo.sym_normal == NK_SYMBOL_RECT_OUTLINE)) 		combo.sym_normal = NK_SYMBOL_RECT_OUTLINE;
			if (nk_option_label(ctx, "TRIANGLE_UP", combo.sym_normal == NK_SYMBOL_TRIANGLE_UP)) 		combo.sym_normal = NK_SYMBOL_TRIANGLE_UP;
			if (nk_option_label(ctx, "TRIANGLE_DOWN", combo.sym_normal == NK_SYMBOL_TRIANGLE_DOWN))		combo.sym_normal = NK_SYMBOL_TRIANGLE_DOWN;
			if (nk_option_label(ctx, "TRIANGLE_LEFT", combo.sym_normal == NK_SYMBOL_TRIANGLE_LEFT))		combo.sym_normal = NK_SYMBOL_TRIANGLE_LEFT;
			if (nk_option_label(ctx, "TRIANGLE_RIGHT", combo.sym_normal == NK_SYMBOL_TRIANGLE_RIGHT))		combo.sym_normal = NK_SYMBOL_TRIANGLE_RIGHT;
			if (nk_option_label(ctx, "PLUS", combo.sym_normal == NK_SYMBOL_PLUS)) 				combo.sym_normal = NK_SYMBOL_PLUS;
			if (nk_option_label(ctx, "MINUS", combo.sym_normal == NK_SYMBOL_MINUS)) 				combo.sym_normal = NK_SYMBOL_MINUS;
			if (nk_option_label(ctx, "MAX", combo.sym_normal == NK_SYMBOL_MAX)) 				combo.sym_normal = NK_SYMBOL_MAX;
			nk_tree_pop(ctx);
		}
		if (nk_tree_push(ctx, NK_TREE_NODE, "sym_hover", NK_MINIMIZED)) {
			if (nk_option_label(ctx, "NONE", combo.sym_hover == NK_SYMBOL_NONE)) 				combo.sym_hover = NK_SYMBOL_NONE;
			if (nk_option_label(ctx, "X", combo.sym_hover == NK_SYMBOL_X)) 					combo.sym_hover = NK_SYMBOL_X;
			if (nk_option_label(ctx, "UNDERSCORE", combo.sym_hover == NK_SYMBOL_UNDERSCORE)) 			combo.sym_hover = NK_SYMBOL_UNDERSCORE;
			if (nk_option_label(ctx, "CIRCLE_SOLID", combo.sym_hover == NK_SYMBOL_CIRCLE_SOLID)) 		combo.sym_hover = NK_SYMBOL_CIRCLE_SOLID;
			if (nk_option_label(ctx, "CIRCLE_OUTLINE", combo.sym_hover == NK_SYMBOL_CIRCLE_OUTLINE))		combo.sym_hover = NK_SYMBOL_CIRCLE_OUTLINE;
			if (nk_option_label(ctx, "RECT_SOLID", combo.sym_hover == NK_SYMBOL_RECT_SOLID)) 			combo.sym_hover = NK_SYMBOL_RECT_SOLID;
			if (nk_option_label(ctx, "RECT_OUTLINE", combo.sym_hover == NK_SYMBOL_RECT_OUTLINE)) 		combo.sym_hover = NK_SYMBOL_RECT_OUTLINE;
			if (nk_option_label(ctx, "TRIANGLE_UP", combo.sym_hover == NK_SYMBOL_TRIANGLE_UP)) 			combo.sym_hover = NK_SYMBOL_TRIANGLE_UP;
			if (nk_option_label(ctx, "TRIANGLE_DOWN", combo.sym_hover == NK_SYMBOL_TRIANGLE_DOWN))		combo.sym_hover = NK_SYMBOL_TRIANGLE_DOWN;
			if (nk_option_label(ctx, "TRIANGLE_LEFT", combo.sym_hover == NK_SYMBOL_TRIANGLE_LEFT))		combo.sym_hover = NK_SYMBOL_TRIANGLE_LEFT;
			if (nk_option_label(ctx, "TRIANGLE_RIGHT", combo.sym_hover == NK_SYMBOL_TRIANGLE_RIGHT))		combo.sym_hover = NK_SYMBOL_TRIANGLE_RIGHT;
			if (nk_option_label(ctx, "PLUS", combo.sym_hover == NK_SYMBOL_PLUS)) 				combo.sym_hover = NK_SYMBOL_PLUS;
			if (nk_option_label(ctx, "MINUS", combo.sym_hover == NK_SYMBOL_MINUS)) 				combo.sym_hover = NK_SYMBOL_MINUS;
			if (nk_option_label(ctx, "MAX", combo.sym_hover == NK_SYMBOL_MAX)) 					combo.sym_hover = NK_SYMBOL_MAX;
			nk_tree_pop(ctx);
		}
		if (nk_tree_push(ctx, NK_TREE_NODE, "sym_active", NK_MINIMIZED)) {
			if (nk_option_label(ctx, "NONE", combo.sym_active == NK_SYMBOL_NONE)) 				combo.sym_active = NK_SYMBOL_NONE;
			if (nk_option_label(ctx, "X", combo.sym_active == NK_SYMBOL_X)) 					combo.sym_active = NK_SYMBOL_X;
			if (nk_option_label(ctx, "UNDERSCORE", combo.sym_active == NK_SYMBOL_UNDERSCORE)) 			combo.sym_active = NK_SYMBOL_UNDERSCORE;
			if (nk_option_label(ctx, "CIRCLE_SOLID", combo.sym_active == NK_SYMBOL_CIRCLE_SOLID)) 		combo.sym_active = NK_SYMBOL_CIRCLE_SOLID;
			if (nk_option_label(ctx, "CIRCLE_OUTLINE", combo.sym_active == NK_SYMBOL_CIRCLE_OUTLINE))		combo.sym_active = NK_SYMBOL_CIRCLE_OUTLINE;
			if (nk_option_label(ctx, "RECT_SOLID", combo.sym_active == NK_SYMBOL_RECT_SOLID)) 			combo.sym_active = NK_SYMBOL_RECT_SOLID;
			if (nk_option_label(ctx, "RECT_OUTLINE", combo.sym_active == NK_SYMBOL_RECT_OUTLINE)) 		combo.sym_active = NK_SYMBOL_RECT_OUTLINE;
			if (nk_option_label(ctx, "TRIANGLE_UP", combo.sym_active == NK_SYMBOL_TRIANGLE_UP)) 		combo.sym_active = NK_SYMBOL_TRIANGLE_UP;
			if (nk_option_label(ctx, "TRIANGLE_DOWN", combo.sym_active == NK_SYMBOL_TRIANGLE_DOWN))		combo.sym_active = NK_SYMBOL_TRIANGLE_DOWN;
			if (nk_option_label(ctx, "TRIANGLE_LEFT", combo.sym_active == NK_SYMBOL_TRIANGLE_LEFT))		combo.sym_active = NK_SYMBOL_TRIANGLE_LEFT;
			if (nk_option_label(ctx, "TRIANGLE_RIGHT", combo.sym_active == NK_SYMBOL_TRIANGLE_RIGHT))		combo.sym_active = NK_SYMBOL_TRIANGLE_RIGHT;
			if (nk_option_label(ctx, "PLUS", combo.sym_active == NK_SYMBOL_PLUS)) 				combo.sym_active = NK_SYMBOL_PLUS;
			if (nk_option_label(ctx, "MINUS", combo.sym_active == NK_SYMBOL_MINUS)) 				combo.sym_active = NK_SYMBOL_MINUS;
			if (nk_option_label(ctx, "MAX", combo.sym_active == NK_SYMBOL_MAX)) 				combo.sym_active = NK_SYMBOL_MAX;
			nk_tree_pop(ctx);
		}
		nk_tree_pop(ctx);
	}

	if (nk_tree_push_id(ctx, NK_TREE_NODE, "properties", NK_MINIMIZED, id + tree_index++)) {
		nk_layout_row_dynamic(ctx, 22, 1);
		nk_label(ctx, "rounding", NK_TEXT_LEFT);
		nk_property_float(ctx, "#value:", 0.f, &combo.rounding, 100.f, 1.f, 0.1f);
		nk_label(ctx, "border", NK_TEXT_LEFT);
		nk_property_float(ctx, "#value:", 0.f, &combo.border, 100.f, 1.f, 0.1f);

		PropertyVector2(ctx, "content_padding", combo.content_padding, 0.f, 1000.f, 1.f, 1.f);
		PropertyVector2(ctx, "button_padding", combo.button_padding, 0.f, 1000.f, 1.f, 1.f);
		PropertyVector2(ctx, "spacing", combo.spacing, 0.f, 1000.f, 1.f, 1.f);

		nk_layout_row_dynamic(ctx, 22, 1);
		nk_label(ctx, "color_factor", NK_TEXT_LEFT);
		nk_property_float(ctx, "#value:", 0.f, &combo.color_factor, 1.f, 0.01f, 0.01f);
		nk_label(ctx, "disabled_factor", NK_TEXT_LEFT);
		nk_property_float(ctx, "#value:", 0.f, &combo.disabled_factor, 1.f, 0.01f, 0.01f);

		nk_tree_pop(ctx);
	}
}


void NKStyle::ItemEditor(nk_context* ctx, NuklearUI* pManager, NKStyleItem& sItem)
{
	nk_layout_row_dynamic(ctx, 30, 1);
	if (nk_option_label(ctx, "color", sItem.option == 0)) sItem.option = 0;
	if (nk_option_label(ctx, "image", sItem.option == 1)) sItem.option = 1;
	if (nk_option_label(ctx, "nine_slice", sItem.option == 2)) sItem.option = 2;

	if (sItem.option == 0) {
		sItem.target->type = NK_STYLE_ITEM_COLOR;
		ColorPicker(ctx, sItem.target->data.color);
	}
	else if (sItem.option == 1 || sItem.option == 2) {
		auto mapSpr = pManager->GetSprMap();
		int size = mapSpr->size();

		nk_layout_row_dynamic(ctx, 22, 1);
		nk_property_int(ctx, "#Index:", 0, &sItem.sprIndex, sItem.sprSize - 1, 1, 1);

		nk_layout_row_dynamic(ctx, 22, 2);
		nk_label(ctx, "Selected:", NK_TEXT_LEFT);
		std::filesystem::path filePath(sItem.imagePath.c_str());
		nk_label(ctx, filePath.filename().string().c_str(), NK_TEXT_RIGHT);
		if (nk_button_label(ctx, "apply"))
		{
			if (sItem.option == 1) {
				struct nk_image img;
				pManager->GetSprite(sItem.imagePath.c_str(), sItem.sprIndex, img, true);
				(*sItem.target) = nk_style_item_image(img);
			}
			else if (sItem.option == 2) {
				struct nk_image img;
				pManager->GetSprite(sItem.imagePath.c_str(), sItem.sprIndex, img, true);
				struct nk_nine_slice nineslice;
				nineslice.img = img;
				nineslice.l = (nk_ushort)sItem.nineslice[0];
				nineslice.t = (nk_ushort)sItem.nineslice[1];
				nineslice.r = (nk_ushort)sItem.nineslice[2];
				nineslice.b = (nk_ushort)sItem.nineslice[3];
				(*sItem.target) = nk_style_item_nine_slice(nineslice);
			}
		}
		if (nk_button_label(ctx, "clear"))
		{
			(*sItem.target) = (*sItem.restore);
		}
		if (sItem.option == 2) {
			nk_label(ctx, "nine_slice", NK_TEXT_LEFT);
			nk_layout_row_dynamic(ctx, 22, 1);
			nk_property_int(ctx, "#Left:", 0, &sItem.nineslice[0], 255, 1, 1);
			nk_property_int(ctx, "#Top:", 0, &sItem.nineslice[1], 255, 1, 1);
			nk_property_int(ctx, "#Right:", 0, &sItem.nineslice[2], 255, 1, 1);
			nk_property_int(ctx, "#Bottom:", 0, &sItem.nineslice[3], 255, 1, 1);
		}

		if (size > 0)
		{

			static char selectedFilename[260] = { 0, };
			float ratio[2] = { 0.7f, 0.3f };
			static char SearchFunction[256] = { 0, };
			static int SearchFunction_Len = 0;
			nk_layout_row(ctx, NK_DYNAMIC, 40, 2, ratio);
			nk_flags searchResult = pManager->IMEInputSystem(ctx, NK_EDIT_FIELD | NK_EDIT_SIG_ENTER, SearchFunction, sizeof(SearchFunction), nk_filter_default, &SearchFunction_Len);

			if (nk_button_label(ctx, "Search") | searchResult & NK_EDIT_COMMITED) {

			}

			nk_layout_row_dynamic(ctx, 300, 1);
			if (nk_group_begin(ctx, "SPR List", NK_WINDOW_TITLE)) {

				float ratio[2] = { 0.8f, 0.2f };
				nk_layout_row(ctx, NK_DYNAMIC, 22, 2, ratio);

				for (auto it = mapSpr->begin(); it != mapSpr->end(); ++it) {
					std::filesystem::path filePath((*it).first.c_str());

					bool bSearch = false;
					bool bSearchResult = true;
					if (strlen(SearchFunction) > 0) {
						bSearch = true;
					}

					if (bSearch) {
						std::wstring word = NuklearUI::utf8ToWstring(filePath.filename().string().c_str());
						std::wstring filter = NuklearUI::utf8ToWstring(SearchFunction);

						// word를 소문자로 변환
						std::transform(word.begin(), word.end(), word.begin(), towlower);
						// filter를 소문자로 변환
						std::transform(filter.begin(), filter.end(), filter.begin(), towlower);

						bSearchResult = word.find(filter) != std::wstring::npos;
					}

					if (!bSearchResult) {
						continue;
					}


					nk_label(ctx, filePath.filename().string().c_str(), NK_TEXT_LEFT);

					if (nk_button_label(ctx, "Load")) {
						sItem.imagePath = (*it).first;
						sItem.sprSize = ((*it).second)->GetSpr()->GetXCount() * ((*it).second)->GetSpr()->GetYCount();
						if (sItem.sprSize <= sItem.sprIndex) {
							sItem.sprIndex = 0;
						}
					}
				}
				nk_group_end(ctx);
			}
		}
	}
}

void NKStyle::ColorPicker(nk_context* ctx, struct nk_color& color)
{
	struct nk_colorf colorf;

	colorf.a = ((float)color.a / 255.0f);
	colorf.r = ((float)color.r / 255.0f);
	colorf.g = ((float)color.g / 255.0f);
	colorf.b = ((float)color.b / 255.0f);

	nk_layout_row_dynamic(ctx, 300, 1);
	colorf = nk_color_picker(ctx, colorf, NK_RGBA);

	nk_layout_row_dynamic(ctx, 22, 1);
	nk_property_float(ctx, "#r:", 0.f, &colorf.r, 255, 0.01f, 0.01f);
	nk_property_float(ctx, "#g:", 0.f, &colorf.g, 255, 0.01f, 0.01f);
	nk_property_float(ctx, "#b:", 0.f, &colorf.b, 255, 0.01f, 0.01f);
	nk_property_float(ctx, "#a:", 0.f, &colorf.a, 255, 0.01f, 0.01f);

	color.a = ((nk_byte)(colorf.a * 255.0f));
	color.r = ((nk_byte)(colorf.r * 255.0f));
	color.g = ((nk_byte)(colorf.g * 255.0f));
	color.b = ((nk_byte)(colorf.b * 255.0f));
}
