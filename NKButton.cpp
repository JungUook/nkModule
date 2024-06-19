#include "pch.h"
#include "NKButton.h"

NKButton::NKButton() : NKBase(), NKHandler(), NKBaseLabel(), NKStyleButton()
{
	m_type = eBUTTON;
}

NKButton::NKButton(nk_context* ctx, NuklearUI* pManager) : NKBase(ctx, pManager), NKHandler(), NKBaseLabel(), NKStyleButton(ctx, &m_style)
{
	m_type = eBUTTON;
	m_cTransform.x = 0.f;
	m_cTransform.y = 0.f;
	m_cTransform.w = 150.f;
	m_cTransform.h = 40.f;
}

NKButton::NKButton(const NKButton& other) : NKBase(other), NKHandler(other), NKBaseLabel(other), NKStyleButton(other)
{
	m_type = eBUTTON;
}

NKButton::~NKButton()
{
}

void NKButton::Layout(nk_context* ctx)
{
	if (nk_button_label(ctx, m_cContent))
	{
		CallEvent(m_pManager);
	}
}

void NKButton::EditInfo()
{
	EditLabel(m_ctx, m_pManager);
}

void NKButton::EditStyle()
{
	NKBase::EditStyle();
	EditComponentStyle(m_ctx, m_pManager);
}
