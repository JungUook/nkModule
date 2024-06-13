#include "pch.h"
#include "NKButton.h"

NKButton::NKButton(nk_context* ctx, NuklearUI* pManager) : NKBase(ctx, pManager)
{
	m_type = eBUTTON;
	m_worldTransform.x = 0.f;
	m_worldTransform.y = 0.f;
	m_worldTransform.w = 150.f;
	m_worldTransform.h = 40.f;

	memset(m_content, 0, sizeof(m_content));
}

NKButton::~NKButton()
{
}

void NKButton::Layout(nk_context* ctx)
{
	if (nk_button_label(ctx, m_content))
	{
		CallEvent(m_pManager);
	}
}

void NKButton::EditInfo()
{
	float ratio[2];
	ratio[0] = 0.3f;
	ratio[1] = 0.7f;
	nk_layout_row(m_ctx, NK_DYNAMIC, 44, 2, ratio);
	nk_label(m_ctx, "Text: ", NK_TEXT_LEFT);
	nk_flags result = m_pManager->IMEInputSystem(m_ctx, NK_EDIT_FIELD | NK_EDIT_SIG_ENTER, m_cEditName, sizeof(m_cEditName), nk_filter_default, &m_cEditName_len);
	if (result & NK_EDIT_COMMITED) {
		SetButtonName(m_cEditName);
	}

}

void NKButton::SetButtonName(const char* name)
{
	strcpy_s(m_content, name);
}