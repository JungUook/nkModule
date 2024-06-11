#pragma once
#ifndef NuklearEditor_h__
#define NuklearEditor_h__
#include "NuklearUI.h"

class NKBase;
class NuklearUI;

enum { eNODE, eREMOVE, eCOPY, eFILE, eFUNCTION, eVARIABLE };
class NuklearEditor
{
public:
	NuklearEditor();
	~NuklearEditor();

public:
	void EditorInit(NuklearUI* manager, std::vector<NKBase*>* obj, std::vector<NKBase*>* module, std::map<unsigned int, NKBase*>* moduleID, std::map<std::string, NKBase*>* moduleName, std::map<int, struct nk_image>* image, std::map<std::string, sprData*>* spr, std::vector<CustomData>* vvariable, std::vector<CustomData>* vfunction);
	void EditorLayout(nk_context* ctx, struct nk_rect debugRect);

	void NodeLayout(nk_context* ctx, int width);
	void NodesLayout(nk_context* ctx, NKBase* pBase, nk_tree_type nkType, nk_collapse_states nkState);
	void SelectNode(NKBase* pBase);
	void CopyNode(NKBase* pBase);
	void DeleteNode(NKBase* pBase);

	void InfoLayout(nk_context* ctx, int width);

	void FileLayout(nk_context* ctx);

	void CustomDataLayout(nk_context* ctx, const char* dataName, std::vector<CustomData>* vCustom);

	void OpenErrorPopup(const char* content);

	static std::wstring utf8ToWstring(const char* str);
	static bool customCompare(const CustomData aData, const CustomData bData);
private:
	NuklearUI* m_manager;

	std::vector<NKBase*>* m_vecObject;
	std::vector<NKBase*>* m_vecModule;
	std::map<unsigned int, NKBase*>* m_mapModuleID;
	std::map<std::string, NKBase*>* m_mapModuleName;
	std::map<int, struct nk_image>* m_mapImage;
	std::map<std::string, sprData*>* m_mapSpr;

	std::vector<CustomData>* m_vecVariable;
	std::vector<CustomData>* m_vecFunction;

	int m_option;
	NKBase* m_selectedNode;
	NKBase* m_deletedNode;

	char m_popup_content[256];
	bool m_show_popup;
};


#endif //NuklearEditor_h__