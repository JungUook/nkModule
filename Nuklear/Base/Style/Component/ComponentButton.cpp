#include "pch.h"
#include "ComponentButton.h"
#include "NuklearUI.h"

ComponentButton::ComponentButton()
{
	m_pTarget  = nullptr;
	m_pRestore = nullptr;

	m_pNormal  = nullptr;
	m_pHover   = nullptr;
	m_pActive  = nullptr;
	m_bDisabled = false;
}

ComponentButton::ComponentButton(nk_style_button* pTarget, nk_style_button* pRestore)
{
	m_pTarget = pTarget;
	m_pRestore = pRestore;

	m_pNormal = new NKStyleItem(&pTarget->normal, &pRestore->normal);
	m_pHover = new NKStyleItem(&pTarget->hover, &pRestore->hover);
	m_pActive = new NKStyleItem(&pTarget->active, &pRestore->active);
	m_bDisabled = false;
}

ComponentButton& ComponentButton::operator=(const ComponentButton& other)
{
	if (this != &other) {
		*m_pTarget = *other.m_pTarget;
		*m_pRestore = *other.m_pRestore;

		*m_pNormal = *other.m_pNormal;
		*m_pHover = *other.m_pHover;
		*m_pActive = *other.m_pActive;
		m_bDisabled = other.m_bDisabled;
	}
	return *this;
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

void ComponentButton::UpdateComponent(nk_context* ctx, NuklearUI* pManager)
{
	m_pNormal->UpdateComponent(ctx, pManager);
	m_pHover->UpdateComponent(ctx, pManager);
	m_pActive->UpdateComponent(ctx, pManager);
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
		if (nk_tree_push_id(ctx, NK_TREE_NODE, "disabled", NK_MINIMIZED, reinterpret_cast<intptr_t>(this) + tree_index_in++)) {
			EditDisablePath(ctx, pManager);
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

void ComponentButton::DisableButton(bool bDisabled)
{
	m_bDisabled = bDisabled;

	m_pNormal->DisableButton(m_bDisabled);
	m_pHover->DisableButton(m_bDisabled);
	m_pActive->DisableButton(m_bDisabled);
}

void ComponentButton::EditDisablePath(nk_context* ctx, NuklearUI* pManager)
{
	auto mapSpr = pManager->GetSprMap();
	int size = mapSpr->size();


	nk_layout_row_dynamic(ctx, 22, 2);
	nk_label(ctx, "Selected:", NK_TEXT_LEFT);
	std::filesystem::path filePath(m_sDisablePath.c_str());
	nk_label(ctx, filePath.filename().string().c_str(), NK_TEXT_RIGHT);
	if (nk_button_label(ctx, "apply"))
	{
		m_pNormal->EditDisablePath(m_sDisablePath.c_str());
		m_pHover->EditDisablePath(m_sDisablePath.c_str());
		m_pActive->EditDisablePath(m_sDisablePath.c_str());
	}

	if (size > 0)
	{
		static char selectedFilename[260] = { 0, };
		float ratio[2] = { 0.7f, 0.3f };
		static char SearchFunction[256] = { 0, };
		static int SearchFunction_Len = 0;
		nk_layout_row(ctx, NK_DYNAMIC, 40, 2, ratio);
		nk_flags searchResult = pManager->IMEInputSystem(ctx, SearchFunction, sizeof(SearchFunction), &SearchFunction_Len);

		if (nk_button_label(ctx, "Search") | searchResult & NK_EDIT_COMMITED) {

		}

		nk_layout_row_dynamic(ctx, 300, 1);
		if (nk_group_begin(ctx, "SPR List", NK_WINDOW_TITLE)) {

			float ratio[2] = { 0.8f, 0.2f };
			nk_layout_row(ctx, NK_DYNAMIC, 22, 2, ratio);

			for (auto it = mapSpr->begin(); it != mapSpr->end(); ++it) {
				std::filesystem::path filePath((*it).first.c_str());

				bool bSearch = false;
				bool bSearchResult = true;
				if (strlen(SearchFunction) > 0) {
					bSearch = true;
				}

				if (bSearch) {
					std::wstring word = NKLuaInterface::utf8ToWstring(filePath.filename().string().c_str());
					std::wstring filter = NKLuaInterface::utf8ToWstring(SearchFunction);

					// word를 소문자로 변환
					std::transform(word.begin(), word.end(), word.begin(), towlower);
					// filter를 소문자로 변환
					std::transform(filter.begin(), filter.end(), filter.begin(), towlower);

					bSearchResult = word.find(filter) != std::wstring::npos;
				}

				if (!bSearchResult) {
					continue;
				}


				nk_label(ctx, filePath.filename().string().c_str(), NK_TEXT_LEFT);

				if (nk_button_label(ctx, "Load")) {
					m_sDisablePath = (*it).first;
				}
			}
			nk_group_end(ctx);
		}
	}
}
