#include "pch.h"
#include "NKBaseStyle.h"
#include "NuklearUI.h"

NKBaseStyle::NKBaseStyle()
{
	m_font = nullptr;
	m_pParentStyle = nullptr;
	m_followParentStyle = nk_true;
}

NKBaseStyle::NKBaseStyle(const NKBaseStyle& other)
{
	m_style = other.m_style;
	m_font = other.m_font;
	m_pParentStyle = other.m_pParentStyle;
	m_followParentStyle = other.m_followParentStyle;
}

NKBaseStyle::~NKBaseStyle()
{
}

void NKBaseStyle::InitializeStyle(nk_context* ctx, NuklearUI* pManager)
{
	m_font = pManager->GetFont();
	m_style = ctx->style;
	m_pParentStyle = nullptr;
}

void NKBaseStyle::InitializeStyle(nk_font* font, nk_style& parentStyle, nk_style* parent_of_parentStyle)
{
	m_font = font;
	m_style = parentStyle;
	m_pParentStyle = parent_of_parentStyle != nullptr ? parent_of_parentStyle : &parentStyle;
}

void NKBaseStyle::StyleUpdateStart(nk_context* ctx, nk_style& original, NKBaseStyle* pParent)
{
	original = ctx->style;
	ctx->style = pParent != nullptr && m_followParentStyle ? *m_pParentStyle : m_style;
}

void NKBaseStyle::StyleUpdateEnd(nk_context* ctx, nk_style& original)
{
	ctx->style = original;
}

void NKBaseStyle::SetStyle(nk_style* style)
{
	CHECK_PTR(style);
	m_style = *style;
}

void NKBaseStyle::Setfont(nk_font* font)
{
	CHECK_PTR(font);
	m_font = font;
}

void NKBaseStyle::SetBackground(NuklearUI* pManager, int SID)
{
	struct nk_image* img = pManager->SearchImage(SID);

	if (img)
	{
		m_style.window.fixed_background = nk_style_item_image(*img);
	}
}

void NKBaseStyle::FollowParentStyle(nk_context* ctx, NKBaseStyle* pParent)
{
	nk_layout_row_dynamic(ctx, 22, 1);
	nk_checkbox_label(ctx, "follow_parent_style", &m_followParentStyle);

	if (m_followParentStyle) {
		if (pParent != nullptr) {

			if (pParent->m_pParentStyle != nullptr) {
				m_pParentStyle = pParent->m_pParentStyle;
			}
			else {
				m_pParentStyle = &pParent->m_style;
			}

		}
		else {
			m_pParentStyle = nullptr;
		}
	}
	else {
		m_pParentStyle = nullptr;
	}
}
