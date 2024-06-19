#include "pch.h"
#include "ComponentCombo.h"
#include "NuklearUI.h"

ComponentCombo::ComponentCombo()
{
	m_pTarget  = nullptr;
	m_pRestore = nullptr;

	m_pNormal  = nullptr;
	m_pHover   = nullptr;
	m_pActive  = nullptr;

	m_pButton  = nullptr;

}

ComponentCombo::ComponentCombo(nk_style_combo* pTarget, nk_style_combo* pRestore)
{
	m_pTarget = pTarget;
	m_pRestore = pRestore;

	m_pNormal = new NKStyleItem(&pTarget->normal, &pRestore->normal);
	m_pHover = new NKStyleItem(&pTarget->hover, &pRestore->hover);
	m_pActive = new NKStyleItem(&pTarget->active, &pRestore->active);

	m_pButton = new ComponentButton(&pTarget->button, &pRestore->button);
}

ComponentCombo::ComponentCombo(const ComponentCombo& other)
{
	m_pTarget = other.m_pTarget;
	m_pRestore = other.m_pRestore;

	m_pNormal = new NKStyleItem(*other.m_pNormal);
	m_pHover = new NKStyleItem(*other.m_pHover);
	m_pActive = new NKStyleItem(*other.m_pActive);

	m_pButton = new ComponentButton(*other.m_pButton);
}

ComponentCombo::~ComponentCombo()
{
	delete m_pNormal;
	delete m_pHover;
	delete m_pActive;

	m_pNormal = nullptr;
	m_pHover = nullptr;
	m_pActive = nullptr;
}

void ComponentCombo::CustomComponentsEditor(nk_context* ctx, NuklearUI* pManager)
{
	int tree_index = 100;
	int tree_index_in = 200;
	int tree_index_in_in = 300;
	intptr_t id = reinterpret_cast<intptr_t>(this);

	if (nk_tree_push_id(ctx, NK_TREE_NODE, "background", NK_MINIMIZED, id + tree_index++)) {

		if (nk_tree_push_id(ctx, NK_TREE_NODE, "normal", NK_MINIMIZED, id + tree_index_in++)) {
			m_pNormal->ItemEditor(ctx, pManager);
			nk_tree_pop(ctx);
		}
		if (nk_tree_push_id(ctx, NK_TREE_NODE, "hover", NK_MINIMIZED, id + tree_index_in++)) {
			m_pHover->ItemEditor(ctx, pManager);
			nk_tree_pop(ctx);
		}
		if (nk_tree_push_id(ctx, NK_TREE_NODE, "active", NK_MINIMIZED, id + tree_index_in++)) {
			m_pActive->ItemEditor(ctx, pManager);
			nk_tree_pop(ctx);
		}
		if (nk_tree_push_id(ctx, NK_TREE_NODE, "border", NK_MINIMIZED, id + tree_index_in++)) {
			ColorPicker(ctx, m_pTarget->border_color);
			nk_tree_pop(ctx);
		}
		nk_tree_pop(ctx);
	}

	if (nk_tree_push_id(ctx, NK_TREE_NODE, "label", NK_MINIMIZED, id + tree_index++)) {
		if (nk_tree_push_id(ctx, NK_TREE_NODE, "normal", NK_MINIMIZED, id + tree_index_in++)) {
			ColorPicker(ctx, m_pTarget->label_normal);
			nk_tree_pop(ctx);
		}
		if (nk_tree_push_id(ctx, NK_TREE_NODE, "hover", NK_MINIMIZED, id + tree_index_in++)) {
			ColorPicker(ctx, m_pTarget->label_hover);
			nk_tree_pop(ctx);
		}
		if (nk_tree_push_id(ctx, NK_TREE_NODE, "active", NK_MINIMIZED, id + tree_index_in++)) {
			ColorPicker(ctx, m_pTarget->label_active);
			nk_tree_pop(ctx);
		}
		nk_tree_pop(ctx);
	}

	if (nk_tree_push_id(ctx, NK_TREE_NODE, "symbol", NK_MINIMIZED, id + tree_index++)) {
		if (nk_tree_push_id(ctx, NK_TREE_NODE, "normal", NK_MINIMIZED, id + tree_index_in++)) {
			ColorPicker(ctx, m_pTarget->symbol_normal);
			nk_tree_pop(ctx);
		}
		if (nk_tree_push_id(ctx, NK_TREE_NODE, "hover", NK_MINIMIZED, id + tree_index_in++)) {
			ColorPicker(ctx, m_pTarget->symbol_hover);
			nk_tree_pop(ctx);
		}
		if (nk_tree_push_id(ctx, NK_TREE_NODE, "active", NK_MINIMIZED, id + tree_index_in++)) {
			ColorPicker(ctx, m_pTarget->symbol_active);
			nk_tree_pop(ctx);
		}
		nk_tree_pop(ctx);
	}

	if (nk_tree_push(ctx, NK_TREE_NODE, "button", NK_MINIMIZED)) {

		m_pButton->CustomComponentsEditor(ctx, pManager);

		if (nk_tree_push(ctx, NK_TREE_NODE, "sym_normal", NK_MINIMIZED)) {
			if (nk_option_label(ctx, "NONE", m_pTarget->sym_normal == NK_SYMBOL_NONE)) 				m_pTarget->sym_normal = NK_SYMBOL_NONE;
			if (nk_option_label(ctx, "X", m_pTarget->sym_normal == NK_SYMBOL_X)) 					m_pTarget->sym_normal = NK_SYMBOL_X;
			if (nk_option_label(ctx, "UNDERSCORE", m_pTarget->sym_normal == NK_SYMBOL_UNDERSCORE)) 			m_pTarget->sym_normal = NK_SYMBOL_UNDERSCORE;
			if (nk_option_label(ctx, "CIRCLE_SOLID", m_pTarget->sym_normal == NK_SYMBOL_CIRCLE_SOLID)) 		m_pTarget->sym_normal = NK_SYMBOL_CIRCLE_SOLID;
			if (nk_option_label(ctx, "CIRCLE_OUTLINE", m_pTarget->sym_normal == NK_SYMBOL_CIRCLE_OUTLINE))		m_pTarget->sym_normal = NK_SYMBOL_CIRCLE_OUTLINE;
			if (nk_option_label(ctx, "RECT_SOLID", m_pTarget->sym_normal == NK_SYMBOL_RECT_SOLID)) 			m_pTarget->sym_normal = NK_SYMBOL_RECT_SOLID;
			if (nk_option_label(ctx, "RECT_OUTLINE", m_pTarget->sym_normal == NK_SYMBOL_RECT_OUTLINE)) 		m_pTarget->sym_normal = NK_SYMBOL_RECT_OUTLINE;
			if (nk_option_label(ctx, "TRIANGLE_UP", m_pTarget->sym_normal == NK_SYMBOL_TRIANGLE_UP)) 		m_pTarget->sym_normal = NK_SYMBOL_TRIANGLE_UP;
			if (nk_option_label(ctx, "TRIANGLE_DOWN", m_pTarget->sym_normal == NK_SYMBOL_TRIANGLE_DOWN))		m_pTarget->sym_normal = NK_SYMBOL_TRIANGLE_DOWN;
			if (nk_option_label(ctx, "TRIANGLE_LEFT", m_pTarget->sym_normal == NK_SYMBOL_TRIANGLE_LEFT))		m_pTarget->sym_normal = NK_SYMBOL_TRIANGLE_LEFT;
			if (nk_option_label(ctx, "TRIANGLE_RIGHT", m_pTarget->sym_normal == NK_SYMBOL_TRIANGLE_RIGHT))		m_pTarget->sym_normal = NK_SYMBOL_TRIANGLE_RIGHT;
			if (nk_option_label(ctx, "PLUS", m_pTarget->sym_normal == NK_SYMBOL_PLUS)) 				m_pTarget->sym_normal = NK_SYMBOL_PLUS;
			if (nk_option_label(ctx, "MINUS", m_pTarget->sym_normal == NK_SYMBOL_MINUS)) 				m_pTarget->sym_normal = NK_SYMBOL_MINUS;
			if (nk_option_label(ctx, "MAX", m_pTarget->sym_normal == NK_SYMBOL_MAX)) 				m_pTarget->sym_normal = NK_SYMBOL_MAX;
			nk_tree_pop(ctx);
		}
		if (nk_tree_push(ctx, NK_TREE_NODE, "sym_hover", NK_MINIMIZED)) {
			if (nk_option_label(ctx, "NONE", m_pTarget->sym_hover == NK_SYMBOL_NONE)) 				m_pTarget->sym_hover = NK_SYMBOL_NONE;
			if (nk_option_label(ctx, "X", m_pTarget->sym_hover == NK_SYMBOL_X)) 					m_pTarget->sym_hover = NK_SYMBOL_X;
			if (nk_option_label(ctx, "UNDERSCORE", m_pTarget->sym_hover == NK_SYMBOL_UNDERSCORE)) 			m_pTarget->sym_hover = NK_SYMBOL_UNDERSCORE;
			if (nk_option_label(ctx, "CIRCLE_SOLID", m_pTarget->sym_hover == NK_SYMBOL_CIRCLE_SOLID)) 		m_pTarget->sym_hover = NK_SYMBOL_CIRCLE_SOLID;
			if (nk_option_label(ctx, "CIRCLE_OUTLINE", m_pTarget->sym_hover == NK_SYMBOL_CIRCLE_OUTLINE))		m_pTarget->sym_hover = NK_SYMBOL_CIRCLE_OUTLINE;
			if (nk_option_label(ctx, "RECT_SOLID", m_pTarget->sym_hover == NK_SYMBOL_RECT_SOLID)) 			m_pTarget->sym_hover = NK_SYMBOL_RECT_SOLID;
			if (nk_option_label(ctx, "RECT_OUTLINE", m_pTarget->sym_hover == NK_SYMBOL_RECT_OUTLINE)) 		m_pTarget->sym_hover = NK_SYMBOL_RECT_OUTLINE;
			if (nk_option_label(ctx, "TRIANGLE_UP", m_pTarget->sym_hover == NK_SYMBOL_TRIANGLE_UP)) 			m_pTarget->sym_hover = NK_SYMBOL_TRIANGLE_UP;
			if (nk_option_label(ctx, "TRIANGLE_DOWN", m_pTarget->sym_hover == NK_SYMBOL_TRIANGLE_DOWN))		m_pTarget->sym_hover = NK_SYMBOL_TRIANGLE_DOWN;
			if (nk_option_label(ctx, "TRIANGLE_LEFT", m_pTarget->sym_hover == NK_SYMBOL_TRIANGLE_LEFT))		m_pTarget->sym_hover = NK_SYMBOL_TRIANGLE_LEFT;
			if (nk_option_label(ctx, "TRIANGLE_RIGHT", m_pTarget->sym_hover == NK_SYMBOL_TRIANGLE_RIGHT))		m_pTarget->sym_hover = NK_SYMBOL_TRIANGLE_RIGHT;
			if (nk_option_label(ctx, "PLUS", m_pTarget->sym_hover == NK_SYMBOL_PLUS)) 				m_pTarget->sym_hover = NK_SYMBOL_PLUS;
			if (nk_option_label(ctx, "MINUS", m_pTarget->sym_hover == NK_SYMBOL_MINUS)) 				m_pTarget->sym_hover = NK_SYMBOL_MINUS;
			if (nk_option_label(ctx, "MAX", m_pTarget->sym_hover == NK_SYMBOL_MAX)) 					m_pTarget->sym_hover = NK_SYMBOL_MAX;
			nk_tree_pop(ctx);
		}
		if (nk_tree_push(ctx, NK_TREE_NODE, "sym_active", NK_MINIMIZED)) {
			if (nk_option_label(ctx, "NONE", m_pTarget->sym_active == NK_SYMBOL_NONE)) 				m_pTarget->sym_active = NK_SYMBOL_NONE;
			if (nk_option_label(ctx, "X", m_pTarget->sym_active == NK_SYMBOL_X)) 					m_pTarget->sym_active = NK_SYMBOL_X;
			if (nk_option_label(ctx, "UNDERSCORE", m_pTarget->sym_active == NK_SYMBOL_UNDERSCORE)) 			m_pTarget->sym_active = NK_SYMBOL_UNDERSCORE;
			if (nk_option_label(ctx, "CIRCLE_SOLID", m_pTarget->sym_active == NK_SYMBOL_CIRCLE_SOLID)) 		m_pTarget->sym_active = NK_SYMBOL_CIRCLE_SOLID;
			if (nk_option_label(ctx, "CIRCLE_OUTLINE", m_pTarget->sym_active == NK_SYMBOL_CIRCLE_OUTLINE))		m_pTarget->sym_active = NK_SYMBOL_CIRCLE_OUTLINE;
			if (nk_option_label(ctx, "RECT_SOLID", m_pTarget->sym_active == NK_SYMBOL_RECT_SOLID)) 			m_pTarget->sym_active = NK_SYMBOL_RECT_SOLID;
			if (nk_option_label(ctx, "RECT_OUTLINE", m_pTarget->sym_active == NK_SYMBOL_RECT_OUTLINE)) 		m_pTarget->sym_active = NK_SYMBOL_RECT_OUTLINE;
			if (nk_option_label(ctx, "TRIANGLE_UP", m_pTarget->sym_active == NK_SYMBOL_TRIANGLE_UP)) 		m_pTarget->sym_active = NK_SYMBOL_TRIANGLE_UP;
			if (nk_option_label(ctx, "TRIANGLE_DOWN", m_pTarget->sym_active == NK_SYMBOL_TRIANGLE_DOWN))		m_pTarget->sym_active = NK_SYMBOL_TRIANGLE_DOWN;
			if (nk_option_label(ctx, "TRIANGLE_LEFT", m_pTarget->sym_active == NK_SYMBOL_TRIANGLE_LEFT))		m_pTarget->sym_active = NK_SYMBOL_TRIANGLE_LEFT;
			if (nk_option_label(ctx, "TRIANGLE_RIGHT", m_pTarget->sym_active == NK_SYMBOL_TRIANGLE_RIGHT))		m_pTarget->sym_active = NK_SYMBOL_TRIANGLE_RIGHT;
			if (nk_option_label(ctx, "PLUS", m_pTarget->sym_active == NK_SYMBOL_PLUS)) 				m_pTarget->sym_active = NK_SYMBOL_PLUS;
			if (nk_option_label(ctx, "MINUS", m_pTarget->sym_active == NK_SYMBOL_MINUS)) 				m_pTarget->sym_active = NK_SYMBOL_MINUS;
			if (nk_option_label(ctx, "MAX", m_pTarget->sym_active == NK_SYMBOL_MAX)) 				m_pTarget->sym_active = NK_SYMBOL_MAX;
			nk_tree_pop(ctx);
		}
		nk_tree_pop(ctx);
	}

	if (nk_tree_push_id(ctx, NK_TREE_NODE, "properties", NK_MINIMIZED, id + tree_index++)) {
		nk_layout_row_dynamic(ctx, 22, 1);
		nk_label(ctx, "rounding", NK_TEXT_LEFT);
		nk_property_float(ctx, "#value:", 0.f, &m_pTarget->rounding, 100.f, 1.f, 0.1f);
		nk_label(ctx, "border", NK_TEXT_LEFT);
		nk_property_float(ctx, "#value:", 0.f, &m_pTarget->border, 100.f, 1.f, 0.1f);

		PropertyVector2(ctx, "content_padding", m_pTarget->content_padding, 0.f, 1000.f, 1.f, 1.f);
		PropertyVector2(ctx, "button_padding", m_pTarget->button_padding, 0.f, 1000.f, 1.f, 1.f);
		PropertyVector2(ctx, "spacing", m_pTarget->spacing, 0.f, 1000.f, 1.f, 1.f);

		nk_layout_row_dynamic(ctx, 22, 1);
		nk_label(ctx, "color_factor", NK_TEXT_LEFT);
		nk_property_float(ctx, "#value:", 0.f, &m_pTarget->color_factor, 1.f, 0.01f, 0.01f);
		nk_label(ctx, "disabled_factor", NK_TEXT_LEFT);
		nk_property_float(ctx, "#value:", 0.f, &m_pTarget->disabled_factor, 1.f, 0.01f, 0.01f);

		nk_tree_pop(ctx);
	}
}
