#include "pch.h"
#include "ComponentHeader.h"
#include "NuklearUI.h"

ComponentHeader::ComponentHeader()
{
	m_pTarget		  = nullptr;
	m_pRestore		  = nullptr;

	m_pNormal		  = nullptr;
	m_pHover		  = nullptr;
	m_pActive		  = nullptr;

	m_pCloseButton	  = nullptr;
	m_pMinimizeButton = nullptr;

}

ComponentHeader::ComponentHeader(nk_style_window_header* pTarget, nk_style_window_header* pRestore)
{
	m_pTarget = pTarget;
	m_pRestore = pRestore;

	m_pNormal = new NKStyleItem(&pTarget->normal, &pRestore->normal);
	m_pHover = new NKStyleItem(&pTarget->hover, &pRestore->hover);
	m_pActive = new NKStyleItem(&pTarget->active, &pRestore->active);

	m_pCloseButton = new ComponentButton(&pTarget->close_button, &pRestore->close_button);
	m_pMinimizeButton = new ComponentButton(&pTarget->minimize_button, &pRestore->minimize_button);
}

ComponentHeader::ComponentHeader(const ComponentHeader& other)
{
	m_pTarget = other.m_pTarget;
	m_pRestore = other.m_pRestore;

	m_pNormal = new NKStyleItem(*other.m_pNormal);
	m_pHover = new NKStyleItem(*other.m_pHover);
	m_pActive = new NKStyleItem(*other.m_pActive);

	m_pCloseButton = new ComponentButton(*other.m_pCloseButton);
	m_pMinimizeButton = new ComponentButton(*other.m_pMinimizeButton);
}

ComponentHeader::~ComponentHeader()
{
	delete m_pNormal;
	delete m_pHover;
	delete m_pActive;

	delete m_pCloseButton;
	delete m_pMinimizeButton;

	m_pNormal = nullptr;
	m_pHover = nullptr;
	m_pActive = nullptr;

	m_pCloseButton = nullptr;
	m_pMinimizeButton = nullptr;
}

void ComponentHeader::CustomComponentsEditor(nk_context* ctx, NuklearUI* pManager)
{
	if (nk_tree_push(ctx, NK_TREE_NODE, "header_bg", NK_MINIMIZED)) {
		if (nk_tree_push(ctx, NK_TREE_NODE, "normal", NK_MINIMIZED)) {
			m_pNormal->ItemEditor(ctx, pManager);
			nk_tree_pop(ctx);
		}
		if (nk_tree_push(ctx, NK_TREE_NODE, "hover", NK_MINIMIZED)) {
			m_pHover->ItemEditor(ctx, pManager);
			nk_tree_pop(ctx);
		}
		if (nk_tree_push(ctx, NK_TREE_NODE, "active", NK_MINIMIZED)) {
			m_pActive->ItemEditor(ctx, pManager);
			nk_tree_pop(ctx);
		}
		nk_tree_pop(ctx);
	}

	if (nk_tree_push(ctx, NK_TREE_NODE, "close_button", NK_MINIMIZED)) {
		if (nk_tree_push(ctx, NK_TREE_NODE, "symbol", NK_MINIMIZED)) {
			if (nk_option_label(ctx, "NONE", m_pTarget->close_symbol == NK_SYMBOL_NONE)) 			m_pTarget->close_symbol = NK_SYMBOL_NONE;
			if (nk_option_label(ctx, "X", m_pTarget->close_symbol == NK_SYMBOL_X)) 				m_pTarget->close_symbol = NK_SYMBOL_X;
			if (nk_option_label(ctx, "UNDERSCORE", m_pTarget->close_symbol == NK_SYMBOL_UNDERSCORE)) 		m_pTarget->close_symbol = NK_SYMBOL_UNDERSCORE;
			if (nk_option_label(ctx, "CIRCLE_SOLID", m_pTarget->close_symbol == NK_SYMBOL_CIRCLE_SOLID)) 	m_pTarget->close_symbol = NK_SYMBOL_CIRCLE_SOLID;
			if (nk_option_label(ctx, "CIRCLE_OUTLINE", m_pTarget->close_symbol == NK_SYMBOL_CIRCLE_OUTLINE))  m_pTarget->close_symbol = NK_SYMBOL_CIRCLE_OUTLINE;
			if (nk_option_label(ctx, "RECT_SOLID", m_pTarget->close_symbol == NK_SYMBOL_RECT_SOLID)) 		m_pTarget->close_symbol = NK_SYMBOL_RECT_SOLID;
			if (nk_option_label(ctx, "RECT_OUTLINE", m_pTarget->close_symbol == NK_SYMBOL_RECT_OUTLINE)) 	m_pTarget->close_symbol = NK_SYMBOL_RECT_OUTLINE;
			if (nk_option_label(ctx, "TRIANGLE_UP", m_pTarget->close_symbol == NK_SYMBOL_TRIANGLE_UP)) 	m_pTarget->close_symbol = NK_SYMBOL_TRIANGLE_UP;
			if (nk_option_label(ctx, "TRIANGLE_DOWN", m_pTarget->close_symbol == NK_SYMBOL_TRIANGLE_DOWN)) 	m_pTarget->close_symbol = NK_SYMBOL_TRIANGLE_DOWN;
			if (nk_option_label(ctx, "TRIANGLE_LEFT", m_pTarget->close_symbol == NK_SYMBOL_TRIANGLE_LEFT)) 	m_pTarget->close_symbol = NK_SYMBOL_TRIANGLE_LEFT;
			if (nk_option_label(ctx, "TRIANGLE_RIGHT", m_pTarget->close_symbol == NK_SYMBOL_TRIANGLE_RIGHT))  m_pTarget->close_symbol = NK_SYMBOL_TRIANGLE_RIGHT;
			if (nk_option_label(ctx, "PLUS", m_pTarget->close_symbol == NK_SYMBOL_PLUS)) 			m_pTarget->close_symbol = NK_SYMBOL_PLUS;
			if (nk_option_label(ctx, "MINUS", m_pTarget->close_symbol == NK_SYMBOL_MINUS)) 			m_pTarget->close_symbol = NK_SYMBOL_MINUS;
			if (nk_option_label(ctx, "MAX", m_pTarget->close_symbol == NK_SYMBOL_MAX)) 			m_pTarget->close_symbol = NK_SYMBOL_MAX;
			nk_tree_pop(ctx);
		}

		m_pCloseButton->CustomComponentsEditor(ctx, pManager);
		nk_tree_pop(ctx);
	}
	if (nk_tree_push(ctx, NK_TREE_NODE, "minimize_button", NK_MINIMIZED)) {
		if (nk_tree_push(ctx, NK_TREE_NODE, "symbol", NK_MINIMIZED)) {
			if (nk_option_label(ctx, "NONE", m_pTarget->minimize_symbol == NK_SYMBOL_NONE)) 			m_pTarget->minimize_symbol = NK_SYMBOL_NONE;
			if (nk_option_label(ctx, "X", m_pTarget->minimize_symbol == NK_SYMBOL_X)) 				m_pTarget->minimize_symbol = NK_SYMBOL_X;
			if (nk_option_label(ctx, "UNDERSCORE", m_pTarget->minimize_symbol == NK_SYMBOL_UNDERSCORE)) 	m_pTarget->minimize_symbol = NK_SYMBOL_UNDERSCORE;
			if (nk_option_label(ctx, "CIRCLE_SOLID", m_pTarget->minimize_symbol == NK_SYMBOL_CIRCLE_SOLID)) 	m_pTarget->minimize_symbol = NK_SYMBOL_CIRCLE_SOLID;
			if (nk_option_label(ctx, "CIRCLE_OUTLINE", m_pTarget->minimize_symbol == NK_SYMBOL_CIRCLE_OUTLINE))	m_pTarget->minimize_symbol = NK_SYMBOL_CIRCLE_OUTLINE;
			if (nk_option_label(ctx, "RECT_SOLID", m_pTarget->minimize_symbol == NK_SYMBOL_RECT_SOLID)) 	m_pTarget->minimize_symbol = NK_SYMBOL_RECT_SOLID;
			if (nk_option_label(ctx, "RECT_OUTLINE", m_pTarget->minimize_symbol == NK_SYMBOL_RECT_OUTLINE)) 	m_pTarget->minimize_symbol = NK_SYMBOL_RECT_OUTLINE;
			if (nk_option_label(ctx, "TRIANGLE_UP", m_pTarget->minimize_symbol == NK_SYMBOL_TRIANGLE_UP)) 	m_pTarget->minimize_symbol = NK_SYMBOL_TRIANGLE_UP;
			if (nk_option_label(ctx, "TRIANGLE_DOWN", m_pTarget->minimize_symbol == NK_SYMBOL_TRIANGLE_DOWN)) 	m_pTarget->minimize_symbol = NK_SYMBOL_TRIANGLE_DOWN;
			if (nk_option_label(ctx, "TRIANGLE_LEFT", m_pTarget->minimize_symbol == NK_SYMBOL_TRIANGLE_LEFT)) 	m_pTarget->minimize_symbol = NK_SYMBOL_TRIANGLE_LEFT;
			if (nk_option_label(ctx, "TRIANGLE_RIGHT", m_pTarget->minimize_symbol == NK_SYMBOL_TRIANGLE_RIGHT))	m_pTarget->minimize_symbol = NK_SYMBOL_TRIANGLE_RIGHT;
			if (nk_option_label(ctx, "PLUS", m_pTarget->minimize_symbol == NK_SYMBOL_PLUS)) 			m_pTarget->minimize_symbol = NK_SYMBOL_PLUS;
			if (nk_option_label(ctx, "MINUS", m_pTarget->minimize_symbol == NK_SYMBOL_MINUS)) 			m_pTarget->minimize_symbol = NK_SYMBOL_MINUS;
			if (nk_option_label(ctx, "MAX", m_pTarget->minimize_symbol == NK_SYMBOL_MAX)) 			m_pTarget->minimize_symbol = NK_SYMBOL_MAX;
			nk_tree_pop(ctx);
		}
		m_pMinimizeButton->CustomComponentsEditor(ctx, pManager);
		nk_tree_pop(ctx);
	}

	if (nk_tree_push(ctx, NK_TREE_NODE, "maximize_symbol", NK_MINIMIZED)) {
		if (nk_option_label(ctx, "NONE", m_pTarget->maximize_symbol == NK_SYMBOL_NONE)) 			m_pTarget->maximize_symbol = NK_SYMBOL_NONE;
		if (nk_option_label(ctx, "X", m_pTarget->maximize_symbol == NK_SYMBOL_X)) 				m_pTarget->maximize_symbol = NK_SYMBOL_X;
		if (nk_option_label(ctx, "UNDERSCORE", m_pTarget->maximize_symbol == NK_SYMBOL_UNDERSCORE)) 	m_pTarget->maximize_symbol = NK_SYMBOL_UNDERSCORE;
		if (nk_option_label(ctx, "CIRCLE_SOLID", m_pTarget->maximize_symbol == NK_SYMBOL_CIRCLE_SOLID)) 	m_pTarget->maximize_symbol = NK_SYMBOL_CIRCLE_SOLID;
		if (nk_option_label(ctx, "CIRCLE_OUTLINE", m_pTarget->maximize_symbol == NK_SYMBOL_CIRCLE_OUTLINE))	m_pTarget->maximize_symbol = NK_SYMBOL_CIRCLE_OUTLINE;
		if (nk_option_label(ctx, "RECT_SOLID", m_pTarget->maximize_symbol == NK_SYMBOL_RECT_SOLID)) 	m_pTarget->maximize_symbol = NK_SYMBOL_RECT_SOLID;
		if (nk_option_label(ctx, "RECT_OUTLINE", m_pTarget->maximize_symbol == NK_SYMBOL_RECT_OUTLINE)) 	m_pTarget->maximize_symbol = NK_SYMBOL_RECT_OUTLINE;
		if (nk_option_label(ctx, "TRIANGLE_UP", m_pTarget->maximize_symbol == NK_SYMBOL_TRIANGLE_UP)) 	m_pTarget->maximize_symbol = NK_SYMBOL_TRIANGLE_UP;
		if (nk_option_label(ctx, "TRIANGLE_DOWN", m_pTarget->maximize_symbol == NK_SYMBOL_TRIANGLE_DOWN)) 	m_pTarget->maximize_symbol = NK_SYMBOL_TRIANGLE_DOWN;
		if (nk_option_label(ctx, "TRIANGLE_LEFT", m_pTarget->maximize_symbol == NK_SYMBOL_TRIANGLE_LEFT)) 	m_pTarget->maximize_symbol = NK_SYMBOL_TRIANGLE_LEFT;
		if (nk_option_label(ctx, "TRIANGLE_RIGHT", m_pTarget->maximize_symbol == NK_SYMBOL_TRIANGLE_RIGHT))	m_pTarget->maximize_symbol = NK_SYMBOL_TRIANGLE_RIGHT;
		if (nk_option_label(ctx, "PLUS", m_pTarget->maximize_symbol == NK_SYMBOL_PLUS)) 			m_pTarget->maximize_symbol = NK_SYMBOL_PLUS;
		if (nk_option_label(ctx, "MINUS", m_pTarget->maximize_symbol == NK_SYMBOL_MINUS)) 			m_pTarget->maximize_symbol = NK_SYMBOL_MINUS;
		if (nk_option_label(ctx, "MAX", m_pTarget->maximize_symbol == NK_SYMBOL_MAX)) 			m_pTarget->maximize_symbol = NK_SYMBOL_MAX;
		nk_tree_pop(ctx);
	}


	if (nk_tree_push(ctx, NK_TREE_NODE, "label", NK_MINIMIZED)) {
		if (nk_tree_push(ctx, NK_TREE_NODE, "normal", NK_MINIMIZED)) {
			ColorPicker(ctx, m_pTarget->label_normal);
			nk_tree_pop(ctx);
		}
		if (nk_tree_push(ctx, NK_TREE_NODE, "hover", NK_MINIMIZED)) {
			ColorPicker(ctx, m_pTarget->label_hover);
			nk_tree_pop(ctx);
		}
		if (nk_tree_push(ctx, NK_TREE_NODE, "active", NK_MINIMIZED)) {
			ColorPicker(ctx, m_pTarget->label_active);
			nk_tree_pop(ctx);
		}
		nk_tree_pop(ctx);
	}
	if (nk_tree_push(ctx, NK_TREE_NODE, "align", NK_MINIMIZED)) {
		if (nk_option_label(ctx, "left", m_pTarget->align == NK_HEADER_LEFT)) m_pTarget->align = NK_HEADER_LEFT;
		if (nk_option_label(ctx, "right", m_pTarget->align == NK_HEADER_RIGHT)) m_pTarget->align = NK_HEADER_RIGHT;
		nk_tree_pop(ctx);
	}
	if (nk_tree_push(ctx, NK_TREE_NODE, "padding", NK_MINIMIZED)) {
		PropertyVector2(ctx, "Vector2", m_pTarget->padding, 0.f, 1000.f, 1.f, 1.f);
		nk_tree_pop(ctx);
	}
	if (nk_tree_push(ctx, NK_TREE_NODE, "label_padding", NK_MINIMIZED)) {
		PropertyVector2(ctx, "Vector2", m_pTarget->label_padding, 0.f, 1000.f, 1.f, 1.f);
		nk_tree_pop(ctx);
	}
	if (nk_tree_push(ctx, NK_TREE_NODE, "spacing", NK_MINIMIZED)) {
		PropertyVector2(ctx, "Vector2", m_pTarget->spacing, 0.f, 1000.f, 1.f, 1.f);
		nk_tree_pop(ctx);
	}
}
