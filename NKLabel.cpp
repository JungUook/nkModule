#include "pch.h"
#include "NKLabel.h"

NKLabel::NKLabel()
{
	m_type = eLABEL;
	m_flags = NK_TEXT_CENTERED;
	memset(m_content, 0, sizeof(m_content));

	m_worldTransform.x = 50.f;
	m_worldTransform.y = 50.f;
	m_worldTransform.w = 150.f;
	m_worldTransform.h = 60.f;
}

NKLabel::~NKLabel()
{
}

void NKLabel::Layout(nk_context* ctx)
{
	nk_label(ctx, m_content, m_flags);
}

void NKLabel::EditInfo()
{
	float ratio[2];
	ratio[0] = 0.3f;
	ratio[1] = 0.7f;
	nk_layout_row(m_ctx, NK_DYNAMIC, 44, 2, ratio);
	nk_label(m_ctx, "Text: ", NK_TEXT_LEFT);
	nk_flags result = m_pManager->IMEInputSystem(m_ctx, NK_EDIT_FIELD | NK_EDIT_SIG_ENTER, m_cEditName, sizeof(m_cEditName), nk_filter_default, &m_cEditName_len);
	if (result & NK_EDIT_COMMITED) {
		SetLabel(m_cEditName);
	}
}

void NKLabel::SetLabel(const char* text)
{
	strcpy_s(m_content, text);
}
