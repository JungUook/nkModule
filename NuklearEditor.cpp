#include "pch.h"
#include "UiLibrary.h"
#include "NuklearEditor.h"
#include "NuklearUI.h"

#include <iostream>
#include <vector>
#include <algorithm>
#include <locale>
#include <string>
#include <windows.h>

NuklearEditor* g_Editor;

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
	m_nodeOption = 0;
	m_selectedNode = nullptr;
	m_deletedNode = nullptr;
	m_vecVariable = nullptr;
	m_vecFunction = nullptr;
	m_show_popup = 0;

	m_ctx = nullptr;

	pDD = nullptr;
	pDDSPrimary = nullptr;
	pDDSBackBuffer = nullptr;
	pClipper = nullptr;
	pD3D = nullptr;
	pD3DDevice = nullptr;
	memset(m_popup_content, 0, sizeof(m_popup_content));
	g_Editor = this;
}

NuklearEditor::~NuklearEditor()
{
	g_Editor = nullptr;
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

void NuklearEditor::EditorLayout(struct nk_rect debugRect)
{
	nk_context* ctx = m_ctx;
	if (nk_begin(ctx, "debug", debugRect, 0))
	{
		if (nk_tree_push(ctx, NK_TREE_TAB, "System", NK_MINIMIZED)) {

			nk_layout_row_dynamic(ctx, 50.f, 4);
			if (nk_button_label(ctx, "New"))
			{
				NKWindow* pWin = new NKWindow(ctx, m_pManager);
				m_pManager->Add(pWin);
			}
			if (nk_button_label(ctx, "Refresh"))
			{
				Clear();
			}
			if (nk_button_label(ctx, "Save")) {
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
				std::string dataPath = basePath + "\\data";
				if (!CreateDirectoryIfNotExists(dataPath)) {
					std::cerr << "Failed to create directory: " << dataPath << std::endl;
					return;
				}
				std::string filePath = dataPath + "\\nkmod.json";

				m_pManager->SaveFile(filePath);
			}
			if (nk_button_label(ctx, "Load")) {
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
				std::string dataPath = basePath + "\\data";
				if (!CreateDirectoryIfNotExists(dataPath)) {
					std::cerr << "Failed to create directory: " << dataPath << std::endl;
					return;
				}
				std::string filePath = dataPath + "\\nkmod.json";

				m_pManager->LoadFile(filePath);
			}

			nk_layout_row_dynamic(ctx, 30, 4);
			if (nk_option_label(ctx, "node", m_option == eNODE)) m_option = eNODE;
			if (nk_option_label(ctx, "file", m_option == eFILE)) m_option = eFILE;
			if (nk_option_label(ctx, "function", m_option == eFUNCTION)) m_option = eFUNCTION;
			if (nk_option_label(ctx, "variable", m_option == eVARIABLE)) m_option = eVARIABLE;

			nk_tree_pop(ctx);
		}


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
				nk_layout_row_dynamic(ctx, 30, 1);
				nk_label(ctx, "Current Node", NK_TEXT_LEFT);
				nk_layout_row_dynamic(ctx, 30, 3);
				if (nk_option_label(ctx, "select", m_nodeOption == eSELECT)) m_nodeOption = eSELECT;
				if (nk_option_label(ctx, "copy", m_nodeOption == eCOPY)) m_nodeOption = eCOPY;
				if (nk_option_label(ctx, "remove", m_nodeOption == eREMOVE)) m_nodeOption = eREMOVE;

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
	if (nk_contextual_begin(ctx, 0, nk_vec2(200, 220), nk_window_get_bounds(ctx))) {
		const char* grid_option[] = { "New Window"};
		nk_layout_row_dynamic(ctx, 25, 1);
		if (nk_contextual_item_label(ctx, grid_option[0], NK_TEXT_CENTERED))
		{
			NKWindow* pWin = new NKWindow(ctx, m_pManager);
			m_pManager->Add(pWin);
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

	if (m_deletedNode) {
		m_pManager->Remove(m_deletedNode);
		m_deletedNode = nullptr;
	}
}

void NuklearEditor::NodesLayout(nk_context* ctx, NKBase* pBase, nk_tree_type nkType, nk_collapse_states nkState)
{
	if (nk_tree_push_id(ctx, nkType, pBase->GetBaseName(), nkState, reinterpret_cast<intptr_t>(pBase)))
	{
		nk_bool bHover = nk_widget_is_hovered(ctx);
		if(bHover)
			nk_tooltip(ctx, pBase->GetBaseName());

		switch (m_nodeOption)
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
	m_selectedNode = pBase;
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
			m_selectedNode->LayoutEditor(ctx);
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
	float ratio[2] = { 0.8f, 0.2f };

	if (m_mapSpr->size() == 0) {
		memset(selectedFilename, 0, sizeof(selectedFilename));
	}

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
	nk_flags searchResult = m_pManager->IMEInputSystem(ctx, SearchFunction, sizeof(SearchFunction), &SearchFunction_Len);

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
				m_pManager->IMEInputSystem(ctx, d.name, sizeof(d.name), &d.nameLen);

				nk_label(ctx, "table: ", NK_TEXT_LEFT);
				m_pManager->IMEInputSystem(ctx, d.tableName, sizeof(d.tableName), &d.tableLen);

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

bool NuklearEditor::CreateDirectoryIfNotExists(const std::string& path)
{
	DWORD ftyp = GetFileAttributesA(path.c_str());
	if (ftyp == INVALID_FILE_ATTRIBUTES) {
		// 경로가 존재하지 않으므로 생성 시도
		if (CreateDirectoryA(path.c_str(), NULL) || GetLastError() == ERROR_ALREADY_EXISTS) {
			return true; // 생성 성공 또는 이미 존재
		}
		else {
			return false; // 생성 실패
		}
	}
	else if (ftyp & FILE_ATTRIBUTE_DIRECTORY) {
		return true; // 이미 디렉토리로 존재
	}
	return false; // 파일은 존재하지만 디렉토리가 아님
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
	m_selectedNode = nullptr;

	m_mapSpr->clear();
	m_vecVariable->clear();
	m_vecFunction->clear();

	m_pManager->RunFunction("Init");
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

BOOL NuklearEditor::InitSubWindow(HINSTANCE hInstance, HWND hMainWnd)
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

	CHAR path[MAX_PATH];
	if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_FONTS, NULL, 0, path))) {
		std::cout << "System font path: " << path << std::endl;
	}

	HRESULT hr;
	hr = DirectDrawCreateEx(NULL, (void**)&pDD, IID_IDirectDraw7, NULL);
	if (FAILED(hr)) return FALSE;

	hr = pDD->SetCooperativeLevel(wnd, DDSCL_NORMAL);
	if (FAILED(hr)) return FALSE;

	memset(&ddsd, 0, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_CAPS;
	ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
	hr = pDD->CreateSurface(&ddsd, &pDDSPrimary, NULL);
	if (FAILED(hr)) return FALSE;

	ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_3DDEVICE;
	ddsd.dwWidth = 512;
	ddsd.dwHeight = 960;

	hr = pDD->CreateSurface(&ddsd, &pDDSBackBuffer, NULL);
	if (FAILED(hr)) return FALSE;

	hr = pDD->CreateClipper(0, &pClipper, NULL);
	if (FAILED(hr)) return FALSE;
	hr = pClipper->SetHWnd(0, wnd);
	if (FAILED(hr)) return FALSE;
	hr = pDDSPrimary->SetClipper(pClipper);
	if (FAILED(hr)) return FALSE;

	hr = pDD->QueryInterface(IID_IDirect3D7, (void**)&pD3D);
	if (FAILED(hr)) return FALSE;

	hr = pD3D->CreateDevice(IID_IDirect3DHALDevice, pDDSBackBuffer, &pD3DDevice);
	if (FAILED(hr)) return FALSE;

	D3DVIEWPORT7 vp;
	vp.dwX = 0;  // X 오프셋을 0으로 설정
	vp.dwY = 0;  // Y 오프셋을 0으로 설정
	vp.dwWidth = 512;
	vp.dwHeight = 960;
	vp.dvMinZ = 0.0f;
	vp.dvMaxZ = 1.0f;
	pD3DDevice->SetViewport(&vp);

	m_ctx = m_dx7.nk_d3d7_init(pDD, pD3DDevice);

	struct nk_font_atlas* atlas;
	m_dx7.nk_d3d7_font_stash_begin(&atlas, path, 0);


	return TRUE;
}

void NuklearEditor::Render()
{
	HRESULT hr;
	hr = IDirect3DDevice7_Clear(pD3DDevice, 1, NULL, D3DCLEAR_TARGET, D3DRGBA(0, 0, 0, 1), 1.0f, 0);
	assert(SUCCEEDED(hr));

	hr = IDirect3DDevice7_BeginScene(pD3DDevice);
	assert(SUCCEEDED(hr));
	m_dx7.nk_d3d7_render(NK_ANTI_ALIASING_ON);
	hr = IDirect3DDevice7_EndScene(pD3DDevice);
	assert(SUCCEEDED(hr));


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
	srcrect.top = 0;

	pDDSPrimary->Blt(&rect, pDDSBackBuffer, &srcrect, DDBLT_WAIT, NULL);
	assert(SUCCEEDED(hr));
}
