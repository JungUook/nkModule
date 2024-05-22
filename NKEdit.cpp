#include "pch.h"
#include "NKEdit.h"

NKEdit::NKEdit()
{
	m_type = eEDIT;
	m_flags = NK_EDIT_FIELD | NK_EDIT_SIG_ENTER;
	memset(m_inputText, 0, sizeof(m_inputText));
	m_filter = nk_filter_default;

	m_pivot.x = 0.f;
	m_pivot.y = 0.f;
	m_worldTransform.x = 50.f;
	m_worldTransform.y = 50.f;
	m_worldTransform.w = 200.f;
	m_worldTransform.h = 60.f;
	m_inputTextLength = 0;
	memset(m_functionName, 0, sizeof(m_functionName));
	memset(m_argsName, 0, sizeof(m_argsName));

	std::string className = getClassName().c_str();
	m_cName = className.c_str();
	strcpy_s(m_primaryName, m_cName);
}

NKEdit::~NKEdit()
{
}

void NKEdit::Layout(nk_context* ctx)
{
	nk_flags nkFlag = nk_edit_string(ctx, m_flags, m_inputText, &m_inputTextLength, sizeof(m_inputText), m_filter);

	if (nkFlag & NK_EDIT_COMMITED)
	{
		CallEvent(NK_EDIT_COMMITED);
	}
}

void NKEdit::RegistFunction(const char* functionName, const char* argsName)
{
	if (functionName != nullptr && strlen(functionName) > 0)
	{
		strcpy_s(m_functionName, functionName);
	}

	if (argsName != nullptr && strlen(argsName) > 0)
	{
		strcpy_s(m_argsName, argsName);
	}
}

void NKEdit::Clear()
{
	memset(m_inputText, 0, sizeof(m_inputText));
	m_inputTextLength = 0;
}

void NKEdit::CallEvent(nk_edit_events edit_event)
{
	if (m_functionName != nullptr && strlen(m_functionName) > 0)
	{
		if (m_argsName != nullptr && strlen(m_argsName) > 0)
		{
			luabridge::LuaRef table = m_manager->GetLuaTable(m_argsName);
			
			switch (edit_event)
			{
			case NK_EDIT_ACTIVE: {
				std::string inputText(m_inputText, m_inputTextLength);
				table["NK_EDIT_ACTIVE"] = inputText;
			}
			break;
			case NK_EDIT_INACTIVE: {
				//table["NK_EDIT_INACTIVE"] = m_inputText;
			}
			break;
			case NK_EDIT_ACTIVATED: {
				//table["NK_EDIT_ACTIVATED"] = m_inputText;
			}
			break;
			case NK_EDIT_DEACTIVATED: {
				//table["NK_EDIT_DEACTIVATED"] = m_inputText;
			}
			break;
			case NK_EDIT_COMMITED: {
				std::string inputText(m_inputText, m_inputTextLength);
				table["NK_EDIT_COMMITED"] = inputText;
			}
			break;
			default:
				break;
			}

			m_manager->RunFunctionArgs(m_functionName, table);
		}
		else
		{
			m_manager->RunFunction(m_functionName);
		}
	}
}
