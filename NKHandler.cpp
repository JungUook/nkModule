#include "pch.h"
#include "NKHandler.h"
#include "NuklearUI.h"

NKHandler::NKHandler()
{
	memset(m_functionName, 0, sizeof(m_functionName));
	memset(m_argsName, 0, sizeof(m_argsName));
}

NKHandler::NKHandler(const NKHandler& other)
{
	strcpy_s(m_functionName, other.m_functionName);
	strcpy_s(m_argsName, other.m_argsName);
}

NKHandler::~NKHandler()
{
}

void NKHandler::RegistFunction(const char* functionName, const char* argsName)
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

void NKHandler::CallEvent(NuklearUI* pManager)
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

void NKHandler::CallEvent(NuklearUI* pManager, nk_edit_events edit_event, char* inputText, int* inputTextLength)
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

void NKHandler::EditInfoData(nk_context* ctx)
{

}
