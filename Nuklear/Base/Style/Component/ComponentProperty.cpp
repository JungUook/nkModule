#include "pch.h"
#include "ComponentProperty.h"
#include "NuklearUI.h"

ComponentProperty::ComponentProperty()
{
	m_pTarget	 = nullptr;
	m_pRestore	 = nullptr;

	m_pNormal	 = nullptr;
	m_pHover	 = nullptr;
	m_pActive	 = nullptr;

	m_pEdit		 = nullptr;
	m_pIncButton = nullptr;
	m_pDecButton = nullptr;

}

ComponentProperty::ComponentProperty(nk_style_property* pTarget, nk_style_property* pRestore)
{
	m_pTarget = pTarget;
	m_pRestore = pRestore;

	m_pNormal = new NKStyleItem(&pTarget->normal, &pRestore->normal);
	m_pHover = new NKStyleItem(&pTarget->hover, &pRestore->hover);
	m_pActive = new NKStyleItem(&pTarget->active, &pRestore->active);

	m_pEdit = new ComponentEdit(&pTarget->edit, &pRestore->edit);
	m_pIncButton = new ComponentButton(&pTarget->inc_button, &pRestore->inc_button);
	m_pDecButton = new ComponentButton(&pTarget->dec_button, &pRestore->dec_button);
}

ComponentProperty& ComponentProperty::operator=(const ComponentProperty& other)
{
	if (this != &other) {
		*m_pTarget = *other.m_pTarget;
		*m_pRestore = *other.m_pRestore;

		*m_pNormal = *other.m_pNormal;
		*m_pHover = *other.m_pHover;
		*m_pActive = *other.m_pActive;

		*m_pEdit = *other.m_pEdit;
		*m_pIncButton = *other.m_pIncButton;
		*m_pDecButton = *other.m_pDecButton;
	}
	return *this;
}

ComponentProperty::~ComponentProperty()
{
	delete m_pNormal;
	delete m_pHover;
	delete m_pActive;

	delete m_pEdit;
	delete m_pIncButton;
	delete m_pDecButton;

	m_pNormal = nullptr;
	m_pHover = nullptr;
	m_pActive = nullptr;

	m_pEdit = nullptr;
	m_pIncButton = nullptr;
	m_pDecButton = nullptr;
}

void ComponentProperty::UpdateComponent(nk_context* ctx, NuklearUI* pManager)
{
	m_pNormal->UpdateComponent(ctx, pManager);
	m_pHover->UpdateComponent(ctx, pManager);
	m_pActive->UpdateComponent(ctx, pManager);

	m_pEdit->UpdateComponent(ctx, pManager);
	m_pIncButton->UpdateComponent(ctx, pManager);
	m_pDecButton->UpdateComponent(ctx, pManager);
}

void ComponentProperty::CustomComponentsEditor(nk_context* ctx, NuklearUI* pManager)
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

	if (nk_tree_push_id(ctx, NK_TREE_NODE, "properties", NK_MINIMIZED, id + tree_index++)) {
		nk_layout_row_dynamic(ctx, 22, 1);
		nk_label(ctx, "rounding", NK_TEXT_LEFT);
		nk_property_float(ctx, "#value:", 0.f, &m_pTarget->rounding, 100.f, 1.f, 0.1f);
		nk_label(ctx, "border", NK_TEXT_LEFT);
		nk_property_float(ctx, "#value:", 0.f, &m_pTarget->border, 100.f, 1.f, 0.1f);
		PropertyVector2(ctx, "padding", m_pTarget->padding, 0.f, 1000.f, 1.f, 1.f);
		nk_layout_row_dynamic(ctx, 22, 1);
		nk_label(ctx, "color_factor", NK_TEXT_LEFT);
		nk_property_float(ctx, "#value:", 0.f, &m_pTarget->color_factor, 1.f, 0.01f, 0.01f);
		nk_label(ctx, "disabled_factor", NK_TEXT_LEFT);
		nk_property_float(ctx, "#value:", 0.f, &m_pTarget->disabled_factor, 1.f, 0.01f, 0.01f);
		nk_tree_pop(ctx);
	}


	if (nk_tree_push_id(ctx, NK_TREE_NODE, "edit", NK_MINIMIZED, id + tree_index++)) {
		m_pEdit->CustomComponentsEditor(ctx, pManager);
		m_pIncButton->CustomComponentsEditor(ctx, pManager);
		m_pDecButton->CustomComponentsEditor(ctx, pManager);
		nk_tree_pop(ctx);
	}
}
