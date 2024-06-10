#include "pch.h"
#include "NKComboItem.h"
#include "NKCombo.h"

NKComboItem::NKComboItem() : NKBase()
{
	m_type = eCOMBO_ITEM;
	m_flags = NK_TEXT_CENTERED;
	m_labelNumber = 0;

	m_pivot.x = 0.f;
	m_pivot.y = 0.f;
	m_worldTransform.x = 0.f;
	m_worldTransform.y = 0.f;
	m_worldTransform.w = 100;
	m_worldTransform.h = 22.f;

	memset(m_content, 0, sizeof(m_content));
	memset(m_functionName, 0, sizeof(m_functionName));
	memset(m_argsName, 0, sizeof(m_argsName));
}

NKComboItem::~NKComboItem()
{
}

void NKComboItem::Layout(nk_context* ctx)
{
	NKCombo* parent = (NKCombo*)m_pParent;
	if (parent)
	{
		if (nk_combo_item_label(ctx, m_content, parent->m_labelAlignment))
		{
			parent->SetCurrentLabel(m_labelNumber);
			parent->SetComboName(m_content);
			CallEvent();
		}
	}
	else {

	}
}

void NKComboItem::EditInfo()
{
	float ratio[2];
	ratio[0] = 0.3f;
	ratio[1] = 0.7f;
	nk_layout_row(m_ctx, NK_DYNAMIC, 44, 2, ratio);
	nk_label(m_ctx, "Text: ", NK_TEXT_LEFT);
	nk_flags result = nk_edit_string(m_ctx, NK_EDIT_SIMPLE | NK_EDIT_SIG_ENTER, m_editName, &m_editName_len, 64, nk_filter_default);
	if (result & NK_EDIT_COMMITED) {
		SetComboName(m_editName);
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