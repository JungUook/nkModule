#include "pch.h"
#include "NKHandler.h"
#include "NuklearUI.h"

NKHandler::NKHandler()
{
	memset(m_functionName, 0, sizeof(m_functionName));
	memset(m_argsName, 0, sizeof(m_argsName));
	memset(m_functionName, 0, sizeof(m_functionNameEdit));
	memset(m_argsName, 0, sizeof(m_argsNameEdit));
	m_functionNameEditLen = 0;
	m_argsNameEditLen = 0;
}

NKHandler::NKHandler(const NKHandler& other)
{
	strcpy_s(m_functionName, other.m_functionName);
	strcpy_s(m_argsName, other.m_argsName);
	strcpy_s(m_functionName, other.m_functionNameEdit);
	strcpy_s(m_argsName, other.m_argsNameEdit);
	m_functionNameEditLen = 0;
	m_argsNameEditLen = 0;
}

NKHandler::~NKHandler()
{
}

void NKHandler::RegistFunction(const char* functionName, NKLuaInterface* pInterface)
{
	if (functionName != nullptr && strlen(functionName) > 0)
	{
		pInterface->UnsubscribeFunction(m_functionName, this);
		strcpy_s(m_functionName, functionName);
		pInterface->SubscribeFunction(functionName, this);
	}
}

void NKHandler::RegistVariable(const char* argsName, NKLuaInterface* pInterface)
{
	if (argsName != nullptr && strlen(argsName) > 0)
	{
		pInterface->UnsubscribeFunction(m_argsName, this);
		strcpy_s(m_argsName, argsName);
		pInterface->SubscribeVariable(argsName, this);
	}
}

void NKHandler::CallEvent(NKLuaInterface* pManager)
{
	if (m_functionName != nullptr && strlen(m_functionName) > 0)
	{
		if (m_argsName != nullptr && strlen(m_argsName) > 0)
		{
			luabridge::LuaRef table = pManager->GetLuaTable(m_argsName);
			pManager->RunFunctionArgs(m_functionName, table);
		}
		else
		{
			pManager->RunFunction(m_functionName);
		}
	}
}

void NKHandler::CallEvent(NKLuaInterface* pManager, nk_edit_events edit_event, char* inputText, int* inputTextLength)
{
	if (m_functionName != nullptr && strlen(m_functionName) > 0)
	{
		if (m_argsName != nullptr && strlen(m_argsName) > 0)
		{
			luabridge::LuaRef table = pManager->GetLuaTable(m_argsName);

			switch (edit_event)
			{
			case NK_EDIT_ACTIVE: {
				std::string inputText(inputText, *inputTextLength);
				table["NK_EDIT_ACTIVE"] = inputText;
				break;
			}
			//case NK_EDIT_INACTIVE: {
			//	table["NK_EDIT_INACTIVE"] = inputText;
			//	break;
			//}
			//case NK_EDIT_ACTIVATED: {
			//	table["NK_EDIT_ACTIVATED"] = inputText;
			//	break;
			//}
			//case NK_EDIT_DEACTIVATED: {
			//	table["NK_EDIT_DEACTIVATED"] = inputText;
			//	break;
			//}
			case NK_EDIT_COMMITED: {
				std::string inputText(inputText, *inputTextLength);
				table["NK_EDIT_COMMITED"] = inputText;
				break;
			}
			default:
				break;
			}

			pManager->RunFunctionArgs(m_functionName, table);
		}
		else
		{
			pManager->RunFunction(m_functionName);
		}
	}
}

void NKHandler::CallbackEvent(NKLuaInterface* pManager)
{
}

const char* NKHandler::GetFunctionName()
{
	return m_functionName;
}

const char* NKHandler::GetArgsName()
{
	return m_argsName;
}

void NKHandler::EditInfoData(nk_context* ctx, NuklearUI* pManager, NKLuaInterface* pInterface)
{
	float ratio[2];
	ratio[0] = 0.3f;
	ratio[1] = 0.7f;
	nk_layout_row(ctx, NK_DYNAMIC, 44, 2, ratio);
	nk_label(ctx, "Function: ", NK_TEXT_LEFT);
	nk_flags fresult = pManager->IMEInputSystem(ctx, m_functionNameEdit, sizeof(m_functionNameEdit), &m_functionNameEditLen);
	if (fresult & NK_EDIT_COMMITED) {
		RegistFunction(m_functionNameEdit, pInterface);
	}

	if (strlen(m_functionName) > 0) {
		nk_layout_row_dynamic(ctx, 33, 1);
		if (pInterface->IsActiveFunction(m_functionName)) {
			nk_color origin = ctx->style.text.color;
			ctx->style.text.color = nk_color(0, 255, 0, 255);
			nk_label(ctx, "Connection successful", NK_TEXT_RIGHT);
			ctx->style.text.color = origin;
		}
		else {
			nk_color origin = ctx->style.text.color;
			ctx->style.text.color = nk_color(0, 0, 255, 255);
			nk_label(ctx, "Connection failed", NK_TEXT_RIGHT);
			ctx->style.text.color = origin;
		}
	}

	nk_layout_row(ctx, NK_DYNAMIC, 44, 2, ratio);
	nk_label(ctx, "Variable: ", NK_TEXT_LEFT);
	nk_flags vresult = pManager->IMEInputSystem(ctx, m_argsNameEdit, sizeof(m_argsNameEdit), &m_argsNameEditLen);
	if (vresult & NK_EDIT_COMMITED) {
		RegistVariable(m_argsNameEdit, pInterface);
	}

	if (strlen(m_argsName) > 0) {
		nk_layout_row_dynamic(ctx, 33, 1);
		if (pInterface->IsActiveVariable(m_argsName)) {
			nk_color origin = ctx->style.text.color;
			ctx->style.text.color = nk_color(0, 255, 0, 255);
			nk_label(ctx, "Connection successful", NK_TEXT_RIGHT);
			ctx->style.text.color = origin;
		}
		else {
			nk_color origin = ctx->style.text.color;
			ctx->style.text.color = nk_color(0, 0, 255, 255);
			nk_label(ctx, "Connection failed", NK_TEXT_RIGHT);
			ctx->style.text.color = origin;
		}
	}
}
