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
	SetLabel("Button");
}

NKButton::NKButton(const NKButton& other) : NKBase(other), NKHandler(other), NKBaseLabel(other), NKStyleButton(other, m_ctx, &m_style)
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

void NKButton::SafeRenderStart(nk_context* ctx)
{
	UpdateComponent(ctx, m_pManager);
}

void NKButton::SafeRenderEnd(nk_context* ctx)
{
}

void NKButton::EditInfo(nk_context* ctx)
{
	EditLabel(ctx, m_pManager);
}

void NKButton::EditStyle(nk_context* ctx)
{
	NKBase::EditStyle(ctx);
	EditComponentStyle(ctx, m_pManager);
}
