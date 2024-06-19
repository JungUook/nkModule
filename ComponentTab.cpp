#include "pch.h"
#include "ComponentTab.h"
#include "NuklearUI.h"

ComponentTab::ComponentTab()
{
	m_pTarget			  = nullptr;
	m_pRestore			  = nullptr;

	m_pBackground		  = nullptr;

	m_pTabMaximizeButton  = nullptr;
	m_pTabMinimizeButton  = nullptr;
	m_pNodeMaximizeButton = nullptr;
	m_pNodeMinimizeButton = nullptr;

}

ComponentTab::ComponentTab(nk_style_tab* pTarget, nk_style_tab* pRestore)
{
	m_pTarget = pTarget;
	m_pRestore = pRestore;

	m_pBackground = new NKStyleItem(&pTarget->background, &pRestore->background);

	m_pTabMaximizeButton = new ComponentButton(&pTarget->tab_maximize_button, &pRestore->tab_maximize_button);
	m_pTabMinimizeButton = new ComponentButton(&pTarget->tab_minimize_button, &pRestore->tab_minimize_button);
	m_pNodeMaximizeButton = new ComponentButton(&pTarget->node_maximize_button, &pRestore->node_maximize_button);
	m_pNodeMinimizeButton = new ComponentButton(&pTarget->node_minimize_button, &pRestore->node_minimize_button);
}

ComponentTab::ComponentTab(const ComponentTab& other)
{
	m_pTarget = other.m_pTarget;
	m_pRestore = other.m_pRestore;

	m_pBackground 		  = new NKStyleItem(*other.m_pBackground);

	m_pTabMaximizeButton  = new ComponentButton(*other.m_pTabMaximizeButton);
	m_pTabMinimizeButton  = new ComponentButton(*other.m_pTabMinimizeButton);
	m_pNodeMaximizeButton = new ComponentButton(*other.m_pNodeMaximizeButton);
	m_pNodeMinimizeButton = new ComponentButton(*other.m_pNodeMinimizeButton);

}

ComponentTab::~ComponentTab()
{
	delete m_pBackground;

	delete m_pTabMaximizeButton;
	delete m_pTabMinimizeButton;
	delete m_pNodeMaximizeButton;
	delete m_pNodeMinimizeButton;

	m_pBackground = nullptr;

	m_pTabMaximizeButton = nullptr;
	m_pTabMinimizeButton = nullptr;
	m_pNodeMaximizeButton = nullptr;
	m_pNodeMinimizeButton = nullptr;

}

void ComponentTab::CustomComponentsEditor(nk_context* ctx, NuklearUI* pManager)
{
	int tree_index = 100;
	int tree_index_in = 200;
	int tree_index_in_in = 300;
	intptr_t id = reinterpret_cast<intptr_t>(this);

	if (nk_tree_push_id(ctx, NK_TREE_NODE, "bg_color", NK_MINIMIZED, id + tree_index++)) {

		if (nk_tree_push_id(ctx, NK_TREE_NODE, "background", NK_MINIMIZED, id + tree_index_in++)) {
			m_pBackground->ItemEditor(ctx, pManager);
			nk_tree_pop(ctx);
		}
		if (nk_tree_push_id(ctx, NK_TREE_NODE, "border", NK_MINIMIZED, id + tree_index_in++)) {
			ColorPicker(ctx, m_pTarget->border_color);
			nk_tree_pop(ctx);
		}
		if (nk_tree_push_id(ctx, NK_TREE_NODE, "text", NK_MINIMIZED, id + tree_index_in++)) {
			ColorPicker(ctx, m_pTarget->text);
			nk_tree_pop(ctx);
		}
		nk_tree_pop(ctx);
	}
	if (nk_tree_push(ctx, NK_TREE_NODE, "button", NK_MINIMIZED)) {
		if (nk_tree_push(ctx, NK_TREE_NODE, "sym_minimize", NK_MINIMIZED)) {
			if (nk_option_label(ctx, "NONE", m_pTarget->sym_minimize == NK_SYMBOL_NONE)) 				m_pTarget->sym_minimize = NK_SYMBOL_NONE;
			if (nk_option_label(ctx, "X", m_pTarget->sym_minimize == NK_SYMBOL_X)) 					m_pTarget->sym_minimize = NK_SYMBOL_X;
			if (nk_option_label(ctx, "UNDERSCORE", m_pTarget->sym_minimize == NK_SYMBOL_UNDERSCORE)) 			m_pTarget->sym_minimize = NK_SYMBOL_UNDERSCORE;
			if (nk_option_label(ctx, "CIRCLE_SOLID", m_pTarget->sym_minimize == NK_SYMBOL_CIRCLE_SOLID)) 		m_pTarget->sym_minimize = NK_SYMBOL_CIRCLE_SOLID;
			if (nk_option_label(ctx, "CIRCLE_OUTLINE", m_pTarget->sym_minimize == NK_SYMBOL_CIRCLE_OUTLINE))		m_pTarget->sym_minimize = NK_SYMBOL_CIRCLE_OUTLINE;
			if (nk_option_label(ctx, "RECT_SOLID", m_pTarget->sym_minimize == NK_SYMBOL_RECT_SOLID)) 			m_pTarget->sym_minimize = NK_SYMBOL_RECT_SOLID;
			if (nk_option_label(ctx, "RECT_OUTLINE", m_pTarget->sym_minimize == NK_SYMBOL_RECT_OUTLINE)) 		m_pTarget->sym_minimize = NK_SYMBOL_RECT_OUTLINE;
			if (nk_option_label(ctx, "TRIANGLE_UP", m_pTarget->sym_minimize == NK_SYMBOL_TRIANGLE_UP)) 		m_pTarget->sym_minimize = NK_SYMBOL_TRIANGLE_UP;
			if (nk_option_label(ctx, "TRIANGLE_DOWN", m_pTarget->sym_minimize == NK_SYMBOL_TRIANGLE_DOWN))		m_pTarget->sym_minimize = NK_SYMBOL_TRIANGLE_DOWN;
			if (nk_option_label(ctx, "TRIANGLE_LEFT", m_pTarget->sym_minimize == NK_SYMBOL_TRIANGLE_LEFT))		m_pTarget->sym_minimize = NK_SYMBOL_TRIANGLE_LEFT;
			if (nk_option_label(ctx, "TRIANGLE_RIGHT", m_pTarget->sym_minimize == NK_SYMBOL_TRIANGLE_RIGHT))		m_pTarget->sym_minimize = NK_SYMBOL_TRIANGLE_RIGHT;
			if (nk_option_label(ctx, "PLUS", m_pTarget->sym_minimize == NK_SYMBOL_PLUS)) 				m_pTarget->sym_minimize = NK_SYMBOL_PLUS;
			if (nk_option_label(ctx, "MINUS", m_pTarget->sym_minimize == NK_SYMBOL_MINUS)) 				m_pTarget->sym_minimize = NK_SYMBOL_MINUS;
			if (nk_option_label(ctx, "MAX", m_pTarget->sym_minimize == NK_SYMBOL_MAX)) 				m_pTarget->sym_minimize = NK_SYMBOL_MAX;
			nk_tree_pop(ctx);
		}
		m_pTabMaximizeButton->CustomComponentsEditor(ctx, pManager);
		m_pTabMinimizeButton->CustomComponentsEditor(ctx, pManager);


		if (nk_tree_push(ctx, NK_TREE_NODE, "sym_maximize", NK_MINIMIZED)) {
			if (nk_option_label(ctx, "NONE", m_pTarget->sym_maximize == NK_SYMBOL_NONE)) 				m_pTarget->sym_maximize = NK_SYMBOL_NONE;
			if (nk_option_label(ctx, "X", m_pTarget->sym_maximize == NK_SYMBOL_X)) 					m_pTarget->sym_maximize = NK_SYMBOL_X;
			if (nk_option_label(ctx, "UNDERSCORE", m_pTarget->sym_maximize == NK_SYMBOL_UNDERSCORE)) 			m_pTarget->sym_maximize = NK_SYMBOL_UNDERSCORE;
			if (nk_option_label(ctx, "CIRCLE_SOLID", m_pTarget->sym_maximize == NK_SYMBOL_CIRCLE_SOLID)) 		m_pTarget->sym_maximize = NK_SYMBOL_CIRCLE_SOLID;
			if (nk_option_label(ctx, "CIRCLE_OUTLINE", m_pTarget->sym_maximize == NK_SYMBOL_CIRCLE_OUTLINE))		m_pTarget->sym_maximize = NK_SYMBOL_CIRCLE_OUTLINE;
			if (nk_option_label(ctx, "RECT_SOLID", m_pTarget->sym_maximize == NK_SYMBOL_RECT_SOLID)) 			m_pTarget->sym_maximize = NK_SYMBOL_RECT_SOLID;
			if (nk_option_label(ctx, "RECT_OUTLINE", m_pTarget->sym_maximize == NK_SYMBOL_RECT_OUTLINE)) 		m_pTarget->sym_maximize = NK_SYMBOL_RECT_OUTLINE;
			if (nk_option_label(ctx, "TRIANGLE_UP", m_pTarget->sym_maximize == NK_SYMBOL_TRIANGLE_UP)) 		m_pTarget->sym_maximize = NK_SYMBOL_TRIANGLE_UP;
			if (nk_option_label(ctx, "TRIANGLE_DOWN", m_pTarget->sym_maximize == NK_SYMBOL_TRIANGLE_DOWN))		m_pTarget->sym_maximize = NK_SYMBOL_TRIANGLE_DOWN;
			if (nk_option_label(ctx, "TRIANGLE_LEFT", m_pTarget->sym_maximize == NK_SYMBOL_TRIANGLE_LEFT))		m_pTarget->sym_maximize = NK_SYMBOL_TRIANGLE_LEFT;
			if (nk_option_label(ctx, "TRIANGLE_RIGHT", m_pTarget->sym_maximize == NK_SYMBOL_TRIANGLE_RIGHT))		m_pTarget->sym_maximize = NK_SYMBOL_TRIANGLE_RIGHT;
			if (nk_option_label(ctx, "PLUS", m_pTarget->sym_maximize == NK_SYMBOL_PLUS)) 				m_pTarget->sym_maximize = NK_SYMBOL_PLUS;
			if (nk_option_label(ctx, "MINUS", m_pTarget->sym_maximize == NK_SYMBOL_MINUS)) 				m_pTarget->sym_maximize = NK_SYMBOL_MINUS;
			if (nk_option_label(ctx, "MAX", m_pTarget->sym_maximize == NK_SYMBOL_MAX)) 				m_pTarget->sym_maximize = NK_SYMBOL_MAX;
			nk_tree_pop(ctx);
		}
		m_pNodeMaximizeButton->CustomComponentsEditor(ctx, pManager);
		m_pNodeMinimizeButton->CustomComponentsEditor(ctx, pManager);
		nk_tree_pop(ctx);
	}

	if (nk_tree_push_id(ctx, NK_TREE_NODE, "properties", NK_MINIMIZED, id + tree_index++)) {
		nk_layout_row_dynamic(ctx, 22, 1);
		nk_label(ctx, "rounding", NK_TEXT_LEFT);
		nk_property_float(ctx, "#value:", 0.f, &m_pTarget->rounding, 100.f, 1.f, 0.1f);
		nk_label(ctx, "border", NK_TEXT_LEFT);
		nk_property_float(ctx, "#value:", 0.f, &m_pTarget->border, 100.f, 1.f, 0.1f);
		nk_label(ctx, "indent", NK_TEXT_LEFT);
		nk_property_float(ctx, "#value:", 0.f, &m_pTarget->indent, 100.f, 1.f, 0.1f);

		PropertyVector2(ctx, "padding", m_pTarget->padding, 0.f, 1000.f, 1.f, 1.f);
		PropertyVector2(ctx, "spacing", m_pTarget->spacing, 0.f, 1000.f, 1.f, 1.f);

		nk_layout_row_dynamic(ctx, 22, 1);
		nk_label(ctx, "color_factor", NK_TEXT_LEFT);
		nk_property_float(ctx, "#value:", 0.f, &m_pTarget->color_factor, 1.f, 0.01f, 0.01f);
		nk_label(ctx, "disabled_factor", NK_TEXT_LEFT);
		nk_property_float(ctx, "#value:", 0.f, &m_pTarget->disabled_factor, 1.f, 0.01f, 0.01f);

		nk_tree_pop(ctx);
	}
}
