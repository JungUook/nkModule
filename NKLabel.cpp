#include "pch.h"
#include "NKLabel.h"

NKLabel::NKLabel() : NKBase(), NKBaseLabel(), NKStyleText()
{
	m_type = eLABEL;
	m_flags = NK_TEXT_CENTERED;
}

NKLabel::NKLabel(nk_context* ctx, NuklearUI* pManager) : NKBase(ctx, pManager), NKBaseLabel(), NKStyleText(ctx, &m_style)
{
	m_type = eLABEL;
	m_flags = NK_TEXT_CENTERED;

	m_cTransform.x = 50.f;
	m_cTransform.y = 50.f;
	m_cTransform.w = 150.f;
	m_cTransform.h = 60.f;
}

NKLabel::NKLabel(const NKLabel& other) : NKBase(other), NKBaseLabel(other), NKStyleText(other, m_ctx, &m_style)
{
	m_type = other.m_type;
	m_flags = other.m_flags;
}

NKLabel::~NKLabel()
{
}

void NKLabel::Layout(nk_context* ctx)
{
	nk_label(ctx, m_cContent, m_flags);
}

void NKLabel::SafeRenderStart(nk_context* ctx)
{
	UpdateComponent(ctx, m_pManager);
}

void NKLabel::SafeRenderEnd(nk_context* ctx)
{
}

void NKLabel::EditInfo(nk_context* ctx)
{
	EditLabel(ctx, m_pManager);
}

void NKLabel::EditStyle(nk_context* ctx)
{
	NKBase::EditStyle(ctx);
	EditComponentStyle(ctx, m_pManager);
}
