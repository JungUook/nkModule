#pragma once

#ifdef _NKDEBUG

#ifndef NuklearEditor_h__
#define NuklearEditor_h__

class NKBase;
class NuklearUI;

namespace luabridge {
	class LuaRef;
};

enum { eNODE, eFILE, eLUA, ePREFAB };
enum { eCODE, eFUNCTION, eVARIABLE };
enum { eSELECT, eREMOVE, eCOPY, eMOVE };
class NuklearEditor
{
public:
	NuklearEditor();
	~NuklearEditor();

public:
	void EditorInit(NuklearUI* manager, std::vector<NKBase*>* obj, std::vector<NKBase*>* module, std::map<unsigned int, NKBase*>* moduleID, std::map<std::string, NKBase*>* moduleName, std::map<int, struct nk_image>* image, std::map<std::string, sprData*>* spr, std::map<std::string, CustomData>* mvariable, std::map<std::string, CustomData>* mfunction, std::vector<std::string>* vPrefab, std::vector<std::string>* vLua);
	void EditorLayout(struct nk_rect debugRect);

	void NodeLayout(nk_context* ctx, int width);
	void NodesLayout(nk_context* ctx, NKBase* pBase, nk_tree_type nkType, nk_collapse_states nkState);
	void SelectNode(NKBase* pBase);
	void CopyNode(NKBase* pBase);
	void DeleteNode(NKBase* pBase);
	void MoveRegistNode(NKBase* pBase);
	void MoveNode(NKBase* pTarget);

	void InfoLayout(nk_context* ctx, int width);

	void FileLayout(nk_context* ctx);

	void LuaDataLayout(nk_context* ctx);
	void LuaCodeLayout(nk_context* ctx);
	void CustomDataLayout(nk_context* ctx, const char* dataName, std::map<std::string, CustomData>* mCustom);
	void PrintTable(nk_context* ctx, luabridge::LuaRef ref);

	void PrefabLayout(nk_context* ctx);

	void OpenErrorPopup(const char* content);

	bool CreateDirectoryIfNotExists(const std::string& path);
	void Clear();
	bool CompileLua(const std::string& filename, const std::string& output_filename);

private:
	NuklearUI* m_pManager;

	std::vector<NKBase*>* m_vecObject;
	std::vector<NKBase*>* m_vecModule;
	std::map<unsigned int, NKBase*>* m_mapModuleID;
	std::map<std::string, NKBase*>* m_mapModuleName;
	std::map<int, struct nk_image>* m_mapImage;
	std::map<std::string, sprData*>* m_mapSpr;

	std::map<std::string, CustomData>* m_mapVariable;
	std::map<std::string, CustomData>* m_mapFunction;

	std::vector<std::string>* m_vecPrefab;
	std::vector<std::string>* m_vecLuaCode;

	int m_iOption;
	int m_iLuaOption;
	int m_iNodeOption;

	NKBase* m_pSelectedNode;
	NKBase* m_pDeletedNode;
	NKBase* m_pMoveNode;

	char m_cPopup_content[256];
	bool m_bShow_popup;

	bool m_bMovingNode;
	//subWindow
public:
	int HandleEvent(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam);
	BOOL InitSubWindow(HINSTANCE hInstance, HWND hMainWnd, const char* fontPath = nullptr);
	void Render();
	void Restore();

	nk_context* m_ctx;
#ifdef _DX7
	DX7Renderer m_dx7;

	HWND wnd;
	LPDIRECTDRAW7 pDD;
	LPDIRECTDRAWSURFACE7 pDDSPrimary;
	LPDIRECTDRAWSURFACE7 pDDSBackBuffer;
	LPDIRECTDRAWCLIPPER pClipper;
	LPDIRECT3D7 pD3D;
	LPDIRECT3DDEVICE7 pD3DDevice;
	DDSURFACEDESC2 ddsd;

	std::string m_sFontPath;
#endif
};


#endif //NuklearEditor_h__

#endif //_NKDEBUG