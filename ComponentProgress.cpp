#include "pch.h"
#include "ComponentProgress.h"
#include "NuklearUI.h"

ComponentProgress::ComponentProgress(nk_style_progress* pTarget, nk_style_progress* pRestore)
{
	m_pTarget = pTarget;
	m_pRestore = pRestore;

	m_pNormal = new NKStyleItem(&pTarget->normal, &pRestore->normal);
	m_pHover = new NKStyleItem(&pTarget->hover, &pRestore->hover);
	m_pActive = new NKStyleItem(&pTarget->active, &pRestore->active);

	m_CursorNormal = new NKStyleItem(&pTarget->cursor_normal, &pRestore->cursor_normal);
	m_CursorHover = new NKStyleItem(&pTarget->cursor_hover, &pRestore->cursor_hover);
	m_CursorActive = new NKStyleItem(&pTarget->cursor_active, &pRestore->cursor_active);
}

ComponentProgress::ComponentProgress(const ComponentProgress& other)
{
	m_pTarget = other.m_pTarget;
	m_pRestore = other.m_pRestore;

	m_pNormal = new NKStyleItem(*other.m_pNormal);
	m_pHover = new NKStyleItem(*other.m_pHover);
	m_pActive = new NKStyleItem(*other.m_pActive);

	m_CursorNormal = new NKStyleItem(*other.m_CursorNormal);
	m_CursorHover = new NKStyleItem(*other.m_CursorHover);
	m_CursorActive = new NKStyleItem(*other.m_CursorActive);
}

ComponentProgress::~ComponentProgress()
{
	delete m_pNormal;
	delete m_pHover;
	delete m_pActive;

	delete m_CursorNormal;
	delete m_CursorHover;
	delete m_CursorActive;

	m_pNormal = nullptr;
	m_pHover = nullptr;
	m_pActive = nullptr;

	m_CursorNormal = nullptr;
	m_CursorHover = nullptr;
	m_CursorActive = nullptr;
}

void ComponentProgress::CustomComponentsEditor(nk_context* ctx, NuklearUI* pManager)
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
		nk_label(ctx, "cursor_border", NK_TEXT_LEFT);
		nk_property_float(ctx, "#value:", 0.f, &m_pTarget->cursor_border, 100.f, 1.f, 0.1f);
		nk_label(ctx, "cursor_rounding", NK_TEXT_LEFT);
		nk_property_float(ctx, "#value:", 0.f, &m_pTarget->cursor_rounding, 100.f, 1.f, 0.1f);

		PropertyVector2(ctx, "padding", m_pTarget->padding, 0.f, 1000.f, 1.f, 1.f);

		nk_layout_row_dynamic(ctx, 22, 1);
		nk_label(ctx, "color_factor", NK_TEXT_LEFT);
		nk_property_float(ctx, "#value:", 0.f, &m_pTarget->color_factor, 1.f, 0.01f, 0.01f);
		nk_label(ctx, "disabled_factor", NK_TEXT_LEFT);
		nk_property_float(ctx, "#value:", 0.f, &m_pTarget->disabled_factor, 1.f, 0.01f, 0.01f);
		nk_tree_pop(ctx);
	}
}
