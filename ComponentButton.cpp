#include "pch.h"
#include "ComponentButton.h"
#include "NuklearUI.h"

ComponentButton::ComponentButton(nk_style_button* pTarget, nk_style_button* pRestore)
{
	m_pTarget = pTarget;
	m_pRestore = pRestore;

	m_pNormal = new NKStyleItem(&pTarget->normal, &pRestore->normal);
	m_pHover = new NKStyleItem(&pTarget->hover, &pRestore->hover);
	m_pActive = new NKStyleItem(&pTarget->active, &pRestore->active);
}

ComponentButton::ComponentButton(const ComponentButton& other)
{
	m_pTarget = other.m_pTarget;
	m_pRestore = other.m_pRestore;

	m_pNormal = new NKStyleItem(*other.m_pNormal);
	m_pHover = new NKStyleItem(*other.m_pHover);
	m_pActive = new NKStyleItem(*other.m_pActive);
}

ComponentButton::~ComponentButton()
{
	delete m_pNormal;
	delete m_pHover;
	delete m_pActive;

	m_pNormal = nullptr;
	m_pHover = nullptr;
	m_pActive = nullptr;
}

void ComponentButton::CustomComponentsEditor(nk_context* ctx, NuklearUI* pManager)
{
	int tree_index = 100;
	int tree_index_in = 200;
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
		if (nk_tree_push_id(ctx, NK_TREE_NODE, "color_factor", NK_MINIMIZED, reinterpret_cast<intptr_t>(this) + tree_index_in++)) {
			nk_layout_row_dynamic(ctx, 22, 1);
			nk_property_float(ctx, "#color:", 0.f, &m_pTarget->color_factor_background, 1.f, 0.01f, 0.01f);
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
		if (nk_tree_push_id(ctx, NK_TREE_NODE, "color_factor", NK_MINIMIZED, reinterpret_cast<intptr_t>(this) + tree_index_in++)) {
			nk_layout_row_dynamic(ctx, 22, 1);
			nk_property_float(ctx, "#color:", 0.f, &m_pTarget->color_factor_text, 1.f, 0.01f, 0.01f);
			nk_tree_pop(ctx);
		}
		nk_tree_pop(ctx);
	}

	if (nk_tree_push_id(ctx, NK_TREE_NODE, "properties", NK_MINIMIZED, reinterpret_cast<intptr_t>(this) + tree_index++)) {
		nk_layout_row_dynamic(ctx, 22, 1);
		nk_property_float(ctx, "#Rounding:", 0.f, &m_pTarget->rounding, 100.f, 1.f, 0.1f);

		PropertyVector2(ctx, "padding", m_pTarget->padding, 0.f, 1000.f, 1.f, 1.f);
		PropertyVector2(ctx, "image_padding", m_pTarget->image_padding, 0.f, 1000.f, 1.f, 1.f);
		PropertyVector2(ctx, "touch_padding", m_pTarget->touch_padding, 0.f, 1000.f, 1.f, 1.f);

		nk_layout_row_dynamic(ctx, 22, 1);
		nk_label(ctx, "disabled_factor", NK_TEXT_LEFT);
		nk_property_float(ctx, "#value:", 0.f, &m_pTarget->disabled_factor, 1.f, 0.01f, 0.01f);
		nk_tree_pop(ctx);
	}
}
