#include "pch.h"
#include "NKEdit.h"

NKEdit::NKEdit() : NKBase(), NKHandler(), NKStyleEdit()
{
	m_type = eEDIT;
	m_flags = NK_EDIT_FIELD | NK_EDIT_SIG_ENTER;
	memset(m_inputText, 0, sizeof(m_inputText));
	m_filter = nk_filter_default;
	m_inputTextLength = 0;
}

NKEdit::NKEdit(nk_context* ctx, NuklearUI* pManager) : NKBase(ctx, pManager), NKHandler(), NKStyleEdit(ctx, &m_style)
{
	m_type = eEDIT;
	m_flags = NK_EDIT_FIELD | NK_EDIT_SIG_ENTER;
	memset(m_inputText, 0, sizeof(m_inputText));
	m_filter = nk_filter_default;

	m_cTransform.w = 200.f;
	m_cTransform.h = 60.f;
	m_inputTextLength = 0;
}

NKEdit::NKEdit(const NKEdit& other) : NKBase(other), NKHandler(other), NKStyleEdit(other, m_ctx, &m_style)
{
	m_type = other.m_type;
	m_flags = other.m_flags;
	strcpy_s(m_inputText, other.m_inputText);
	m_filter = other.m_filter;

	m_inputTextLength = other.m_inputTextLength;
}

NKEdit::~NKEdit()
{
}

void NKEdit::Layout(nk_context* ctx)
{
	nk_flags nkFlag = m_pManager->IMEInputSystem(ctx, m_inputText, sizeof(m_inputText), &m_inputTextLength, m_flags, m_filter);

	if (nkFlag & NK_EDIT_COMMITED)
	{
		CallEvent(m_pLuaManager, NK_EDIT_COMMITED, m_inputText, &m_inputTextLength);
	}
}

void NKEdit::SafeRenderStart(nk_context* ctx)
{
	UpdateComponent(ctx, m_pManager);
}

void NKEdit::SafeRenderEnd(nk_context* ctx)
{
}

void NKEdit::EditInfo(nk_context* ctx)
{
	EditInfoData(ctx, m_pManager, m_pLuaManager);
}

void NKEdit::EditStyle(nk_context* ctx)
{
	NKBase::EditStyle(ctx);
	EditComponentStyle(ctx, m_pManager);
}

void NKEdit::Clear()
{
	memset(m_inputText, 0, sizeof(m_inputText));
	m_inputTextLength = 0;
}

bool NKEdit::CClear(void* param)
{
	Clear();
	return true;
}

void NKEdit::SetText(const char* text)
{
	strcpy_s(m_inputText, text);
}

void NKEdit::LSetText(luabridge::LuaRef ref)
{
	CHECK_LUA_REF(ref);
	std::string text = ref.cast<std::string>();
	SetText(text.c_str());
}

bool NKEdit::CSetText(void* param)
{
	const char** text = static_cast<const char**>(param);

	if (text) {
		SetText(*text);
		return true;
	}
	return false;
}

void NKEdit::RegistCommand(const char* classname)
{
	NKBase::RegistCommand(classname);
	MAKE_INTERFACE(m_mapFunc, this, NKEdit::CClear, classname);
	MAKE_INTERFACE(m_mapFunc, this, NKEdit::CSetText, classname);
}
