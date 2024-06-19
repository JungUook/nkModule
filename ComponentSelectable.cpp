#include "pch.h"
#include "ComponentSelectable.h"
#include "NuklearUI.h"

ComponentSelectable::ComponentSelectable()
{
	m_pTarget		 = nullptr;
	m_pRestore		 = nullptr;

	m_pNormal		 = nullptr;
	m_pHover		 = nullptr;
	m_pPressed		 = nullptr;

	m_pNormalActive	 = nullptr;
	m_pHoverActive	 = nullptr;
	m_pPressedActive = nullptr;

}

ComponentSelectable::ComponentSelectable(nk_style_selectable* pTarget, nk_style_selectable* pRestore)
{
	m_pTarget		 = pTarget;
	m_pRestore		 = pRestore;
					 
	m_pNormal		 = new NKStyleItem(&pTarget->normal, &pRestore->normal);
	m_pHover		 = new NKStyleItem(&pTarget->hover, &pRestore->hover);
	m_pPressed		 = new NKStyleItem(&pTarget->pressed, &pRestore->pressed);
					 
	m_pNormalActive	 = new NKStyleItem(&pTarget->normal_active, &pRestore->normal_active);
	m_pHoverActive	 = new NKStyleItem(&pTarget->hover_active, &pRestore->hover_active);
	m_pPressedActive = new NKStyleItem(&pTarget->pressed_active, &pRestore->pressed_active);
}

ComponentSelectable::ComponentSelectable(const ComponentSelectable& other)
{
	m_pTarget		 = other.m_pTarget;
	m_pRestore		 = other.m_pRestore;
					 
	m_pNormal		 = new NKStyleItem(*other.m_pNormal);
	m_pHover		 = new NKStyleItem(*other.m_pHover);
	m_pPressed		 = new NKStyleItem(*other.m_pPressed);
					 
	m_pNormalActive	 = new NKStyleItem(*other.m_pNormalActive);
	m_pHoverActive	 = new NKStyleItem(*other.m_pHoverActive);
	m_pPressedActive = new NKStyleItem(*other.m_pPressedActive);
}

ComponentSelectable::~ComponentSelectable()
{
	m_pTarget	= nullptr;
	m_pRestore	= nullptr;

	delete m_pNormal;
	delete m_pHover;
	delete m_pPressed;

	delete m_pNormalActive;
	delete m_pHoverActive;
	delete m_pPressedActive;

	m_pNormal		 = nullptr;
	m_pHover		 = nullptr;
	m_pPressed		 = nullptr;

	m_pNormalActive	 = nullptr;
	m_pHoverActive	 = nullptr;
	m_pPressedActive = nullptr;
}

void ComponentSelectable::CustomComponentsEditor(nk_context* ctx, NuklearUI* pManager)
{
	int tree_index = 100;
	int tree_index_in = 200;
	int tree_index_in_in = 300;
	intptr_t id = reinterpret_cast<intptr_t>(this);

	if (nk_tree_push_id(ctx, NK_TREE_NODE, "background(inactive)", NK_MINIMIZED, id + tree_index++)) {

		if (nk_tree_push_id(ctx, NK_TREE_NODE, "normal", NK_MINIMIZED, id + tree_index_in++)) {
			m_pNormal->ItemEditor(ctx, pManager);
			nk_tree_pop(ctx);
		}
		if (nk_tree_push_id(ctx, NK_TREE_NODE, "hover", NK_MINIMIZED, id + tree_index_in++)) {
			m_pHover->ItemEditor(ctx, pManager);
			nk_tree_pop(ctx);
		}
		if (nk_tree_push_id(ctx, NK_TREE_NODE, "pressed", NK_MINIMIZED, id + tree_index_in++)) {
			m_pPressed->ItemEditor(ctx, pManager);
			nk_tree_pop(ctx);
		}
		nk_tree_pop(ctx);
	}

	if (nk_tree_push_id(ctx, NK_TREE_NODE, "background(active)", NK_MINIMIZED, id + tree_index++)) {

		if (nk_tree_push_id(ctx, NK_TREE_NODE, "normal", NK_MINIMIZED, id + tree_index_in++)) {
			m_pNormalActive->ItemEditor(ctx, pManager);
			nk_tree_pop(ctx);
		}
		if (nk_tree_push_id(ctx, NK_TREE_NODE, "hover", NK_MINIMIZED, id + tree_index_in++)) {
			m_pHoverActive->ItemEditor(ctx, pManager);
			nk_tree_pop(ctx);
		}
		if (nk_tree_push_id(ctx, NK_TREE_NODE, "pressed", NK_MINIMIZED, id + tree_index_in++)) {
			m_pPressedActive->ItemEditor(ctx, pManager);
			nk_tree_pop(ctx);
		}
		nk_tree_pop(ctx);
	}

	if (nk_tree_push_id(ctx, NK_TREE_NODE, "text(inactive)", NK_MINIMIZED, id + tree_index++)) {

		if (nk_tree_push_id(ctx, NK_TREE_NODE, "normal", NK_MINIMIZED, id + tree_index_in++)) {
			ColorPicker(ctx, m_pTarget->text_normal);
			nk_tree_pop(ctx);
		}
		if (nk_tree_push_id(ctx, NK_TREE_NODE, "hover", NK_MINIMIZED, id + tree_index_in++)) {
			ColorPicker(ctx, m_pTarget->text_hover);
			nk_tree_pop(ctx);
		}
		if (nk_tree_push_id(ctx, NK_TREE_NODE, "pressed", NK_MINIMIZED, id + tree_index_in++)) {
			ColorPicker(ctx, m_pTarget->text_pressed);
			nk_tree_pop(ctx);
		}
		nk_tree_pop(ctx);
	}

	if (nk_tree_push_id(ctx, NK_TREE_NODE, "text(active)", NK_MINIMIZED, id + tree_index++)) {

		if (nk_tree_push_id(ctx, NK_TREE_NODE, "normal", NK_MINIMIZED, id + tree_index_in++)) {
			ColorPicker(ctx, m_pTarget->text_normal_active);
			nk_tree_pop(ctx);
		}
		if (nk_tree_push_id(ctx, NK_TREE_NODE, "hover", NK_MINIMIZED, id + tree_index_in++)) {
			ColorPicker(ctx, m_pTarget->text_hover_active);
			nk_tree_pop(ctx);
		}
		if (nk_tree_push_id(ctx, NK_TREE_NODE, "pressed", NK_MINIMIZED, id + tree_index_in++)) {
			ColorPicker(ctx, m_pTarget->text_pressed_active);
			nk_tree_pop(ctx);
		}
		if (nk_tree_push_id(ctx, NK_TREE_NODE, "background", NK_MINIMIZED, id + tree_index_in++)) {
			ColorPicker(ctx, m_pTarget->text_background);
			nk_tree_pop(ctx);
		}

		if (nk_tree_push_id(ctx, NK_TREE_NODE, "align", NK_MINIMIZED, id + tree_index_in++)) {
			if (nk_option_label(ctx, "left", m_pTarget->text_alignment == NK_HEADER_LEFT))  m_pTarget->text_alignment = NK_HEADER_LEFT;
			if (nk_option_label(ctx, "center", m_pTarget->text_alignment == NK_TEXT_CENTERED))  m_pTarget->text_alignment = NK_TEXT_CENTERED;
			if (nk_option_label(ctx, "right", m_pTarget->text_alignment == NK_HEADER_RIGHT))  m_pTarget->text_alignment = NK_HEADER_RIGHT;
			nk_tree_pop(ctx);
		}
		nk_tree_pop(ctx);
	}

	if (nk_tree_push_id(ctx, NK_TREE_NODE, "properties", NK_MINIMIZED, id + tree_index++)) {
		nk_layout_row_dynamic(ctx, 22, 1);
		nk_label(ctx, "rounding", NK_TEXT_LEFT);
		nk_property_float(ctx, "#value:", 0.f, &m_pTarget->rounding, 100.f, 1.f, 0.1f);

		PropertyVector2(ctx, "padding", m_pTarget->padding, 0.f, 1000.f, 1.f, 1.f);
		PropertyVector2(ctx, "touch_padding", m_pTarget->touch_padding, 0.f, 1000.f, 1.f, 1.f);
		PropertyVector2(ctx, "image_padding", m_pTarget->image_padding, 0.f, 1000.f, 1.f, 1.f);

		nk_layout_row_dynamic(ctx, 22, 1);
		nk_label(ctx, "color_factor", NK_TEXT_LEFT);
		nk_property_float(ctx, "#value:", 0.f, &m_pTarget->color_factor, 1.f, 0.01f, 0.01f);
		nk_label(ctx, "disabled_factor", NK_TEXT_LEFT);
		nk_property_float(ctx, "#value:", 0.f, &m_pTarget->disabled_factor, 1.f, 0.01f, 0.01f);
		nk_tree_pop(ctx);
	}
}
