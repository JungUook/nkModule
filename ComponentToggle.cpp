#include "pch.h"
#include "ComponentToggle.h"
#include "NuklearUI.h"

ComponentToggle::ComponentToggle()
{
	m_pTarget		= nullptr;
	m_pRestore		= nullptr;

	m_pNormal		= nullptr;
	m_pHover		= nullptr;
	m_pActive		= nullptr;

	m_pCursorNormal	= nullptr;
	m_pCursorHover	= nullptr;

}

ComponentToggle::ComponentToggle(nk_style_toggle* pTarget, nk_style_toggle* pRestore)
{
	m_pTarget = pTarget;
	m_pRestore = pRestore;

	m_pNormal	= new NKStyleItem(&pTarget->normal, &pRestore->normal);
	m_pHover	= new NKStyleItem(&pTarget->hover, &pRestore->hover);
	m_pActive	= new NKStyleItem(&pTarget->active, &pRestore->active);

	m_pCursorNormal	= new NKStyleItem(&pTarget->cursor_normal, &pRestore->cursor_normal);
	m_pCursorHover	= new NKStyleItem(&pTarget->cursor_hover, &pRestore->cursor_hover);
}

ComponentToggle& ComponentToggle::operator=(const ComponentToggle& other)
{
	if (this != &other) {
		*m_pTarget = *other.m_pTarget;
		*m_pRestore = *other.m_pRestore;

		*m_pNormal = *other.m_pNormal;
		*m_pHover = *other.m_pHover;
		*m_pActive = *other.m_pActive;

		*m_pCursorNormal = *other.m_pCursorNormal;
		*m_pCursorHover = *other.m_pCursorHover;
	}
	return *this;
}

ComponentToggle::~ComponentToggle()
{
	delete m_pNormal;
	delete m_pHover;
	delete m_pActive;

	delete m_pCursorNormal;
	delete m_pCursorHover;

	m_pNormal = nullptr;
	m_pHover = nullptr;
	m_pActive = nullptr;

	m_pCursorNormal = nullptr;
	m_pCursorHover = nullptr;
}

void ComponentToggle::UpdateComponent(nk_context* ctx, NuklearUI* pManager)
{
	m_pNormal->UpdateComponent(ctx,pManager);
	m_pHover->UpdateComponent(ctx,pManager);
	m_pActive->UpdateComponent(ctx,pManager);

	m_pCursorNormal->UpdateComponent(ctx,pManager);
	m_pCursorHover->UpdateComponent(ctx, pManager);
}

void ComponentToggle::CustomComponentsEditor(nk_context* ctx, NuklearUI* pManager)
{
	int tree_index = 100;
	int tree_index_in = 200;
	int tree_index_in_in = 300;

	if (nk_tree_push_id(ctx, NK_TREE_NODE, "background", NK_MINIMIZED, reinterpret_cast<intptr_t>(this) + tree_index++)) {

		if (nk_tree_push_id(ctx, NK_TREE_NODE, "normal", NK_MINIMIZED, reinterpret_cast<intptr_t>(this) + tree_index_in++)) {
			m_pNormal->ItemEditor(ctx, pManager);
			nk_tree_pop(ctx);
		}
		if (nk_tree_push_id(ctx, NK_TREE_NODE, "hover", NK_MINIMIZED, reinterpret_cast<intptr_t>(this) + tree_index_in++)) {
			m_pHover->ItemEditor(ctx, pManager);
			nk_tree_pop(ctx);
		}
		if (nk_tree_push_id(ctx, NK_TREE_NODE, "active", NK_MINIMIZED, reinterpret_cast<intptr_t>(this) + tree_index_in++)) {
			m_pActive->ItemEditor(ctx, pManager);
			nk_tree_pop(ctx);
		}
		nk_tree_pop(ctx);
	}

	if (nk_tree_push_id(ctx, NK_TREE_NODE, "border", NK_MINIMIZED, reinterpret_cast<intptr_t>(this) + tree_index++)) {
		nk_layout_row_dynamic(ctx, 22, 1);
		nk_property_float(ctx, "#Border:", 0.f, &m_pTarget->border, 9.f, 1.f, 0.1f);
		ColorPicker(ctx, m_pTarget->border_color);
		nk_tree_pop(ctx);
	}

	if (nk_tree_push_id(ctx, NK_TREE_NODE, "cursor", NK_MINIMIZED, reinterpret_cast<intptr_t>(this) + tree_index++)) {

		if (nk_tree_push_id(ctx, NK_TREE_NODE, "normal", NK_MINIMIZED, reinterpret_cast<intptr_t>(this) + tree_index_in++)) {
			m_pCursorNormal->ItemEditor(ctx, pManager);
			nk_tree_pop(ctx);
		}
		if (nk_tree_push_id(ctx, NK_TREE_NODE, "hover", NK_MINIMIZED, reinterpret_cast<intptr_t>(this) + tree_index_in++)) {
			m_pCursorHover->ItemEditor(ctx, pManager);
			nk_tree_pop(ctx);
		}
		nk_tree_pop(ctx);
	}

	if (nk_tree_push_id(ctx, NK_TREE_NODE, "text", NK_MINIMIZED, reinterpret_cast<intptr_t>(this) + tree_index++)) {

		if (nk_tree_push_id(ctx, NK_TREE_NODE, "background", NK_MINIMIZED, reinterpret_cast<intptr_t>(this) + tree_index_in++)) {
			ColorPicker(ctx, m_pTarget->text_background);
			nk_tree_pop(ctx);
		}
		if (nk_tree_push_id(ctx, NK_TREE_NODE, "normal", NK_MINIMIZED, reinterpret_cast<intptr_t>(this) + tree_index_in++)) {
			ColorPicker(ctx, m_pTarget->text_normal);
			nk_tree_pop(ctx);
		}
		if (nk_tree_push_id(ctx, NK_TREE_NODE, "hover", NK_MINIMIZED, reinterpret_cast<intptr_t>(this) + tree_index_in++)) {
			ColorPicker(ctx, m_pTarget->text_hover);
			nk_tree_pop(ctx);
		}
		if (nk_tree_push_id(ctx, NK_TREE_NODE, "active", NK_MINIMIZED, reinterpret_cast<intptr_t>(this) + tree_index_in++)) {
			ColorPicker(ctx, m_pTarget->text_active);
			nk_tree_pop(ctx);
		}

		if (nk_tree_push_id(ctx, NK_TREE_NODE, "align", NK_MINIMIZED, reinterpret_cast<intptr_t>(this) + tree_index_in++)) {
			if (nk_option_label(ctx, "left", m_pTarget->text_alignment == NK_HEADER_LEFT))  m_pTarget->text_alignment = NK_HEADER_LEFT;
			if (nk_option_label(ctx, "center", m_pTarget->text_alignment == NK_TEXT_CENTERED))  m_pTarget->text_alignment = NK_TEXT_CENTERED;
			if (nk_option_label(ctx, "right", m_pTarget->text_alignment == NK_HEADER_RIGHT))  m_pTarget->text_alignment = NK_HEADER_RIGHT;
			nk_tree_pop(ctx);
		}
		nk_tree_pop(ctx);
	}

	if (nk_tree_push_id(ctx, NK_TREE_NODE, "properties", NK_MINIMIZED, reinterpret_cast<intptr_t>(this) + tree_index++)) {
		nk_layout_row_dynamic(ctx, 22, 1);

		PropertyVector2(ctx, "padding", m_pTarget->padding, 0.f, 1000.f, 1.f, 1.f);
		PropertyVector2(ctx, "touch_padding", m_pTarget->touch_padding, 0.f, 1000.f, 1.f, 1.f);

		nk_layout_row_dynamic(ctx, 22, 1);
		nk_label(ctx, "spacing", NK_TEXT_LEFT);
		nk_property_float(ctx, "#value:", 0.f, &m_pTarget->spacing, 100.f, 1.f, 0.1f);
		nk_label(ctx, "color_factor", NK_TEXT_LEFT);
		nk_property_float(ctx, "#value:", 0.f, &m_pTarget->color_factor, 1.f, 0.01f, 0.01f);
		nk_label(ctx, "disabled_factor", NK_TEXT_LEFT);
		nk_property_float(ctx, "#value:", 0.f, &m_pTarget->disabled_factor, 1.f, 0.01f, 0.01f);
		nk_tree_pop(ctx);
	}
}
