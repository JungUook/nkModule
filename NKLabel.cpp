#include "pch.h"
#include "NKLabel.h"

NKLabel::NKLabel() : NKBase(), NKBaseLabel(), NKStyleText()
{
	m_type = eLABEL;
	m_flags = NK_TEXT_CENTERED;
	m_bWrap = nk_false;
}

NKLabel::NKLabel(nk_context* ctx, NuklearUI* pManager) : NKBase(ctx, pManager), NKBaseLabel(), NKStyleText(ctx, &m_style)
{
	m_type = eLABEL;
	m_flags = NK_TEXT_CENTERED;

	m_cTransform.x = 50.f;
	m_cTransform.y = 50.f;
	m_cTransform.w = 150.f;
	m_cTransform.h = 60.f;
	SetLabel("Label");
	m_bWrap = nk_false;
}

NKLabel::NKLabel(const NKLabel& other) : NKBase(other), NKBaseLabel(other), NKStyleText(other, m_ctx, &m_style)
{
	m_type = other.m_type;
	m_flags = other.m_flags;
	m_bWrap = other.m_bWrap;
}

NKLabel::~NKLabel()
{
}

void NKLabel::LayoutBegin(nk_context* ctx)
{
	CustomFontSizeBegin(ctx, m_font);
}

void NKLabel::Layout(nk_context* ctx)
{
	if (m_bWrap) {
		nk_label_wrap(ctx, m_cContent);
	}
	else {
		nk_label(ctx, m_cContent, m_flags);
	}
}

void NKLabel::LayoutEnd(nk_context* ctx)
{
	CustomFontSizeEnd(ctx, m_pManager, m_font);
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

	nk_layout_row_dynamic(ctx, 30, 1);
	nk_checkbox_label(ctx, "Wrap", &m_bWrap);

	if (!m_bWrap) {
		if (nk_option_label(ctx, "Left", m_flags == NK_TEXT_LEFT)) m_flags = NK_TEXT_LEFT;
		if (nk_option_label(ctx, "Center", m_flags == NK_TEXT_CENTERED)) m_flags = NK_TEXT_CENTERED;
		if (nk_option_label(ctx, "Right", m_flags == NK_TEXT_RIGHT)) m_flags = NK_TEXT_RIGHT;
	}

}

void NKLabel::EditStyle(nk_context* ctx)
{
	NKBase::EditStyle(ctx);
	EditComponentStyle(ctx, m_pManager);
}
