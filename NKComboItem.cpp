#include "pch.h"
#include "NKComboItem.h"
#include "NKCombo.h"

NKComboItem::NKComboItem()
{
	m_type = eCOMBO_ITEM;
	m_flags = NK_TEXT_CENTERED;
	m_labelNumber = 0;

	m_pivot.x = 0.f;
	m_pivot.y = 0.f;
	m_worldTransform.x = 50.f;
	m_worldTransform.y = 50.f;
	m_worldTransform.w = 150.f;
	m_worldTransform.h = 60.f;

	memset(m_content, 0, sizeof(m_content));
	memset(m_functionName, 0, sizeof(m_functionName));
	memset(m_argsName, 0, sizeof(m_argsName));

	std::string className = getClassName().c_str();
	m_cName = className.c_str();
	strcpy_s(m_primaryName, m_cName);
}

NKComboItem::~NKComboItem()
{
}

void NKComboItem::Layout(nk_context* ctx)
{
	if (nk_combo_item_label(ctx, m_content, m_flags))
	{
		NKCombo* pParent = (NKCombo*) m_pParent;
		pParent->SetCurrentLabel(m_labelNumber);
		pParent->SetComboName(m_content);
		CallEvent();
	}
}

void NKComboItem::SetComboName(const char* name)
{
	strcpy_s(m_content, name);
}

void NKComboItem::RegistFunction(const char* functionName, const char* argsName)
{
	if (strlen(functionName) > 0)
	{
		strcpy_s(m_functionName, functionName);
	}
	if (strlen(argsName) > 0)
	{
		strcpy_s(m_argsName, argsName);
	}
}

void NKComboItem::SetLabel(int number)
{
	m_labelNumber = number;
}

void NKComboItem::CallEvent()
{
	if (strlen(m_functionName) > 0)
	{
		if (strlen(m_argsName) > 0)
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