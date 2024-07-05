#include "pch.h"
#include "NKBaseLabel.h"
#include "NuklearUI.h"

NKBaseLabel::NKBaseLabel()
{
	memset(m_cEditLabel, 0, sizeof(m_cEditLabel));
	memset(m_cContent, 0, sizeof(m_cContent));
	m_iEditLabelLen = 0;
	m_fScale = 1.f;
}

NKBaseLabel::NKBaseLabel(const NKBaseLabel& other)
{
	strcpy_s(m_cEditLabel, other.m_cEditLabel);
	strcpy_s(m_cContent, other.m_cContent);
	m_iEditLabelLen = other.m_iEditLabelLen;
	m_fScale = other.m_fScale;
}

NKBaseLabel::~NKBaseLabel()
{
}

nk_flags NKBaseLabel::EditLabel(nk_context* ctx, NuklearUI* pManager)
{
	float ratio[2];
	ratio[0] = 0.3f;
	ratio[1] = 0.7f;
	nk_layout_row(ctx, NK_DYNAMIC, 44, 2, ratio);
	nk_label(ctx, "Text: ", NK_TEXT_LEFT);
	nk_flags result = pManager->IMEInputSystem(ctx, m_cEditLabel, sizeof(m_cEditLabel), &m_iEditLabelLen);
	if (result & NK_EDIT_COMMITED) {
		SetLabel(m_cEditLabel);
	}

	nk_layout_row_dynamic(ctx, 44, 1);
	nk_slider_float(ctx, 0.1f, &m_fScale, 2.f, 0.01f);
	nk_property_float(ctx, "#Font size", 0.1f, &m_fScale, 2.f, 0.1f, 0.01f);
	return result;
}

void NKBaseLabel::SetLabel(const char* text)
{
	strcpy_s(m_cContent, text);
}

void NKBaseLabel::LSetLabel(luabridge::LuaRef ref)
{
	CHECK_LUA_REF(ref);
	std::string text = ref.cast<std::string>();
	SetLabel(text.c_str());
}

bool NKBaseLabel::CSetLabel(void* param)
{
	const char** text = static_cast<const char**>(param);

	if (text) {
		SetLabel(*text);
		return true;
	}
	return false;
}

void NKBaseLabel::CustomFontSizeBegin(nk_context* ctx, nk_font* font)
{
	font->handle.height *= m_fScale;
	nk_style_set_font(ctx, &font->handle);
}

void NKBaseLabel::CustomFontSizeEnd(nk_context* ctx, NuklearUI* pManager, nk_font* font)
{
	font->handle.height = pManager->GetOriginalFontSize();
	nk_style_set_font(ctx, &font->handle);
}
