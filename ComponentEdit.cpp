#include "pch.h"
#include "ComponentEdit.h"
#include "NuklearUI.h"

ComponentEdit::ComponentEdit(nk_style_edit* pTarget, nk_style_edit* pRestore)
{
	m_pTarget = pTarget;
	m_pRestore = pRestore;

	m_pNormal = new NKStyleItem(&pTarget->normal, &pRestore->normal);
	m_pHover = new NKStyleItem(&pTarget->hover, &pRestore->hover);
	m_pActive = new NKStyleItem(&pTarget->active, &pRestore->active);

	m_pScrollbar = new ComponentScrollbar(&pTarget->scrollbar, &pRestore->scrollbar);
}

ComponentEdit::~ComponentEdit()
{
	delete m_pNormal;
	delete m_pHover;
	delete m_pActive;

	delete m_pScrollbar;

	m_pNormal = nullptr;
	m_pHover = nullptr;
	m_pActive = nullptr;

	m_pScrollbar = nullptr;
}

void ComponentEdit::CustomComponentsEditor(nk_context* ctx, NuklearUI* pManager)
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
		m_pScrollbar->CustomComponentsEditor(ctx, pManager);
		nk_tree_pop(ctx);
	}

	if (nk_tree_push_id(ctx, NK_TREE_NODE, "cursor", NK_MINIMIZED, id + tree_index++)) {

		if (nk_tree_push_id(ctx, NK_TREE_NODE, "normal", NK_MINIMIZED, id + tree_index_in++)) {
			ColorPicker(ctx, m_pTarget->cursor_normal);
			nk_tree_pop(ctx);
		}
		if (nk_tree_push_id(ctx, NK_TREE_NODE, "hover", NK_MINIMIZED, id + tree_index_in++)) {
			ColorPicker(ctx, m_pTarget->cursor_hover);
			nk_tree_pop(ctx);
		}
		if (nk_tree_push_id(ctx, NK_TREE_NODE, "text_normal", NK_MINIMIZED, id + tree_index_in++)) {
			ColorPicker(ctx, m_pTarget->cursor_text_normal);
			nk_tree_pop(ctx);
		}
		if (nk_tree_push_id(ctx, NK_TREE_NODE, "text_hover", NK_MINIMIZED, id + tree_index_in++)) {
			ColorPicker(ctx, m_pTarget->cursor_text_hover);
			nk_tree_pop(ctx);
		}
		nk_tree_pop(ctx);
	}

	if (nk_tree_push_id(ctx, NK_TREE_NODE, "text(unselected)", NK_MINIMIZED, id + tree_index++)) {

		if (nk_tree_push_id(ctx, NK_TREE_NODE, "normal", NK_MINIMIZED, id + tree_index_in++)) {
			ColorPicker(ctx, m_pTarget->text_normal);
			nk_tree_pop(ctx);
		}
		if (nk_tree_push_id(ctx, NK_TREE_NODE, "hover", NK_MINIMIZED, id + tree_index_in++)) {
			ColorPicker(ctx, m_pTarget->text_hover);
			nk_tree_pop(ctx);
		}
		if (nk_tree_push_id(ctx, NK_TREE_NODE, "active", NK_MINIMIZED, id + tree_index_in++)) {
			ColorPicker(ctx, m_pTarget->text_active);
			nk_tree_pop(ctx);
		}
		nk_tree_pop(ctx);
	}

	if (nk_tree_push_id(ctx, NK_TREE_NODE, "text(selected)", NK_MINIMIZED, id + tree_index++)) {

		if (nk_tree_push_id(ctx, NK_TREE_NODE, "normal", NK_MINIMIZED, id + tree_index_in++)) {
			ColorPicker(ctx, m_pTarget->selected_normal);
			nk_tree_pop(ctx);
		}
		if (nk_tree_push_id(ctx, NK_TREE_NODE, "hover", NK_MINIMIZED, id + tree_index_in++)) {
			ColorPicker(ctx, m_pTarget->selected_hover);
			nk_tree_pop(ctx);
		}
		if (nk_tree_push_id(ctx, NK_TREE_NODE, "text_normal", NK_MINIMIZED, id + tree_index_in++)) {
			ColorPicker(ctx, m_pTarget->selected_text_normal);
			nk_tree_pop(ctx);
		}
		if (nk_tree_push_id(ctx, NK_TREE_NODE, "text_hover", NK_MINIMIZED, id + tree_index_in++)) {
			ColorPicker(ctx, m_pTarget->selected_text_hover);
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
		nk_label(ctx, "cursor_size", NK_TEXT_LEFT);
		nk_property_float(ctx, "#value:", 0.f, &m_pTarget->cursor_size, 100.f, 1.f, 0.1f);

		PropertyVector2(ctx, "scrollbar_size", m_pTarget->scrollbar_size, 0.f, 1000.f, 1.f, 1.f);
		PropertyVector2(ctx, "padding", m_pTarget->padding, 0.f, 1000.f, 1.f, 1.f);

		nk_layout_row_dynamic(ctx, 22, 1);
		nk_label(ctx, "row_padding", NK_TEXT_LEFT);
		nk_property_float(ctx, "#value:", 0.f, &m_pTarget->row_padding, 100.f, 1.f, 0.1f);
		nk_label(ctx, "color_factor", NK_TEXT_LEFT);
		nk_property_float(ctx, "#value:", 0.f, &m_pTarget->color_factor, 1.f, 0.01f, 0.01f);
		nk_label(ctx, "disabled_factor", NK_TEXT_LEFT);
		nk_property_float(ctx, "#value:", 0.f, &m_pTarget->disabled_factor, 1.f, 0.01f, 0.01f);
		nk_tree_pop(ctx);
	}
}
