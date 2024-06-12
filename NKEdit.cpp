#include "pch.h"
#include "NKEdit.h"

NKEdit::NKEdit()
{
	m_type = eEDIT;
	m_flags = NK_EDIT_FIELD | NK_EDIT_SIG_ENTER;
	memset(m_inputText, 0, sizeof(m_inputText));
	m_filter = nk_filter_default;

	m_worldTransform.x = 50.f;
	m_worldTransform.y = 50.f;
	m_worldTransform.w = 200.f;
	m_worldTransform.h = 60.f;
	m_inputTextLength = 0;
}

NKEdit::~NKEdit()
{
}

void NKEdit::Layout(nk_context* ctx)
{
	nk_flags nkFlag = m_pManager->IMEInputSystem(ctx, m_flags, m_inputText, sizeof(m_inputText), m_filter, &m_cEditName_len);

	if (nkFlag & NK_EDIT_COMMITED)
	{
		CallEvent(m_pManager,NK_EDIT_COMMITED, m_inputText, &m_inputTextLength);
	}
}

void NKEdit::Clear()
{
	memset(m_inputText, 0, sizeof(m_inputText));
	m_inputTextLength = 0;
}
