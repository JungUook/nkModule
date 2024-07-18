#include "pch.h"
#include "NuklearUI.h"
#include "UiLibrary.h"
#include "NuklearEditor.h"

#include <iostream>
#include <vector>
#include <algorithm>
#include <locale>
#include <string>
#include <windows.h>

#ifdef _NKDEBUG
NuklearEditor* g_Editor;

NuklearEditor::NuklearEditor()
{
	m_pManager = nullptr;

	m_vecObject = nullptr;
	m_vecModule = nullptr;
	m_mapModuleID = nullptr;
	m_mapModuleName = nullptr;
	m_mapWindowName = nullptr;
	m_mapImage = nullptr;
	m_mapSpr = nullptr;

	m_iOption = 0;
	m_iNodeOption = 0;
	m_iLuaOption = 0;
	m_pSelectedNode = nullptr;
	m_pDeletedNode = nullptr;
	m_pMoveNode = nullptr;
	m_mapVariable = nullptr;
	m_mapFunction = nullptr;
	m_vecPrefab = nullptr;
	m_vecLuaCode = nullptr;
	m_bShow_popup = 0;
	m_bMovingNode = false;

	m_ctx = nullptr;

	pDD = nullptr;
	pDDSPrimary = nullptr;
	pDDSBackBuffer = nullptr;
	pClipper = nullptr;
	pD3D = nullptr;
	pD3DDevice = nullptr;
	memset(m_cPopup_content, 0, sizeof(m_cPopup_content));
	g_Editor = this;
}

NuklearEditor::~NuklearEditor()
{
	g_Editor = nullptr;
}

void NuklearEditor::EditorInit(NuklearUI* manager, std::vector<NKBase*>* obj, std::vector<NKBase*>* module, std::map<unsigned int, NKBase*>* moduleID, std::map<std::string, NKBase*>* moduleName, std::map<std::string, NKBase*>* windowName, std::map<int, struct nk_image>* image, std::map<std::string, sprData*>* spr, std::map<std::string, CustomData>* mvariable, std::map<std::string, CustomData>* mfunction, std::vector<std::string>* vPrefab, std::vector<std::string>* vLua)
{
	m_pSelectedNode = nullptr;
	m_pDeletedNode = nullptr;
	m_pManager = manager;
	m_vecObject = obj;
	m_vecModule = module;
	m_mapModuleID = moduleID;
	m_mapModuleName = moduleName;
	m_mapWindowName = windowName;
	m_mapImage = image;
	m_mapSpr = spr;
	m_mapVariable = mvariable;
	m_mapFunction = mfunction;
	m_vecPrefab = vPrefab;
	m_vecLuaCode = vLua;
}

void NuklearEditor::EditorLayout(struct nk_rect debugRect)
{
	nk_context* ctx = m_ctx;
	if (nk_begin(ctx, "debug", debugRect, 0))
	{
		if (m_bMovingNode) {
			nk_tooltip_begin(ctx, 300.f);
			nk_layout_row_dynamic(ctx, 30.f, 2);
			nk_label(ctx, "MoveNode:", NK_TEXT_LEFT);
			nk_label(ctx, m_pMoveNode->GetPrimaryName(), NK_TEXT_RIGHT);
			nk_tooltip_end(ctx);
		}

		if (nk_tree_push(ctx, NK_TREE_TAB, "System", NK_MINIMIZED)) {
			nk_layout_row_dynamic(ctx, 50.f, 4);
			//if (nk_button_label(ctx, "New"))
			//{
			//	NKWindow* pWin = new NKWindow(ctx, m_pManager);
			//	m_pManager->Add(pWin);
			//}

			static bool bRealClear = false;

			if(bRealClear) {
				if (nk_popup_begin(ctx, NK_POPUP_STATIC, "Clear", NK_WINDOW_TITLE, nk_rect(debugRect.w / 2.f - 400.f / 2, debugRect.h / 2.f - 100.f / 2.f, 400.f, 200.f))) {

					nk_layout_row_dynamic(ctx, 100, 1);
					nk_label_wrap(ctx, "Are you sure you want to clear the project? Confirming will delete all the information in the current project.");

					nk_layout_row_dynamic(ctx, 20, 2);
					if (nk_button_label(ctx, "Yes")) {
						Clear();
						bRealClear = false;
						nk_popup_close(ctx);
					}
					if (nk_button_label(ctx, "No")) {
						bRealClear = false;
						nk_popup_close(ctx);
					}
					nk_popup_end(ctx);
				}
			}

			if (nk_button_label(ctx, "Clear"))
			{
				bRealClear = true;
			}
			if (nk_button_label(ctx, "Reload"))
			{
				for (auto it = m_vecLuaCode->begin(); it != m_vecLuaCode->end(); ++it) {
					std::string str = m_pManager->m_cereal.GetExecutablePath() + "\\" + *it;
					m_pManager->m_luaInterface.LoadLuaFile(str.c_str());
				}
			}
			if (nk_menu_begin_label(ctx, "Save", NK_TEXT_CENTERED, nk_vec2(100,100))) {

				nk_layout_row_dynamic(ctx, 25, 1);
				if (nk_menu_item_label(ctx, "json", NK_TEXT_LEFT))
				{
					char path[MAX_PATH];
					HMODULE hModule = GetModuleHandle(NULL);
					if (hModule != NULL) {
						// 현재 실행 파일의 경로를 얻습니다.
						GetModuleFileNameA(hModule, path, MAX_PATH);
					}
					else {
						std::cerr << "Failed to get module handle." << std::endl;
						return;
					}
					std::string basePath(path);
					basePath = basePath.substr(0, basePath.find_last_of("\\\\"));
					std::string dataPath = basePath + "\\NInterface\\Data2";
					if (!m_pManager->CreateDirectoryIfNotExists(dataPath)) {
						std::cerr << "Failed to create directory: " << dataPath << std::endl;
						return;
					}
					std::string filePath = dataPath + "\\nkmod.json";

					m_pManager->m_cereal.SaveFile(filePath);
				}
				if (nk_menu_item_label(ctx, "binary", NK_TEXT_LEFT))
				{
					char path[MAX_PATH];
					HMODULE hModule = GetModuleHandle(NULL);
					if (hModule != NULL) {
						// 현재 실행 파일의 경로를 얻습니다.
						GetModuleFileNameA(hModule, path, MAX_PATH);
					}
					else {
						std::cerr << "Failed to get module handle." << std::endl;
						return;
					}
					std::string basePath(path);
					basePath = basePath.substr(0, basePath.find_last_of("\\\\"));
					std::string dataPath = basePath + "\\NInterface\\Data2";
					if (!m_pManager->CreateDirectoryIfNotExists(dataPath)) {
						std::cerr << "Failed to create directory: " << dataPath << std::endl;
						return;
					}
					std::string filePath = dataPath + "\\nkmod.bin";

					m_pManager->m_cereal.SaveFileBinary(filePath);
				}
				nk_menu_end(ctx);
			}
			if (nk_menu_begin_label(ctx, "Load", NK_TEXT_CENTERED, nk_vec2(100, 100))) {

				nk_layout_row_dynamic(ctx, 25, 1);
				if (nk_menu_item_label(ctx, "json", NK_TEXT_LEFT))
				{
					Clear();
					char path[MAX_PATH];
					HMODULE hModule = GetModuleHandle(NULL);
					if (hModule != NULL) {
						// 현재 실행 파일의 경로를 얻습니다.
						GetModuleFileNameA(hModule, path, MAX_PATH);
					}
					else {
						std::cerr << "Failed to get module handle." << std::endl;
						return;
					}
					std::string basePath(path);
					basePath = basePath.substr(0, basePath.find_last_of("\\\\"));
					std::string dataPath = basePath + "\\NInterface\\Data2";
					if (!m_pManager->CreateDirectoryIfNotExists(dataPath)) {
						std::cerr << "Failed to create directory: " << dataPath << std::endl;
						return;
					}
					std::string filePath = dataPath + "\\nkmod.json";

					m_pManager->m_cereal.LoadFile(*m_mapVariable, *m_mapFunction, filePath);
				}
				if (nk_menu_item_label(ctx, "binary", NK_TEXT_LEFT))
				{
					Clear();
					char path[MAX_PATH];
					HMODULE hModule = GetModuleHandle(NULL);
					if (hModule != NULL) {
						// 현재 실행 파일의 경로를 얻습니다.
						GetModuleFileNameA(hModule, path, MAX_PATH);
					}
					else {
						std::cerr << "Failed to get module handle." << std::endl;
						return;
					}
					std::string basePath(path);
					basePath = basePath.substr(0, basePath.find_last_of("\\\\"));
					std::string dataPath = basePath + "\\NInterface\\Data2";
					if (!m_pManager->CreateDirectoryIfNotExists(dataPath)) {
						std::cerr << "Failed to create directory: " << dataPath << std::endl;
						return;
					}
					std::string filePath = dataPath + "\\nkmod.bin";

					m_pManager->m_cereal.LoadFileBinary(*m_mapVariable, *m_mapFunction, filePath);
				}
				nk_menu_end(ctx);
			}

			nk_layout_row_dynamic(ctx, 30, 4);
			if (nk_option_label(ctx, "node", m_iOption == eNODE)) m_iOption = eNODE;
			if (nk_option_label(ctx, "file", m_iOption == eFILE)) m_iOption = eFILE;
			if (nk_option_label(ctx, "lua", m_iOption == eLUA)) m_iOption = eLUA;
			if (nk_option_label(ctx, "prefab", m_iOption == ePREFAB)) m_iOption = ePREFAB;
			
			nk_tree_pop(ctx);
		}


		static int groupLeft = 150;

		switch (m_iOption)
		{
			case eLUA: {
				nk_layout_row_dynamic(ctx, 30, 4);
				nk_label(ctx, "Lua Data:", NK_TEXT_LEFT);
				if (nk_option_label(ctx, "code", m_iLuaOption == eCODE)) m_iLuaOption = eCODE;
				if (nk_option_label(ctx, "function", m_iLuaOption == eFUNCTION)) m_iLuaOption = eFUNCTION;
				if (nk_option_label(ctx, "variable", m_iLuaOption == eVARIABLE)) m_iLuaOption = eVARIABLE;
				LuaDataLayout(ctx);
			} break;
			case eFILE: {
				FileLayout(ctx);
			} break;
			case ePREFAB: {
				PrefabLayout(ctx);
			} break;
			default: {
				if (nk_tree_push(ctx, NK_TREE_TAB, "Current Node", NK_MINIMIZED)) {
					nk_layout_row_dynamic(ctx, 30, 4);
					if (nk_option_label(ctx, "select", m_iNodeOption == eSELECT)) m_iNodeOption = eSELECT;
					if (nk_option_label(ctx, "copy", m_iNodeOption == eCOPY)) m_iNodeOption = eCOPY;
					if (nk_option_label(ctx, "remove", m_iNodeOption == eREMOVE)) m_iNodeOption = eREMOVE;
					if (nk_option_label(ctx, "move", m_iNodeOption == eMOVE)) m_iNodeOption = eMOVE;

					nk_layout_row_dynamic(ctx, 22, 1);
					nk_property_int(ctx, "#Left:", 150, &groupLeft, (int)debugRect.w - 100, 1, 1.f);
					nk_tree_pop(ctx);
				}
				float row_layout[2] = { 0.f };
				row_layout[0] = (float)groupLeft;
				row_layout[1] = debugRect.w - 25.f - (float)groupLeft;

				nk_layout_row(ctx, NK_STATIC, debugRect.h - 175, 2, row_layout);
				NodeLayout(ctx, (int)row_layout[0]);
				InfoLayout(ctx, (int)row_layout[1]);
			} break;
		}


		if (m_bShow_popup) {
			if (nk_popup_begin(ctx, NK_POPUP_STATIC, "Error", NK_WINDOW_TITLE, nk_rect(debugRect.w / 2.f - 400.f / 2, debugRect.h / 2.f - 100.f / 2.f, 400.f, 200.f)))	{
				
				nk_layout_row_dynamic(ctx, 100, 1);
				nk_label_wrap(ctx, m_cPopup_content);

				nk_layout_row_dynamic(ctx, 20, 1);
				if (nk_button_label(ctx, "Close Popup")) {
					m_bShow_popup = 0;
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
	if (nk_contextual_begin(ctx, 0, nk_vec2(180, 220), nk_window_get_bounds(ctx))) {
		const char* grid_option[] = { "New Window", "Save Prefab"};
		nk_layout_row_dynamic(ctx, 25, 1);
		if (nk_contextual_item_label(ctx, grid_option[0], NK_TEXT_LEFT))
		{
			NKWindow* pWin = new NKWindow(ctx, m_pManager);
			m_pManager->Add(pWin);
		}
		if (m_pSelectedNode != nullptr && nk_contextual_item_label(ctx, grid_option[1], NK_TEXT_LEFT))
		{
			char path[MAX_PATH];
			HMODULE hModule = GetModuleHandle(NULL);
			if (hModule != NULL) {
				// 현재 실행 파일의 경로를 얻습니다.
				GetModuleFileNameA(hModule, path, MAX_PATH);
			}
			else {
				std::cerr << "Failed to get module handle." << std::endl;
				return;
			}
			std::string basePath(path);
			basePath = basePath.substr(0, basePath.find_last_of("\\\\"));
			std::string dataPath = basePath + "\\NInterface\\Data2\\dp";
			if (!m_pManager->CreateDirectoryIfNotExists(dataPath)) {
				std::cerr << "Failed to create directory: " << dataPath << std::endl;
				return;
			}
			std::string filePath = dataPath + "\\" + m_pSelectedNode->GetPrimaryName();

			m_pManager->m_cereal.SavePrefab(filePath, m_pSelectedNode);
		}
		nk_contextual_end(ctx);
	}

	if (nk_group_begin(ctx, "Node", NK_WINDOW_TITLE)) {
		for (size_t i = 0; i < m_vecObject->size(); ++i)
		{
			NodesLayout(ctx, m_vecObject->at(i), NK_TREE_TAB, NK_MINIMIZED);
		}
		//for (i = 0; i < 16; ++i)
			//nk_selectable_label(m_ctx, (selected[i]) ? "Selected" : "Unselected", NK_TEXT_LEFT, &selected[i]);
		nk_group_end(ctx);
	}

	if (m_pDeletedNode) {
		m_pManager->Remove(m_pDeletedNode);
		m_pDeletedNode = nullptr;
	}
}

void NuklearEditor::NodesLayout(nk_context* ctx, NKBase* pBase, nk_tree_type nkType, nk_collapse_states nkState)
{
	if (nk_tree_push_id(ctx, nkType, pBase->GetBaseName(), nkState, reinterpret_cast<intptr_t>(pBase)))
	{
		nk_bool bHover = nk_widget_is_hovered(ctx);
		if(bHover)
			nk_tooltip(ctx, pBase->GetBaseName());

		switch (m_iNodeOption)
		{
		case eSELECT:
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
		case eMOVE:

			if (m_bMovingNode) {
				if (nk_button_label(ctx, "Target")) {
					MoveNode(pBase);
				}
			}
			else {
				if (nk_button_label(ctx, "Move")) {
					MoveRegistNode(pBase);
				}
			}
			break;
		default:
			break;
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
	m_pSelectedNode = pBase;
	m_pDeletedNode = nullptr;
	m_pMoveNode = nullptr;
}

void NuklearEditor::CopyNode(NKBase* pBase)
{
	eTypeUI type = pBase->GetType();
	NKBase* parent = nullptr;
	if (type != eWINDOW) {
		parent = pBase->GetParent();
	}

	m_pManager->CopyUI(pBase, parent);
}

void NuklearEditor::DeleteNode(NKBase* pBase)
{
	m_pSelectedNode = nullptr;
	m_pDeletedNode = pBase;
	m_pMoveNode = nullptr;
}

void NuklearEditor::MoveRegistNode(NKBase* pBase)
{
	m_pSelectedNode = nullptr;
	m_pDeletedNode = nullptr;
	m_pMoveNode = pBase;
	m_bMovingNode = true;
}

void NuklearEditor::MoveNode(NKBase* pTarget)
{
	if (m_pMoveNode == pTarget) {
		m_bMovingNode = false;
		m_pMoveNode = nullptr;
		return;
	}

	m_pSelectedNode = nullptr;
	m_pDeletedNode = nullptr;
	m_bMovingNode = false;

	m_pManager->Move(m_pMoveNode->GetPrimaryID(), pTarget->GetPrimaryID());
	m_pMoveNode = nullptr;
}

void NuklearEditor::InfoLayout(nk_context* ctx, int width)
{
	const char* ObjectInfo = m_pSelectedNode ? m_pSelectedNode->GetBaseName() : "ObjectInfo";
	if (nk_group_begin(ctx, ObjectInfo, NK_WINDOW_TITLE)) {

		if (m_pSelectedNode) {
			m_pSelectedNode->ActiveEditor(ctx);
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
			m_pSelectedNode->LayoutEditor(ctx);
			if (nk_tree_push(ctx, NK_TREE_TAB, "Prefab", NK_MINIMIZED)) {

				PrefabLayout(ctx);

				nk_tree_pop(ctx);
			}
		}
		nk_group_end(ctx);
	}
}

void NuklearEditor::FileLayout(nk_context* ctx)
{
	nk_layout_row_dynamic(ctx, 44, 1);
	if (nk_button_label(ctx, "Open")) {
		m_pManager->OpenFileDialog();
	}


	float row_layout[2] = { 0.f };
	row_layout[0] = 0.7f;
	row_layout[1] = 0.3f;
	nk_layout_row(ctx, NK_DYNAMIC, 55, 2, row_layout);
	static char SearchFunction[256] = { 0, };
	static int SearchFunction_Len = 0;
	nk_flags searchResult = m_pManager->IMEInputSystem(ctx, SearchFunction, sizeof(SearchFunction), &SearchFunction_Len);

	if (nk_button_label(ctx, "Search") | searchResult & NK_EDIT_COMMITED) {

	}

	static char selectedFilename[260] = { 0, };
	float ratio[3] = { 0.6f, 0.2f, 0.2f };

	if (m_mapSpr->size() == 0) {
		memset(selectedFilename, 0, sizeof(selectedFilename));
	}

	nk_layout_row_dynamic(ctx, 500, 1);
	if (nk_group_begin(ctx, "File List", NK_WINDOW_TITLE)) {
		nk_layout_row(ctx, NK_DYNAMIC, 22, 3, ratio);
		for (std::map<std::string, sprData*>::iterator it = m_mapSpr->begin(); it != m_mapSpr->end();) {
			std::filesystem::path filePath((*it).first.c_str());

			bool bSearch = false;
			bool bSearchResult = true;
			if (strlen(SearchFunction) > 0) {
				bSearch = true;
			}

			if (bSearch) {
				std::wstring word = NKLuaInterface::utf8ToWstring(filePath.filename().string().c_str());
				std::wstring filter = NKLuaInterface::utf8ToWstring(SearchFunction);

				// word를 소문자로 변환
				std::transform(word.begin(), word.end(), word.begin(), towlower);
				// filter를 소문자로 변환
				std::transform(filter.begin(), filter.end(), filter.begin(), towlower);

				bSearchResult = word.find(filter) != std::wstring::npos;
			}

			if (!bSearchResult) {
				continue;
			}

			float text_width = ctx->style.font->width(ctx->style.font->userdata, ctx->style.font->height, (*it).first.c_str(), (*it).first.length());
			float text_height = ctx->style.font->height;

			nk_label(ctx, filePath.filename().string().c_str(), NK_TEXT_LEFT);
			struct nk_rect label_bounds = nk_layout_widget_bounds(ctx);
			if (nk_input_is_mouse_hovering_rect(&ctx->input, label_bounds)) {

				const struct nk_style* style;
				struct nk_vec2 padding;

				float text_width;
				float text_height;

				style = &ctx->style;
				padding = style->window.padding;

				text_width = style->font->width(style->font->userdata,
					style->font->height, (*it).first.c_str(), (*it).first.length());
				text_width += (4 * padding.x);
				text_height = (style->font->height + 2 * padding.y);

				if (nk_tooltip_begin(ctx, (float)text_width)) {
					nk_layout_row_dynamic(ctx, (float)text_height, 1);
					nk_text(ctx, (*it).first.c_str(), (*it).first.length(), NK_TEXT_LEFT);
					nk_tooltip_end(ctx);
				}
			}

			if (nk_button_label(ctx, "Load")) {
				memset(selectedFilename, 0, sizeof(selectedFilename));
				strcpy_s(selectedFilename, (*it).first.c_str());
			}

			if (nk_button_label(ctx, "Delete")) {
				memset(selectedFilename, 0, sizeof(selectedFilename));
				it = m_mapSpr->erase(it);
			}
			else {
				++it;
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

void NuklearEditor::LuaDataLayout(nk_context* ctx)
{
	if (m_iLuaOption == eCODE) {
		LuaCodeLayout(ctx);
	}
	else if (m_iLuaOption == eFUNCTION) {
		CustomDataLayout(ctx, "Function", m_mapFunction);
	}
	else if (m_iLuaOption = eVARIABLE) {
		CustomDataLayout(ctx, "Variable", m_mapVariable);
	}
}

void NuklearEditor::LuaCodeLayout(nk_context* ctx)
{
	nk_layout_row_dynamic(ctx, 44, 2);
	if (nk_button_label(ctx, "Open")) {
		m_pManager->m_cereal.OpenLuaCodeDialog();
	}
	if (nk_button_label(ctx, "Reload")) {
		
		for (auto it = m_vecLuaCode->begin(); it != m_vecLuaCode->end(); ++it) {
			std::string str = m_pManager->m_cereal.GetExecutablePath() + "\\" + *it;
			m_pManager->m_luaInterface.LoadLuaFile(str.c_str());
		}
	}

	float row_layout[2] = { 0.f };
	row_layout[0] = 0.7f;
	row_layout[1] = 0.3f;
	nk_layout_row(ctx, NK_DYNAMIC, 55, 2, row_layout);
	static char SearchFunction[256] = { 0, };
	static int SearchFunction_Len = 0;
	nk_flags searchResult = m_pManager->IMEInputSystem(ctx, SearchFunction, sizeof(SearchFunction), &SearchFunction_Len);

	if (nk_button_label(ctx, "Search") | searchResult & NK_EDIT_COMMITED) {

	}

	static char selectedFilename[260] = { 0, };
	float ratio[3] = { 0.6f, 0.2f, 0.2f };

	if (m_vecLuaCode->size() == 0) {
		memset(selectedFilename, 0, sizeof(selectedFilename));
	}

	nk_layout_row_dynamic(ctx, 500, 1);
	if (nk_group_begin(ctx, "Lua Code List", NK_WINDOW_TITLE)) {
		nk_layout_row(ctx, NK_DYNAMIC, 22, 3, ratio);
		for (std::vector<std::string>::iterator it = m_vecLuaCode->begin(); it != m_vecLuaCode->end();) {
			std::filesystem::path filePath((*it).c_str());

			bool bSearch = false;
			bool bSearchResult = true;
			if (strlen(SearchFunction) > 0) {
				bSearch = true;
			}

			if (bSearch) {
				std::wstring word = NKLuaInterface::utf8ToWstring(filePath.filename().string().c_str());
				std::wstring filter = NKLuaInterface::utf8ToWstring(SearchFunction);

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

			float text_width = ctx->style.font->width(ctx->style.font->userdata, ctx->style.font->height, (*it).c_str(), (*it).length());
			float text_height = ctx->style.font->height;
			struct nk_rect label_bounds = nk_layout_widget_bounds(ctx);
			if (nk_input_is_mouse_hovering_rect(&ctx->input, label_bounds)) {

				const struct nk_style* style;
				struct nk_vec2 padding;

				float text_width;
				float text_height;

				style = &ctx->style;
				padding = style->window.padding;

				text_width = style->font->width(style->font->userdata,
					style->font->height, (*it).c_str(), (*it).length());
				text_width += (4 * padding.x);
				text_height = (style->font->height + 2 * padding.y);

				if (nk_tooltip_begin(ctx, (float)text_width)) {
					nk_layout_row_dynamic(ctx, (float)text_height, 1);
					nk_text(ctx, (*it).c_str(), (*it).length(), NK_TEXT_LEFT);
					nk_tooltip_end(ctx);
				}
			}

			if (nk_button_label(ctx, "Reload")) {
				memset(selectedFilename, 0, sizeof(selectedFilename));
				strcpy_s(selectedFilename, (*it).c_str());

				std::string str = m_pManager->m_cereal.GetExecutablePath() + "\\" + *it;
				m_pManager->m_luaInterface.LoadLuaFile(str.c_str());
			}
			if (nk_button_label(ctx, "Delete")) {
				it = m_vecLuaCode->erase(it);
			}
			else {
				++it;
			}
		}
		nk_group_end(ctx);
	}
}

void NuklearEditor::CustomDataLayout(nk_context* ctx, const char* dataName, std::map<std::string, CustomData>* mCustom)
{
	float row_layout[2] = { 0.f };
	row_layout[0] = 0.7f;
	row_layout[1] = 0.3f;
	nk_layout_row(ctx, NK_DYNAMIC, 55, 2, row_layout);
	static char SearchFunction[256] = { 0, };
	static int SearchFunction_Len = 0;
	nk_flags searchResult = m_pManager->IMEInputSystem(ctx, SearchFunction, sizeof(SearchFunction), &SearchFunction_Len);

	if (nk_button_label(ctx, "Search") | searchResult & NK_EDIT_COMMITED) {

	}

	nk_layout_row_dynamic(ctx, 800, 1);
	if (nk_group_begin(ctx, dataName, NK_WINDOW_TITLE)) {

		nk_layout_row_dynamic(ctx, 22, 1);

		for (auto it = mCustom->begin(); it != mCustom->end(); ++it) {
			CustomData& d = it->second;
			bool bSearch = false;
			bool bSearchResult = true;
			if (strlen(SearchFunction) > 0) {
				bSearch = true;
			}

			if (bSearch) {
				std::wstring word = NKLuaInterface::utf8ToWstring(d.name);
				std::wstring filter = NKLuaInterface::utf8ToWstring(SearchFunction);

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
				//nk_layout_row_dynamic(ctx, 33, 1);
				//nk_label(ctx, "Desc: ", NK_TEXT_LEFT);
				//nk_layout_row_dynamic(ctx, 100, 1);
				//m_pManager->IMEInputSystem(ctx, d.desc, sizeof(d.desc), &d.descLen, NK_EDIT_BOX );

				//nk_label(ctx, "table: ", NK_TEXT_LEFT);
				//m_pManager->IMEInputSystem(ctx, d.tableName, sizeof(d.tableName), &d.tableLen);

				nk_layout_row_dynamic(ctx, 33, 2);
				nk_label(ctx, "Linked Function: ", NK_TEXT_LEFT);
				if (d.bFunction) {
					if (m_pManager->m_luaInterface.IsActiveFunction(d.name)) {
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
				else {
					if (m_pManager->m_luaInterface.IsActiveVariable(d.name)) {
						nk_color origin = ctx->style.text.color;
						ctx->style.text.color = nk_color(0, 255, 0, 255);
						nk_label(ctx, "Connection successful", NK_TEXT_RIGHT);
						ctx->style.text.color = origin;

						luabridge::LuaRef var = m_pManager->m_luaInterface.GetLuaTable(d.name);
						PrintTable(ctx, var);
					}
					else {
						nk_color origin = ctx->style.text.color;
						ctx->style.text.color = nk_color(0, 0, 255, 255);
						nk_label(ctx, "Connection failed", NK_TEXT_RIGHT);
						ctx->style.text.color = origin;
					}
				}

				nk_tree_pop(ctx);
			}
		}
		nk_group_end(ctx);
	}
}

void NuklearEditor::PrintTable(nk_context* ctx, luabridge::LuaRef ref)
{
	nk_layout_row_dynamic(ctx, 33, 1);
	nk_label(ctx, "Lua table contents", NK_TEXT_LEFT);
	if (ref.isTable()) {
		for (luabridge::Iterator iter(ref); !iter.isNil(); ++iter) {
			luabridge::LuaRef key = iter.key();
			luabridge::LuaRef value = iter.value();

			std::string contentValue = "";
			if (value.isNumber()) {
				contentValue = std::format("{:.2f}", value.cast<double>());
			}
			else if (value.isString()) {
				contentValue = value.cast<std::string>();
			}
			else if (value.isBool()) {
				contentValue = value.cast<bool>() ? "true" : "false";
			}
			else {
				contentValue = "Unknown type";
			}

			nk_layout_row_dynamic(ctx, 33, 2);
			nk_label(ctx, "Key: ", NK_TEXT_LEFT);
			nk_label(ctx, key.tostring().c_str(), NK_TEXT_RIGHT);
			nk_label(ctx, "Value: ", NK_TEXT_LEFT);
			nk_label(ctx, contentValue.c_str(), NK_TEXT_RIGHT);
		}
	}
	else if (ref.isNumber()) {
		std::string contentValue = std::format("{:.2f}", ref.cast<double>());
		nk_layout_row_dynamic(ctx, 33, 2);
		nk_label(ctx, "Value: ", NK_TEXT_LEFT);
		nk_label(ctx, contentValue.c_str(), NK_TEXT_RIGHT);
	}
	else if (ref.isString()) {
		std::string contentValue = ref.cast<std::string>();
		nk_layout_row_dynamic(ctx, 33, 2);
		nk_label(ctx, "Value: ", NK_TEXT_LEFT);
		nk_label(ctx, contentValue.c_str(), NK_TEXT_RIGHT);
	}
	else if (ref.isBool()) {
		std::string contentValue = ref.cast<bool>() ? "true" : "false";
		nk_layout_row_dynamic(ctx, 33, 2);
		nk_label(ctx, "Value: ", NK_TEXT_LEFT);
		nk_label(ctx, contentValue.c_str(), NK_TEXT_RIGHT);
	}
	else {
		std::string contentValue = "Unknown type";
		nk_layout_row_dynamic(ctx, 33, 2);
		nk_label(ctx, "Value: ", NK_TEXT_LEFT);
		nk_label(ctx, contentValue.c_str(), NK_TEXT_RIGHT);
	}
}

void NuklearEditor::PrefabLayout(nk_context* ctx)
{
	nk_layout_row_dynamic(ctx, 44, 1);
	if (nk_button_label(ctx, "Open")) {
		m_pManager->m_cereal.OpenPrefabDialog();
	}

	float row_layout[2] = { 0.f };
	row_layout[0] = 0.7f;
	row_layout[1] = 0.3f;
	nk_layout_row(ctx, NK_DYNAMIC, 55, 2, row_layout);
	static char SearchFunction[256] = { 0, };
	static int SearchFunction_Len = 0;
	nk_flags searchResult = m_pManager->IMEInputSystem(ctx, SearchFunction, sizeof(SearchFunction), &SearchFunction_Len);

	if (nk_button_label(ctx, "Search") | searchResult & NK_EDIT_COMMITED) {

	}

	static char selectedFilename[260] = { 0, };
	float ratio[3] = { 0.6f, 0.2f, 0.2f };

	if (m_vecPrefab->size() == 0) {
		memset(selectedFilename, 0, sizeof(selectedFilename));
	}

	nk_layout_row_dynamic(ctx, 500, 1);
	if (nk_group_begin(ctx, "Prefab List", NK_WINDOW_TITLE)) {
		nk_layout_row(ctx, NK_DYNAMIC, 22, 3, ratio);
		for (std::vector<std::string>::iterator it = m_vecPrefab->begin(); it != m_vecPrefab->end();) {
			std::filesystem::path filePath((*it).c_str());

			bool bSearch = false;
			bool bSearchResult = true;
			if (strlen(SearchFunction) > 0) {
				bSearch = true;
			}

			if (bSearch) {
				std::wstring word = NKLuaInterface::utf8ToWstring(filePath.filename().string().c_str());
				std::wstring filter = NKLuaInterface::utf8ToWstring(SearchFunction);

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

			float text_width = ctx->style.font->width(ctx->style.font->userdata, ctx->style.font->height, (*it).c_str(), (*it).length());
			float text_height = ctx->style.font->height;
			struct nk_rect label_bounds = nk_layout_widget_bounds(ctx);
			if (nk_input_is_mouse_hovering_rect(&ctx->input, label_bounds)) {

				const struct nk_style* style;
				struct nk_vec2 padding;

				float text_width;
				float text_height;

				style = &ctx->style;
				padding = style->window.padding;

				text_width = style->font->width(style->font->userdata,
					style->font->height, (*it).c_str(), (*it).length());
				text_width += (4 * padding.x);
				text_height = (style->font->height + 2 * padding.y);

				if (nk_tooltip_begin(ctx, (float)text_width)) {
					nk_layout_row_dynamic(ctx, (float)text_height, 1);
					nk_text(ctx, (*it).c_str(), (*it).length(), NK_TEXT_LEFT);
					nk_tooltip_end(ctx);
				}
			}

			if (nk_button_label(ctx, "Make")) {
				memset(selectedFilename, 0, sizeof(selectedFilename));
				strcpy_s(selectedFilename, (*it).c_str());

				m_pManager->m_cereal.LoadPrefab(selectedFilename, m_pSelectedNode);
			}
			if (nk_button_label(ctx, "Delete")) {
				it = m_vecPrefab->erase(it);
			}
			else {
				++it;
			}
		}
		nk_group_end(ctx);
	}
}

void NuklearEditor::OpenErrorPopup(const char* content)
{
	memset(m_cPopup_content, 0, sizeof(m_cPopup_content));
	strcpy_s(m_cPopup_content, content);
	m_bShow_popup = 1;
}

bool NuklearEditor::CreateDirectoryIfNotExists(const std::string& path) {
	return m_pManager->CreateDirectoryIfNotExists(path);
}

void NuklearEditor::Clear()
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
	m_mapWindowName->clear();
	m_pSelectedNode = nullptr;

	m_mapSpr->clear();
	m_mapVariable->clear();
	m_mapFunction->clear();
	m_vecPrefab->clear();

	//m_pManager->m_luaInterface.RunFunction("Init");
}

static LRESULT CALLBACK
WindowProc(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	if (g_Editor->HandleEvent(wnd, msg, wparam, lparam))
		return 0;
	return DefWindowProcW(wnd, msg, wparam, lparam);
}

int NuklearEditor::HandleEvent(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_SIZE:
		if (m_dx7.d3d7.device)
		{
			UINT width = LOWORD(lparam);
			UINT height = HIWORD(lparam);
			if (width != 0 && height != 0)
			{
				m_dx7.nk_d3d7_resize(width, height);
			}
		}
		break;
	}

	return m_dx7.nk_d3d7_handle_event(wnd, msg, wparam, lparam);
}

BOOL NuklearEditor::InitSubWindow(HINSTANCE hInstance, HWND hMainWnd, const char* fontPath)
{
	WNDCLASSW wc;
	RECT rect = { 0, 0, 512, 960 };
	DWORD style = WS_OVERLAPPEDWINDOW;
	DWORD exstyle = WS_EX_APPWINDOW;
	
	int running = 1;

	/* Win32 */
	memset(&wc, 0, sizeof(wc));
	wc.style = CS_DBLCLKS;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszClassName = L"NuklearWindowClass2";
	RegisterClassW(&wc);

	AdjustWindowRectEx(&rect, style, FALSE, exstyle);

	wnd = CreateWindowExW(exstyle, wc.lpszClassName, L"UI Editor",
		style | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT,
		rect.right - rect.left, rect.bottom - rect.top,
		hMainWnd, NULL, wc.hInstance, NULL);

	ShowWindow(hMainWnd, true);
	ShowWindow(wnd, true);

	CHAR systemPath[MAX_PATH];
	if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_FONTS, NULL, 0, systemPath))) {
		std::cout << "System font path: " << systemPath << std::endl;
	}

	HRESULT hr;
	hr = DirectDrawCreateEx(NULL, (void**)&pDD, IID_IDirectDraw7, NULL);

	hr = pDD->SetCooperativeLevel(wnd, DDSCL_NORMAL);

	memset(&ddsd, 0, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_CAPS;
	ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
	hr = pDD->CreateSurface(&ddsd, &pDDSPrimary, NULL);
	if (FAILED(hr)) return FALSE;

	ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_VIDEOMEMORY | DDSCAPS_3DDEVICE;
	ddsd.dwWidth = 512;
	ddsd.dwHeight = 960;

	hr = pDD->CreateSurface(&ddsd, &pDDSBackBuffer, NULL);
	hr = pDD->CreateClipper(0, &pClipper, NULL);
	hr = pClipper->SetHWnd(0, wnd);
	hr = pDDSPrimary->SetClipper(pClipper);
	hr = pDD->QueryInterface(IID_IDirect3D7, (void**)&pD3D);
	hr = pD3D->CreateDevice(IID_IDirect3DHALDevice, pDDSBackBuffer, &pD3DDevice);

	D3DVIEWPORT7 vp;
	vp.dwX = 0;  // X 오프셋을 0으로 설정
	vp.dwY = 0;  // Y 오프셋을 0으로 설정
	vp.dwWidth = 512;
	vp.dwHeight = 960;
	vp.dvMinZ = 0.0f;
	vp.dvMaxZ = 1.0f;
	pD3DDevice->SetViewport(&vp);

	m_ctx = m_dx7.nk_d3d7_init(pDD, pD3DDevice);

	char path[MAX_PATH];
	HMODULE hModule = GetModuleHandle(NULL);
	if (hModule != NULL) {
		// 현재 실행 파일의 경로를 얻습니다.
		GetModuleFileNameA(hModule, path, MAX_PATH);
	}
	else {
		std::cerr << "Failed to get module handle." << std::endl;
		return FALSE;
	}

	std::string basePath(path);
	basePath = basePath.substr(0, basePath.find_last_of("\\\\"));
	std::string dataPath = "";
	if (fontPath != nullptr) {
		dataPath = basePath + fontPath;
		if (!CreateDirectoryIfNotExists(dataPath)) {
			std::cerr << "Failed to create directory: " << dataPath << std::endl;
			return FALSE;
		}
	}
	else {
		dataPath = "None";
	}

	struct nk_font_atlas* atlas;
	if (dataPath != "None") {
		m_dx7.nk_d3d7_font_stash_begin(&atlas, systemPath, 0, dataPath.c_str());
	}
	else {
		m_dx7.nk_d3d7_font_stash_begin(&atlas, systemPath, 0);
	}

	return TRUE;
}

void NuklearEditor::Render()
{
	HRESULT hr;

	int xindent = 0;
	int yindent = 0;

	RECT rect;
	GetClientRect(wnd, &rect);

	if (ClientToScreen(wnd, (POINT*)&rect) == FALSE) return;
	if (ClientToScreen(wnd, (POINT*)&rect + 1) == FALSE) return;

	rect.left += xindent;
	rect.top += yindent;

	rect.right = rect.left + 512;
	rect.bottom = rect.top + 960;

	RECT srcrect = rect;
	srcrect.right -= srcrect.left;
	srcrect.left = 0;
	srcrect.bottom -= srcrect.top;

	hr = IDirect3DDevice7_Clear(pD3DDevice, 1, NULL, D3DCLEAR_TARGET, D3DRGBA(0, 0, 0, 1), 1.0f, 0);

	hr = IDirect3DDevice7_BeginScene(pD3DDevice);
	m_dx7.nk_d3d7_render(NK_ANTI_ALIASING_ON);
	hr = IDirect3DDevice7_EndScene(pD3DDevice);

	srcrect.top = 0;

	hr = pDDSPrimary->Blt(&rect, pDDSBackBuffer, &srcrect, DDBLT_WAIT, NULL);
	if (FAILED(hr)) {
		hr = pDDSPrimary->IsLost();
		if (hr == DDERR_SURFACELOST) {
			hr = pDDSPrimary->Restore();
			if (FAILED(hr)) {
				printf("Failed to restore primary surface with error: 0x%08lx\n", hr);
				return;
			}
		}

		hr = pDDSBackBuffer->IsLost();
		if (hr == DDERR_SURFACELOST) {
			hr = pDDSBackBuffer->Restore();
			if (FAILED(hr)) {
				printf("Failed to restore primary surface with error: 0x%08lx\n", hr);
				return;
			}

			pD3DDevice->Release();
			pD3DDevice = nullptr;
			hr = pD3D->CreateDevice(IID_IDirect3DHALDevice, pDDSBackBuffer, &pD3DDevice);

			D3DVIEWPORT7 vp;
			vp.dwX = 0;  // X 오프셋을 0으로 설정
			vp.dwY = 0;  // Y 오프셋을 0으로 설정
			vp.dwWidth = 512;
			vp.dwHeight = 960;
			vp.dvMinZ = 0.0f;
			vp.dvMaxZ = 1.0f;
			pD3DDevice->SetViewport(&vp);

			nk_free(m_ctx);

			m_ctx = m_dx7.nk_d3d7_init(pDD, pD3DDevice);

			CHAR path[MAX_PATH];
			if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_FONTS, NULL, 0, path))) {
				std::cout << "System font path: " << path << std::endl;
			}

			struct nk_font_atlas* atlas;
			m_dx7.nk_d3d7_font_stash_begin(&atlas, path, 0);
		}
	}
}

#endif