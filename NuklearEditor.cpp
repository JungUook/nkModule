#include "pch.h"
#include "NuklearEditor.h"
#include "UiLibrary.h"

NuklearEditor::NuklearEditor()
{
	m_manager = nullptr;

	m_vecObject = nullptr;
	m_vecModule = nullptr;
	m_mapModuleID = nullptr;
	m_mapModuleName = nullptr;
	m_mapImage = nullptr;
	m_mapSpr = nullptr;

	m_option = 0;
	m_selectedNode = nullptr;
	m_deletedNode = nullptr;

	m_show_popup = 0;
	memset(m_popup_content, 0, sizeof(m_popup_content));
}

NuklearEditor::~NuklearEditor()
{
}

void NuklearEditor::EditorInit(NuklearUI* manager, std::vector<NKBase*>* obj, std::vector<NKBase*>* module, std::map<unsigned int, NKBase*>* moduleID, std::map<std::string, NKBase*>* moduleName, std::map<int, struct nk_image>* image, std::map<std::string, sprData*>* spr)
{
	m_selectedNode = nullptr;
	m_deletedNode = nullptr;
	m_manager = manager;
	m_vecObject = obj;
	m_vecModule = module;
	m_mapModuleID = moduleID;
	m_mapModuleName = moduleName;
	m_mapImage = image;
	m_mapSpr = spr;
}

void NuklearEditor::EditorLayout(nk_context* ctx, struct nk_rect debugRect)
{
	if (nk_begin(ctx, "debug", debugRect, NK_WINDOW_TITLE | NK_WINDOW_MINIMIZABLE | NK_WINDOW_MOVABLE))
	{
		nk_layout_row_dynamic(ctx, 50.f, 2);
		if (nk_button_label(ctx, "New"))
		{
			NKWindow* pWin = new NKWindow();
			m_manager->Add(pWin);
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

			m_manager->RunFunction("Init");
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

		static int groupLeft = 150;

		if (m_option != eFILE) {
			nk_layout_row_dynamic(ctx, 22, 1);
			nk_property_int(ctx, "#Left:", 150, &groupLeft, debugRect.w - 100, 1, 1);

			float row_layout[2];
			row_layout[0] = groupLeft;
			row_layout[1] = debugRect.w -25 - groupLeft;

			nk_layout_row(ctx, NK_STATIC, debugRect.h - 175, 2, row_layout);
			NodeLayout(ctx, row_layout[0]);
			InfoLayout(ctx, row_layout[1]);
		}
		else {
			FileLayout(ctx);
		}

		if (m_show_popup) {
			if (nk_popup_begin(ctx, NK_POPUP_STATIC, "Error", NK_WINDOW_TITLE, nk_rect(debugRect.w / 2 - 400 / 2, debugRect.h / 2 - 100 / 2, 400, 200)))	{
				
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
		for (int i = 0; i < m_vecObject->size(); ++i)
		{
			NodesLayout(ctx, m_vecObject->at(i), NK_TREE_TAB, NK_MINIMIZED);
		}
		//for (i = 0; i < 16; ++i)
			//nk_selectable_label(m_ctx, (selected[i]) ? "Selected" : "Unselected", NK_TEXT_LEFT, &selected[i]);
		nk_group_end(ctx);
	}

	if (m_deletedNode) {
		m_manager->Remove(m_deletedNode);
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
				nk_property_float(ctx, "#X:", .0f, &m_manager->GetPivot()->x, 1.f, 0.01f, 0.01f);
				nk_property_float(ctx, "#Y:", .0f, &m_manager->GetPivot()->y, 1.f, 0.01f, 0.01f);

				static char viewportStr[32] = { 0, };

				nk_label(ctx, "Viewport", NK_TEXT_LEFT);

				sprintf_s(viewportStr, "%.1f", m_manager->GetViewport()->w);
				nk_layout_row_dynamic(ctx, 22, 2);
				nk_label(ctx, "Width:", NK_TEXT_LEFT);
				nk_label(ctx, viewportStr, NK_TEXT_RIGHT);


				sprintf_s(viewportStr, "%.1f", m_manager->GetViewport()->h);
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
		m_manager->OpenFileDialog();
	}

	static char selectedFilename[260] = { 0, };
	float ratio[2] = { 0.8f, 0.2f };
	nk_layout_row_dynamic(ctx, 500, 1);
	if (nk_group_begin(ctx, "File List", NK_WINDOW_TITLE)) {

		nk_layout_row(ctx, NK_DYNAMIC, 22, 2, ratio);
		for (auto it = m_mapSpr->begin(); it != m_mapSpr->end(); ++it) {
			std::filesystem::path filePath((*it).first.c_str());
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
			m_manager->GetImage(selectedFilename, img);
			nk_layout_row_dynamic(ctx, 300, 1);
			nk_image(ctx, img);
			nk_group_end(ctx);
		}
	}
}

void NuklearEditor::OpenErrorPopup(const char* content)
{
	memset(m_popup_content, 0, sizeof(m_popup_content));
	strcpy_s(m_popup_content, content);
	m_show_popup = 1;
}
