#include "pch.h"
#include "NuklearEditor.h"
#include "UiLibrary.h"

#include <iostream>
#include <vector>
#include <algorithm>
#include <locale>
#include <string>
#include <windows.h>

NuklearEditor::NuklearEditor()
{
	m_pManager = nullptr;

	m_vecObject = nullptr;
	m_vecModule = nullptr;
	m_mapModuleID = nullptr;
	m_mapModuleName = nullptr;
	m_mapImage = nullptr;
	m_mapSpr = nullptr;

	m_option = 0;
	m_selectedNode = nullptr;
	m_deletedNode = nullptr;
	m_vecVariable = nullptr;
	m_vecFunction = nullptr;
	m_show_popup = 0;
	memset(m_popup_content, 0, sizeof(m_popup_content));
}

NuklearEditor::~NuklearEditor()
{
}

void NuklearEditor::EditorInit(NuklearUI* manager, std::vector<NKBase*>* obj, std::vector<NKBase*>* module, std::map<unsigned int, NKBase*>* moduleID, std::map<std::string, NKBase*>* moduleName, std::map<int, struct nk_image>* image, std::map<std::string, sprData*>* spr, std::vector<CustomData>* vvariable, std::vector<CustomData>* vfunction)
{
	m_selectedNode = nullptr;
	m_deletedNode = nullptr;
	m_pManager = manager;
	m_vecObject = obj;
	m_vecModule = module;
	m_mapModuleID = moduleID;
	m_mapModuleName = moduleName;
	m_mapImage = image;
	m_mapSpr = spr;
	m_vecVariable = vvariable;
	m_vecFunction = vfunction;
}

void NuklearEditor::EditorLayout(nk_context* ctx, struct nk_rect debugRect)
{
	if (nk_begin(ctx, "debug", debugRect, NK_WINDOW_TITLE | NK_WINDOW_MINIMIZABLE | NK_WINDOW_MOVABLE))
	{
		nk_layout_row_dynamic(ctx, 50.f, 2);
		if (nk_button_label(ctx, "New"))
		{
			NKWindow* pWin = new NKWindow(ctx, m_pManager);
			m_pManager->Add(pWin);
		}
		if (nk_button_label(ctx, "Refresh"))
		{
			for (std::vector<NKBase*>::iterator iter = m_vecObject->begin(); iter != m_vecObject->end();)
			{
				NKBase* pNKBase = *iter;

				if (pNKBase)
				{
					pNKBase->Release();
					delete pNKBase;
					pNKBase = NULL;
				}
				iter = m_vecObject->erase(iter);
			}
			m_vecObject->clear();
			m_vecModule->clear();
			m_mapModuleID->clear();
			m_mapModuleName->clear();
			m_selectedNode = nullptr;

			m_pManager->RunFunction("Init");
		}


		if (nk_contextual_begin(ctx, 0, nk_vec2(100, 220), nk_window_get_bounds(ctx))) {
			const char* grid_option[] = { "Show Grid", "Hide Grid" };
			nk_layout_row_dynamic(ctx, 25, 1);
			if (nk_contextual_item_label(ctx, "New", NK_TEXT_CENTERED))
			{
			}
			if (nk_contextual_item_label(ctx, grid_option[0], NK_TEXT_CENTERED))
			{

			}
			nk_contextual_end(ctx);
		}
		nk_layout_row_dynamic(ctx, 30, 4);
		if (nk_option_label(ctx, "node", m_option == eNODE)) m_option = eNODE;
		if (nk_option_label(ctx, "copy", m_option == eCOPY)) m_option = eCOPY;
		if (nk_option_label(ctx, "remove", m_option == eREMOVE)) m_option = eREMOVE;
		if (nk_option_label(ctx, "file", m_option == eFILE)) m_option = eFILE;
		if (nk_option_label(ctx, "function", m_option == eFUNCTION)) m_option = eFUNCTION;
		if (nk_option_label(ctx, "variable", m_option == eVARIABLE)) m_option = eVARIABLE;

		static int groupLeft = 150;

		switch (m_option)
		{
			case eFUNCTION: {
				CustomDataLayout(ctx, "Function", m_vecFunction);
			} break;
			case eVARIABLE: {
				CustomDataLayout(ctx, "Variable", m_vecVariable);
			} break;
			case eFILE: {
				FileLayout(ctx);
			} break;
			default: {
				nk_layout_row_dynamic(ctx, 22, 1);
				nk_property_int(ctx, "#Left:", 150, &groupLeft, (int)debugRect.w - 100, 1, 1.f);

				float row_layout[2] = { 0.f };
				row_layout[0] = (float)groupLeft;
				row_layout[1] = debugRect.w - 25.f - (float)groupLeft;

				nk_layout_row(ctx, NK_STATIC, debugRect.h - 175, 2, row_layout);
				NodeLayout(ctx, (int)row_layout[0]);
				InfoLayout(ctx, (int)row_layout[1]);
			} break;
		}


		if (m_show_popup) {
			if (nk_popup_begin(ctx, NK_POPUP_STATIC, "Error", NK_WINDOW_TITLE, nk_rect(debugRect.w / 2.f - 400.f / 2, debugRect.h / 2.f - 100.f / 2.f, 400.f, 200.f)))	{
				
				nk_layout_row_dynamic(ctx, 100, 1);
				nk_label_wrap(ctx, m_popup_content);

				nk_layout_row_dynamic(ctx, 20, 1);
				if (nk_button_label(ctx, "Close Popup")) {
					m_show_popup = 0;
					nk_popup_close(ctx);
				}
				nk_popup_end(ctx);
			}
		}
	}
	nk_end(ctx);
}

void NuklearEditor::NodeLayout(nk_context* ctx, int width)
{
	if (nk_group_begin(ctx, "Node", NK_WINDOW_TITLE)) {
		for (size_t i = 0; i < m_vecObject->size(); ++i)
		{
			NodesLayout(ctx, m_vecObject->at(i), NK_TREE_TAB, NK_MINIMIZED);
		}
		//for (i = 0; i < 16; ++i)
			//nk_selectable_label(m_ctx, (selected[i]) ? "Selected" : "Unselected", NK_TEXT_LEFT, &selected[i]);
		nk_group_end(ctx);
	}

	if (m_deletedNode) {
		m_pManager->Remove(m_deletedNode);
		m_deletedNode = nullptr;
	}
}

void NuklearEditor::NodesLayout(nk_context* ctx, NKBase* pBase, nk_tree_type nkType, nk_collapse_states nkState)
{
	if (nk_tree_push_id(ctx, nkType, pBase->GetBaseName(), nkState, reinterpret_cast<intptr_t>(pBase)))
	{
		switch (m_option)
		{
		case eNODE:
			if (nk_button_label(ctx, "Select")) {
				SelectNode(pBase);
			}
			break;
		case eREMOVE:
			if (nk_button_label(ctx, "Delete")) {
				DeleteNode(pBase);
			}
			break;
		case eCOPY:
			if (nk_button_label(ctx, "Copy")) {
				CopyNode(pBase);
			}
			break;
		default:
			break;
		}
		if (m_option == eNODE) {
		}
		else if (m_option == eREMOVE) {
		}

		if (pBase) {
			auto childList = pBase->GetChildList();
			for (auto child = childList->begin(); child != childList->end(); ++child)
			{
				if (*child)
				{
					NodesLayout(ctx, *child, NK_TREE_NODE, NK_MINIMIZED);
				}
			}
		}

		nk_tree_pop(ctx);
	}
}

void NuklearEditor::SelectNode(NKBase* pBase)
{
	m_selectedNode = pBase;
}

void NuklearEditor::CopyNode(NKBase* pBase)
{
	
}

void NuklearEditor::DeleteNode(NKBase* pBase)
{
	m_selectedNode = nullptr;
	m_deletedNode = pBase;
}

void NuklearEditor::InfoLayout(nk_context* ctx, int width)
{
	const char* ObjectInfo = m_selectedNode ? m_selectedNode->GetBaseName() : "ObjectInfo";
	if (nk_group_begin(ctx, ObjectInfo, NK_WINDOW_TITLE)) {

		if (m_selectedNode) {
			if (nk_tree_push(ctx, NK_TREE_TAB, "ViewportInfo", NK_MINIMIZED)) {

				nk_label(ctx, "Pivot", NK_TEXT_LEFT);
				nk_property_float(ctx, "#X:", .0f, &m_pManager->GetPivot()->x, 1.f, 0.01f, 0.01f);
				nk_property_float(ctx, "#Y:", .0f, &m_pManager->GetPivot()->y, 1.f, 0.01f, 0.01f);

				static char viewportStr[32] = { 0, };

				nk_label(ctx, "Viewport", NK_TEXT_LEFT);

				sprintf_s(viewportStr, "%.1f", m_pManager->GetViewport()->w);
				nk_layout_row_dynamic(ctx, 22, 2);
				nk_label(ctx, "Width:", NK_TEXT_LEFT);
				nk_label(ctx, viewportStr, NK_TEXT_RIGHT);


				sprintf_s(viewportStr, "%.1f", m_pManager->GetViewport()->h);
				nk_layout_row_dynamic(ctx, 22, 2);
				nk_label(ctx, "Height", NK_TEXT_LEFT);
				nk_label(ctx, viewportStr, NK_TEXT_RIGHT);
				nk_tree_pop(ctx);
			}
			m_selectedNode->LayoutEditor();
		}
		nk_group_end(ctx);
	}
}

void NuklearEditor::FileLayout(nk_context* ctx)
{
	nk_layout_row_dynamic(ctx, 22, 1);
	if (nk_button_label(ctx, "Open")) {
		m_pManager->OpenFileDialog();
	}


	float row_layout[2] = { 0.f };
	row_layout[0] = 0.7f;
	row_layout[1] = 0.3f;
	nk_layout_row(ctx, NK_DYNAMIC, 55, 2, row_layout);
	static char SearchFunction[256] = { 0, };
	static int SearchFunction_Len = 0;
	nk_flags searchResult = m_pManager->IMEInputSystem(ctx, NK_EDIT_FIELD | NK_EDIT_SIG_ENTER, SearchFunction, sizeof(SearchFunction), nk_filter_default, &SearchFunction_Len);

	if (nk_button_label(ctx, "Search") | searchResult & NK_EDIT_COMMITED) {

	}

	static char selectedFilename[260] = { 0, };
	float ratio[2] = { 0.8f, 0.2f };
	nk_layout_row_dynamic(ctx, 500, 1);
	if (nk_group_begin(ctx, "File List", NK_WINDOW_TITLE)) {
		nk_layout_row(ctx, NK_DYNAMIC, 22, 2, ratio);
		for (std::map<std::string, sprData*>::iterator it = m_mapSpr->begin(); it != m_mapSpr->end(); ++it) {
			std::filesystem::path filePath((*it).first.c_str());

			bool bSearch = false;
			bool bSearchResult = true;
			if (strlen(SearchFunction) > 0) {
				bSearch = true;
			}

			if (bSearch) {
				std::wstring word = NuklearUI::utf8ToWstring(filePath.filename().string().c_str());
				std::wstring filter = NuklearUI::utf8ToWstring(SearchFunction);

				// word를 소문자로 변환
				std::transform(word.begin(), word.end(), word.begin(), towlower);
				// filter를 소문자로 변환
				std::transform(filter.begin(), filter.end(), filter.begin(), towlower);

				bSearchResult = word.find(filter) != std::wstring::npos;
			}

			if (!bSearchResult) {
				continue;
			}



			nk_label(ctx, filePath.filename().string().c_str(), NK_TEXT_LEFT);

			if (nk_button_label(ctx, "Load")) {
				memset(selectedFilename, 0, sizeof(selectedFilename));
				strcpy_s(selectedFilename, (*it).first.c_str());
			}
		}
		nk_group_end(ctx);
	}
	nk_layout_row_dynamic(ctx, 300, 1);
	if (nk_group_begin(ctx, "Viewer", NK_WINDOW_TITLE)) {
		if (strlen(selectedFilename))
		{
			struct nk_image img;
			m_pManager->GetImage(selectedFilename, img);
			nk_layout_row_dynamic(ctx, 300, 1);
			nk_image(ctx, img);			
		}
		nk_group_end(ctx);
	}
}

void NuklearEditor::CustomDataLayout(nk_context* ctx, const char* dataName, std::vector<CustomData>* vCustom)
{
	float row_layout[2] = { 0.f };
	row_layout[0] = 0.7f;
	row_layout[1] = 0.3f;
	nk_layout_row(ctx, NK_DYNAMIC, 55, 2, row_layout);
	static char SearchFunction[256] = { 0, };
	static int SearchFunction_Len = 0;
	nk_flags searchResult = m_pManager->IMEInputSystem(ctx, NK_EDIT_FIELD | NK_EDIT_SIG_ENTER, SearchFunction, sizeof(SearchFunction), nk_filter_default, &SearchFunction_Len);

	if (nk_button_label(ctx, "Search") | searchResult & NK_EDIT_COMMITED) {

	}

	nk_layout_row_dynamic(ctx, 800, 1);
	if (nk_group_begin(ctx, dataName, NK_WINDOW_TITLE)) {

		nk_layout_row_dynamic(ctx, 22, 1);
		if (nk_button_label(ctx, "Add")) {
			CustomData cfunc;
			sprintf_s(cfunc.name, "%s%d", dataName, vCustom->size());
			sprintf_s(cfunc.tableName, "table%d", vCustom->size());
			vCustom->push_back(cfunc);

			std::sort(vCustom->begin(), vCustom->end(), NuklearUI::customCompare);
		}

		for (auto it = vCustom->begin(); it != vCustom->end(); ++it) {
			CustomData& d = *it;
			bool bSearch = false;
			bool bSearchResult = true;
			if (strlen(SearchFunction) > 0) {
				bSearch = true;
			}

			if (bSearch) {
				std::wstring word = NuklearUI::utf8ToWstring(d.name);
				std::wstring filter = NuklearUI::utf8ToWstring(SearchFunction);

				// word를 소문자로 변환
				std::transform(word.begin(), word.end(), word.begin(), towlower);
				// filter를 소문자로 변환
				std::transform(filter.begin(), filter.end(), filter.begin(), towlower);

				bSearchResult = word.find(filter) != std::wstring::npos;
			}

			if (!bSearchResult) {
				continue;
			}

			if (nk_tree_push_id(ctx, NK_TREE_TAB, d.name, NK_MINIMIZED, reinterpret_cast<intptr_t>(&d))) {
				float tree_layout[2] = { 0.f, };
				tree_layout[0] = 0.3f;
				tree_layout[1] = 0.7f;
				nk_layout_row(ctx, NK_DYNAMIC, 55, 2, tree_layout);
				nk_label(ctx, "name: ", NK_TEXT_LEFT);
				m_pManager->IMEInputSystem(ctx, NK_EDIT_FIELD | NK_EDIT_SIG_ENTER, d.name, sizeof(d.name), nk_filter_default, &d.nameLen);

				nk_label(ctx, "table: ", NK_TEXT_LEFT);
				m_pManager->IMEInputSystem(ctx, NK_EDIT_FIELD | NK_EDIT_SIG_ENTER, d.tableName, sizeof(d.tableName), nk_filter_default, &d.tableLen);

				if (nk_button_label(ctx, "remove")) {
					it = vCustom->erase(it);

					if (it >= vCustom->end()) {

						nk_tree_pop(ctx);
						break;
					}
				}

				nk_tree_pop(ctx);
			}
		}
		nk_group_end(ctx);
	}
}

void NuklearEditor::OpenErrorPopup(const char* content)
{
	memset(m_popup_content, 0, sizeof(m_popup_content));
	strcpy_s(m_popup_content, content);
	m_show_popup = 1;
}
