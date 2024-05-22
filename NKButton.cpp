#include "pch.h"
#include "NKButton.h"

NKButton::NKButton()
{
	m_type = eBUTTON;

	m_pivot.x = 0.f;
	m_pivot.y = 0.f;
	m_worldTransform.x = 0.f;
	m_worldTransform.y = 0.f;
	m_worldTransform.w = 150.f;
	m_worldTransform.h = 40.f;

	memset(m_content, 0, sizeof(m_content));
	memset(m_functionName, 0, sizeof(m_functionName));
	memset(m_argsName, 0, sizeof(m_argsName));
}

NKButton::~NKButton()
{
}

void NKButton::Layout(nk_context* ctx)
{
	if (nk_button_label_styled(ctx, &m_ctx->style.button, m_content))
	{
		CallEvent();
	}
}

void NKButton::SetButtonName(const char* name)
{
	strcpy_s(m_content, name);
}

void NKButton::RegistFunction(const char* functionName, const char* argsName)
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

void NKButton::CallEvent()
{
	if (m_functionName != nullptr && strlen(m_functionName) > 0)
	{
		if (m_argsName != nullptr && strlen(m_argsName) > 0)
		{
			luabridge::LuaRef table = m_manager->GetLuaTable(m_argsName);
			m_manager->RunFunctionArgs(m_functionName, table);
		}
		else
		{
			m_manager->RunFunction(m_functionName);
		}
	}
}
