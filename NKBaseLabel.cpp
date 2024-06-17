#include "pch.h"
#include "NKBaseLabel.h"
#include "NuklearUI.h"

NKBaseLabel::NKBaseLabel()
{
	memset(m_cEditLabel, 0, sizeof(m_cEditLabel));
	memset(m_cContent, 0, sizeof(m_cContent));
	m_iEditLabelLen = 0;
}

NKBaseLabel::NKBaseLabel(const NKBaseLabel& other)
{
	strcpy_s(m_cEditLabel, other.m_cEditLabel);
	strcpy_s(m_cContent, other.m_cContent);
	m_iEditLabelLen = other.m_iEditLabelLen;
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
	nk_flags result = pManager->IMEInputSystem(m_cEditLabel, sizeof(m_cEditLabel), &m_iEditLabelLen);
	if (result & NK_EDIT_COMMITED) {
		SetLabel(m_cEditLabel);
	}

	return result;
}

void NKBaseLabel::SetLabel(const char* text)
{
	strcpy_s(m_cContent, text);
}
