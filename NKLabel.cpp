#include "pch.h"
#include "NKLabel.h"

NKLabel::NKLabel() : NKBase(), NKBaseLabel(), NKStyleText()
{
	m_type = eLABEL;
	m_flags = NK_TEXT_CENTERED;
	m_bWrap = nk_false;
	m_bBold = nk_false;
	m_bUnderline = nk_false;
	m_bStrikethrough = nk_false;
}

NKLabel::NKLabel(nk_context* ctx, NuklearUI* pManager) : NKBase(ctx, pManager), NKBaseLabel(), NKStyleText(ctx, &m_style)
{
	m_type = eLABEL;
	m_flags = NK_TEXT_CENTERED;

	m_cTransform.w = 150.f;
	m_cTransform.h = 60.f;
	SetLabel("Label");
	m_bWrap = nk_false;
	m_bBold = nk_false;
	m_bUnderline = nk_false;
	m_bStrikethrough = nk_false;
}

NKLabel::NKLabel(const NKLabel& other) : NKBase(other), NKBaseLabel(other), NKStyleText(other, m_ctx, &m_style)
{
	m_type = other.m_type;
	m_flags = other.m_flags;
	m_bWrap = other.m_bWrap;
	m_bBold = other.m_bBold;
	m_bUnderline = other.m_bUnderline;
	m_bStrikethrough = other.m_bStrikethrough;
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
		nk_render_wrapped_label(ctx, m_sContent.c_str());
	}
	else if (m_bBold) {
		nk_label_bold(ctx, m_sContent.c_str(), NK_TEXT_LEFT);
	}
	else if (m_bUnderline) {
		nk_label_underline(ctx, m_sContent.c_str(), NK_TEXT_LEFT);
	}
	else if (m_bStrikethrough) {
		nk_label_strikethrough(ctx, m_sContent.c_str(), NK_TEXT_LEFT);
	}
	else {
		nk_label(ctx, m_sContent.c_str(), m_flags);
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
	nk_checkbox_label(ctx, "Bold", &m_bBold);
	nk_checkbox_label(ctx, "Underline", &m_bUnderline);
	nk_checkbox_label(ctx, "Strikethrough", &m_bStrikethrough);

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

void NKLabel::RegistCommand(const char* classname)
{
	NKBase::RegistCommand(classname);
	MAKE_INTERFACE(m_mapFunc, this, NKLabel::CSetLabel, classname);
}
