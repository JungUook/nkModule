#include "pch.h"
#include "NKComboItem.h"
#include "NKCombo.h"

NKComboItem::NKComboItem() : NKBase()
{
	m_type = eCOMBO_ITEM;
	m_flags = NK_TEXT_CENTERED;
	m_labelNumber = 0;

	m_worldTransform.x = 0.f;
	m_worldTransform.y = 0.f;
	m_worldTransform.w = 100;
	m_worldTransform.h = 22.f;

	memset(m_content, 0, sizeof(m_content));
}

NKComboItem::~NKComboItem()
{
}

void NKComboItem::Layout(nk_context* ctx)
{
	NKCombo* parent = (NKCombo*)m_pParent;
	if (parent)
	{
		if (nk_combo_item_label(ctx, m_content, parent->m_labelAlignment))
		{
			parent->SetCurrentLabel(m_labelNumber);
			parent->SetComboName(m_content);
			CallEvent(m_pManager);
		}
	}
	else {

	}
}

void NKComboItem::EditInfo()
{
	float ratio[2];
	ratio[0] = 0.3f;
	ratio[1] = 0.7f;
	nk_layout_row(m_ctx, NK_DYNAMIC, 44, 2, ratio);
	nk_label(m_ctx, "Text: ", NK_TEXT_LEFT);
	nk_flags result = m_pManager->IMEInputSystem(m_ctx, NK_EDIT_FIELD | NK_EDIT_SIG_ENTER, m_cEditName, sizeof(m_cEditName), nk_filter_default, &m_cEditName_len);
	if (result & NK_EDIT_COMMITED) {
		SetComboName(m_cEditName);
	}
}

void NKComboItem::SetComboName(const char* name)
{
	strcpy_s(m_content, name);
}

void NKComboItem::SetLabel(int number)
{
	m_labelNumber = number;
}