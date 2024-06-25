#include "pch.h"
#include "NKCombo.h"
#include "NKComboItem.h"

NKCombo::NKCombo() : NKBase(), NKStyleCombo()
{
	m_type = eCOMBO;

	m_labelSize.x = 150.f;
	m_labelSize.y = 300.f;
	m_currentLabel = 0;
	m_labelAlignment = NK_TEXT_LEFT;

	memset(m_cComboLabel, 0, sizeof(m_cComboLabel));
}

NKCombo::NKCombo(nk_context* ctx, NuklearUI* pManager) : NKBase(ctx, pManager), NKStyleCombo(ctx, &m_style)
{
	m_type = eCOMBO;

	m_cTransform.x = 50.f;
	m_cTransform.y = 50.f;
	m_cTransform.w = 150.f;
	m_cTransform.h = 60.f;
	m_labelSize.x = 150.f;
	m_labelSize.y = 300.f;
	m_currentLabel = 0;
	m_labelAlignment = NK_TEXT_LEFT;

	memset(m_cComboLabel, 0, sizeof(m_cComboLabel));
}

NKCombo::NKCombo(const NKCombo& other) : NKBase(other), NKStyleCombo(other, m_ctx, &m_style)
{
	m_type = other.m_type;

	m_labelSize.x = other.m_labelSize.x;
	m_labelSize.y = other.m_labelSize.y;
	m_currentLabel = other.m_currentLabel;
	m_labelAlignment = other.m_labelAlignment;

	memset(m_cComboLabel, 0, sizeof(m_cComboLabel));
}

NKCombo::~NKCombo()
{
}

void NKCombo::Layout(nk_context* ctx)
{
	if (nk_combo_begin_label(ctx, m_cComboLabel, m_labelSize))
	{
		nk_layout_space_begin(ctx, NK_STATIC, m_labelSize.y, m_pChildList.size());

		int i = 0;
		for (std::list<NKBase*>::iterator it = m_pChildList.begin(); it != m_pChildList.end(); ++it) {
			nk_layout_space_push(ctx, nk_rect(0, (m_labelSize.y / m_pChildList.size()) * i++, m_labelSize.x, m_labelSize.y / m_pChildList.size()));
			
			if ((*it)->GetType() == eCOMBO_ITEM)
			{
				NKComboItem* pItem = (NKComboItem*)(*it);
				pItem->SetLabelNumber(i);
			}
			(*it)->Update(ctx);
		}
		nk_layout_space_end(ctx);
		nk_combo_end(ctx);
	}
}

void NKCombo::SafeRenderStart(nk_context* ctx)
{
	UpdateComponent(ctx, m_pManager);
}

void NKCombo::SafeRenderEnd(nk_context* ctx)
{
}

void NKCombo::EditInfo(nk_context* ctx)
{
	nk_layout_row_dynamic(ctx, 22, 1);
	nk_label(ctx, "Create_UI", NK_TEXT_LEFT);
	if (nk_button_label(ctx, "ComboItem"))
	{
		CreateUI("NKComboItem");
	}

	nk_label(ctx, "alignment", NK_TEXT_LEFT);
	nk_layout_row_dynamic(ctx, 22, 3);
	if (nk_option_label(ctx, "left", m_labelAlignment == NK_TEXT_LEFT)) m_labelAlignment = NK_TEXT_LEFT;
	if (nk_option_label(ctx, "center", m_labelAlignment == NK_TEXT_CENTERED)) m_labelAlignment = NK_TEXT_CENTERED;
	if (nk_option_label(ctx, "right", m_labelAlignment == NK_TEXT_RIGHT)) m_labelAlignment = NK_TEXT_RIGHT;

	PropertyVector2(ctx, "Label Size", m_labelSize, .0f, 500.f, 0.01f, 0.01f);

	if (nk_tree_push(ctx, NK_TREE_NODE, "Combo Item List", NK_MINIMIZED)) {
		for (std::list<NKBase*>::iterator it = m_pChildList.begin(); it != m_pChildList.end(); ++it) {
			if ((*it)->GetType() == eCOMBO_ITEM)
			{
				NKComboItem* pItem = (NKComboItem*)(*it);
				pItem->EditInfo(ctx);
			}
		}
		nk_tree_pop(ctx);
	}	
}

void NKCombo::EditStyle(nk_context* ctx)
{
	EditComponentStyle(ctx, m_pManager);
}

void NKCombo::SetComboName(const char* name)
{
	strcpy_s(m_cComboLabel, name);
}

void NKCombo::SetLabelSize(float x, float y)
{
	m_labelSize.x = x;
	m_labelSize.y = y;
}

void NKCombo::SetCurrentLabel(int number)
{
	m_currentLabel = number;
}
