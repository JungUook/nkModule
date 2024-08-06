#include "pch.h"
#include "ComponentScrollbar.h"
#include "NuklearUI.h"

ComponentScrollbar::ComponentScrollbar()
{
	m_pTarget	   = nullptr;
	m_pRestore	   = nullptr;

	m_pNormal	   = nullptr;
	m_pHover	   = nullptr;
	m_pActive	   = nullptr;

	m_CursorNormal = nullptr;
	m_CursorHover  = nullptr;
	m_CursorActive = nullptr;

	m_pIncButton   = nullptr;
	m_pDecButton   = nullptr;

}

ComponentScrollbar::ComponentScrollbar(nk_style_scrollbar* pTarget, nk_style_scrollbar* pRestore)
{
	m_pTarget = pTarget;
	m_pRestore = pRestore;

	m_pNormal = new NKStyleItem(&pTarget->normal, &pRestore->normal);
	m_pHover = new NKStyleItem(&pTarget->hover, &pRestore->hover);
	m_pActive = new NKStyleItem(&pTarget->active, &pRestore->active);

	m_CursorNormal = new NKStyleItem(&pTarget->cursor_normal, &pRestore->cursor_normal);
	m_CursorHover = new NKStyleItem(&pTarget->cursor_hover, &pRestore->cursor_hover);
	m_CursorActive = new NKStyleItem(&pTarget->cursor_active, &pRestore->cursor_active);

	m_pIncButton = new ComponentButton(&pTarget->inc_button, &pRestore->inc_button);
	m_pDecButton = new ComponentButton(&pTarget->dec_button, &pRestore->dec_button);
}

ComponentScrollbar& ComponentScrollbar::operator=(const ComponentScrollbar& other)
{
	if (this != &other) {
		*m_pTarget = *other.m_pTarget;
		*m_pRestore = *other.m_pRestore;

		*m_pNormal = *other.m_pNormal;
		*m_pHover = *other.m_pHover;
		*m_pActive = *other.m_pActive;

		*m_CursorNormal = *other.m_CursorNormal;
		*m_CursorHover = *other.m_CursorHover;
		*m_CursorActive = *other.m_CursorActive;

		*m_pIncButton = *other.m_pIncButton;
		*m_pDecButton = *other.m_pDecButton;
	}
	return *this;
}

ComponentScrollbar::~ComponentScrollbar()
{
	delete m_pNormal;
	delete m_pHover;
	delete m_pActive;

	delete m_CursorNormal;
	delete m_CursorHover;
	delete m_CursorActive;

	delete m_pIncButton;
	delete m_pDecButton;

	m_pNormal = nullptr;
	m_pHover = nullptr;
	m_pActive = nullptr;

	m_CursorNormal = nullptr;
	m_CursorHover = nullptr;
	m_CursorActive = nullptr;

	m_pIncButton = nullptr;
	m_pDecButton = nullptr;
}

void ComponentScrollbar::UpdateComponent(nk_context* ctx, NuklearUI* pManager)
{
	m_pNormal->UpdateComponent(ctx, pManager);
	m_pHover->UpdateComponent(ctx, pManager);
	m_pActive->UpdateComponent(ctx, pManager);

	m_CursorNormal->UpdateComponent(ctx, pManager);
	m_CursorHover->UpdateComponent(ctx, pManager);
	m_CursorActive->UpdateComponent(ctx, pManager);

	m_pIncButton->UpdateComponent(ctx, pManager);
	m_pDecButton->UpdateComponent(ctx, pManager);
}

void ComponentScrollbar::CustomComponentsEditor(nk_context* ctx, NuklearUI* pManager)
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
		if (nk_tree_push_id(ctx, NK_TREE_NODE, "border_color", NK_MINIMIZED, id + tree_index_in++)) {
			ColorPicker(ctx, m_pTarget->border_color);
			nk_tree_pop(ctx);
		}
		nk_tree_pop(ctx);
	}

	if (nk_tree_push_id(ctx, NK_TREE_NODE, "cursor", NK_MINIMIZED, id + tree_index++)) {

		if (nk_tree_push_id(ctx, NK_TREE_NODE, "normal", NK_MINIMIZED, id + tree_index_in++)) {
			m_CursorNormal->ItemEditor(ctx, pManager);
			nk_tree_pop(ctx);
		}
		if (nk_tree_push_id(ctx, NK_TREE_NODE, "hover", NK_MINIMIZED, id + tree_index_in++)) {
			m_CursorHover->ItemEditor(ctx, pManager);
			nk_tree_pop(ctx);
		}
		if (nk_tree_push_id(ctx, NK_TREE_NODE, "active", NK_MINIMIZED, id + tree_index_in++)) {
			m_CursorActive->ItemEditor(ctx, pManager);
			nk_tree_pop(ctx);
		}
		if (nk_tree_push_id(ctx, NK_TREE_NODE, "border_color", NK_MINIMIZED, id + tree_index_in++)) {
			ColorPicker(ctx, m_pTarget->cursor_border_color);
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
		nk_label(ctx, "border_cursor", NK_TEXT_LEFT);
		nk_property_float(ctx, "#value:", 0.f, &m_pTarget->border_cursor, 100.f, 1.f, 0.1f);
		nk_label(ctx, "rounding_cursor", NK_TEXT_LEFT);
		nk_property_float(ctx, "#value:", 0.f, &m_pTarget->rounding_cursor, 100.f, 1.f, 0.1f);

		PropertyVector2(ctx, "padding", m_pTarget->padding, 0.f, 1000.f, 1.f, 1.f);

		nk_layout_row_dynamic(ctx, 22, 1);
		nk_label(ctx, "color_factor", NK_TEXT_LEFT);
		nk_property_float(ctx, "#value:", 0.f, &m_pTarget->color_factor, 1.f, 0.01f, 0.01f);
		nk_label(ctx, "disabled_factor", NK_TEXT_LEFT);
		nk_property_float(ctx, "#value:", 0.f, &m_pTarget->disabled_factor, 1.f, 0.01f, 0.01f);
		nk_tree_pop(ctx);
	}
	if (nk_tree_push_id(ctx, NK_TREE_NODE, "optional buttons", NK_MINIMIZED, id + tree_index++)) {

		nk_checkbox_label(ctx, "Show_buttons", &m_pTarget->show_buttons);

		if (nk_tree_push_id(ctx, NK_TREE_NODE, "inc_button", NK_MINIMIZED, id + tree_index_in++)) {
			if (nk_tree_push_id(ctx, NK_TREE_NODE, "symbol", NK_MINIMIZED, id + tree_index_in_in++)) {
				if (nk_option_label(ctx, "NONE", m_pTarget->inc_symbol == NK_SYMBOL_NONE)) 			m_pTarget->inc_symbol = NK_SYMBOL_NONE;
				if (nk_option_label(ctx, "X", m_pTarget->inc_symbol == NK_SYMBOL_X)) 				m_pTarget->inc_symbol = NK_SYMBOL_X;
				if (nk_option_label(ctx, "UNDERSCORE", m_pTarget->inc_symbol == NK_SYMBOL_UNDERSCORE)) 		m_pTarget->inc_symbol = NK_SYMBOL_UNDERSCORE;
				if (nk_option_label(ctx, "CIRCLE_SOLID", m_pTarget->inc_symbol == NK_SYMBOL_CIRCLE_SOLID)) 	m_pTarget->inc_symbol = NK_SYMBOL_CIRCLE_SOLID;
				if (nk_option_label(ctx, "CIRCLE_OUTLINE", m_pTarget->inc_symbol == NK_SYMBOL_CIRCLE_OUTLINE))	m_pTarget->inc_symbol = NK_SYMBOL_CIRCLE_OUTLINE;
				if (nk_option_label(ctx, "RECT_SOLID", m_pTarget->inc_symbol == NK_SYMBOL_RECT_SOLID)) 		m_pTarget->inc_symbol = NK_SYMBOL_RECT_SOLID;
				if (nk_option_label(ctx, "RECT_OUTLINE", m_pTarget->inc_symbol == NK_SYMBOL_RECT_OUTLINE)) 	m_pTarget->inc_symbol = NK_SYMBOL_RECT_OUTLINE;
				if (nk_option_label(ctx, "TRIANGLE_UP", m_pTarget->inc_symbol == NK_SYMBOL_TRIANGLE_UP)) 	m_pTarget->inc_symbol = NK_SYMBOL_TRIANGLE_UP;
				if (nk_option_label(ctx, "TRIANGLE_DOWN", m_pTarget->inc_symbol == NK_SYMBOL_TRIANGLE_DOWN)) 	m_pTarget->inc_symbol = NK_SYMBOL_TRIANGLE_DOWN;
				if (nk_option_label(ctx, "TRIANGLE_LEFT", m_pTarget->inc_symbol == NK_SYMBOL_TRIANGLE_LEFT)) 	m_pTarget->inc_symbol = NK_SYMBOL_TRIANGLE_LEFT;
				if (nk_option_label(ctx, "TRIANGLE_RIGHT", m_pTarget->inc_symbol == NK_SYMBOL_TRIANGLE_RIGHT))	m_pTarget->inc_symbol = NK_SYMBOL_TRIANGLE_RIGHT;
				if (nk_option_label(ctx, "PLUS", m_pTarget->inc_symbol == NK_SYMBOL_PLUS)) 			m_pTarget->inc_symbol = NK_SYMBOL_PLUS;
				if (nk_option_label(ctx, "MINUS", m_pTarget->inc_symbol == NK_SYMBOL_MINUS)) 			m_pTarget->inc_symbol = NK_SYMBOL_MINUS;
				if (nk_option_label(ctx, "MAX", m_pTarget->inc_symbol == NK_SYMBOL_MAX)) 			m_pTarget->inc_symbol = NK_SYMBOL_MAX;
				nk_tree_pop(ctx);
			}
			m_pIncButton->CustomComponentsEditor(ctx, pManager);
			nk_tree_pop(ctx);
		}

		if (nk_tree_push_id(ctx, NK_TREE_NODE, "dec_button", NK_MINIMIZED, id + tree_index_in++)) {
			if (nk_tree_push_id(ctx, NK_TREE_NODE, "symbol", NK_MINIMIZED, id + tree_index_in_in++)) {
				if (nk_option_label(ctx, "NONE", m_pTarget->dec_symbol == NK_SYMBOL_NONE)) 			m_pTarget->dec_symbol = NK_SYMBOL_NONE;
				if (nk_option_label(ctx, "X", m_pTarget->dec_symbol == NK_SYMBOL_X)) 				m_pTarget->dec_symbol = NK_SYMBOL_X;
				if (nk_option_label(ctx, "UNDERSCORE", m_pTarget->dec_symbol == NK_SYMBOL_UNDERSCORE)) 		m_pTarget->dec_symbol = NK_SYMBOL_UNDERSCORE;
				if (nk_option_label(ctx, "CIRCLE_SOLID", m_pTarget->dec_symbol == NK_SYMBOL_CIRCLE_SOLID)) 	m_pTarget->dec_symbol = NK_SYMBOL_CIRCLE_SOLID;
				if (nk_option_label(ctx, "CIRCLE_OUTLINE", m_pTarget->dec_symbol == NK_SYMBOL_CIRCLE_OUTLINE))	m_pTarget->dec_symbol = NK_SYMBOL_CIRCLE_OUTLINE;
				if (nk_option_label(ctx, "RECT_SOLID", m_pTarget->dec_symbol == NK_SYMBOL_RECT_SOLID)) 		m_pTarget->dec_symbol = NK_SYMBOL_RECT_SOLID;
				if (nk_option_label(ctx, "RECT_OUTLINE", m_pTarget->dec_symbol == NK_SYMBOL_RECT_OUTLINE)) 	m_pTarget->dec_symbol = NK_SYMBOL_RECT_OUTLINE;
				if (nk_option_label(ctx, "TRIANGLE_UP", m_pTarget->dec_symbol == NK_SYMBOL_TRIANGLE_UP)) 	m_pTarget->dec_symbol = NK_SYMBOL_TRIANGLE_UP;
				if (nk_option_label(ctx, "TRIANGLE_DOWN", m_pTarget->dec_symbol == NK_SYMBOL_TRIANGLE_DOWN)) 	m_pTarget->dec_symbol = NK_SYMBOL_TRIANGLE_DOWN;
				if (nk_option_label(ctx, "TRIANGLE_LEFT", m_pTarget->dec_symbol == NK_SYMBOL_TRIANGLE_LEFT)) 	m_pTarget->dec_symbol = NK_SYMBOL_TRIANGLE_LEFT;
				if (nk_option_label(ctx, "TRIANGLE_RIGHT", m_pTarget->dec_symbol == NK_SYMBOL_TRIANGLE_RIGHT))	m_pTarget->dec_symbol = NK_SYMBOL_TRIANGLE_RIGHT;
				if (nk_option_label(ctx, "PLUS", m_pTarget->dec_symbol == NK_SYMBOL_PLUS)) 			m_pTarget->dec_symbol = NK_SYMBOL_PLUS;
				if (nk_option_label(ctx, "MINUS", m_pTarget->dec_symbol == NK_SYMBOL_MINUS)) 			m_pTarget->dec_symbol = NK_SYMBOL_MINUS;
				if (nk_option_label(ctx, "MAX", m_pTarget->dec_symbol == NK_SYMBOL_MAX)) 			m_pTarget->dec_symbol = NK_SYMBOL_MAX;
				nk_tree_pop(ctx);
			}
			m_pDecButton->CustomComponentsEditor(ctx, pManager);
			nk_tree_pop(ctx);
		}
		nk_tree_pop(ctx);
	}
}
