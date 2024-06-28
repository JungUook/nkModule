#include "pch.h"
#include "NKComboItem.h"
#include "NKCombo.h"

NKComboItem::NKComboItem() : NKBase(), NKHandler(), NKBaseLabel()
{
	m_type = eCOMBO_ITEM;
	m_flags = NK_TEXT_CENTERED;
	m_labelNumber = 0;
}

NKComboItem::NKComboItem(nk_context* ctx, NuklearUI* pManager) : NKBase(ctx, pManager), NKHandler(), NKBaseLabel()
{
	m_type = eCOMBO_ITEM;
	m_flags = NK_TEXT_CENTERED;
	m_labelNumber = 0;

	m_cTransform.x = 0.f;
	m_cTransform.y = 0.f;
	m_cTransform.w = 100;
	m_cTransform.h = 22.f;
	SetLabel("ComboItem");
}

NKComboItem::NKComboItem(const NKComboItem& other) : NKBase(other), NKHandler(other), NKBaseLabel(other)
{
	m_type = other.m_type;
	m_flags = other.m_flags;
	m_labelNumber = other.m_labelNumber;
}

NKComboItem::~NKComboItem()
{
}

void NKComboItem::LayoutBegin(nk_context* ctx)
{
	CustomFontSizeBegin(ctx, m_font);
}

void NKComboItem::Layout(nk_context* ctx)
{
	NKCombo* parent = (NKCombo*)m_pParent;
	if (parent)
	{
		if (nk_combo_item_label(ctx, m_cContent, parent->m_labelAlignment))
		{
			parent->SetCurrentLabel(m_labelNumber);
			parent->SetComboName(m_cContent);
			CallEvent(m_pManager);
		}
	}
	else {

	}
}

void NKComboItem::LayoutEnd(nk_context* ctx)
{
	CustomFontSizeEnd(ctx, m_pManager, m_font);
}

void NKComboItem::SafeRenderStart(nk_context* ctx)
{
	//UpdateComponent(ctx, m_pManager);
}

void NKComboItem::SafeRenderEnd(nk_context* ctx)
{
}

void NKComboItem::EditInfo(nk_context* ctx)
{
	EditLabel(ctx, m_pManager);
}

void NKComboItem::SetLabelNumber(int number)
{
	m_labelNumber = number;
}