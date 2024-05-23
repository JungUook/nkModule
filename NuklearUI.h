#pragma once
#ifndef NuklearUI_h__
#define NuklearUI_h__

#ifdef _DX9
#include <d3d9.h>
#elif _DX7
#include <ddraw.h>
#include <d3d.h>
#endif // _DX9

#include <vector>
#include <map>
#include <fstream>
#include "NKBase.h"

class ObjMaker {
public:
	template <typename T>
	static T* create() {
		T* obj = new T();
		return obj;
	}
};

class NKBase;
class NuklearUI
{
public:
	enum eLang {
		KOR = 0,
		JPN = 1,
		TWA = 2,
		CHI = 3
	};

public:
	NuklearUI();
	~NuklearUI();

public:
#ifdef _DX9
	void Initialize(IDirect3DDevice9* device, int width, int height, int lang);
#elif _DX7
	void Initialize(IDirectDraw7* pdd, IDirect3DDevice7* pdevice, int width, int height, int lang);
#endif // _DX9
	void Release();
	void NKInputBegin();
	void NKInputEnd();
	void Update();
	void DebugLayout();
	void DebugLayoutLeft(int width);
	void DebugLayoutLeftNodes(NKBase* pBase, nk_tree_type nkType, nk_collapse_states nkState);
	void DebugLayoutRight(int width);
	void SelectNode(NKBase* pBase);
#ifdef _DX9
	void Render(IDirect3DDevice9* device);
	int HandleEvent(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam, D3DPRESENT_PARAMETERS* present);
#elif _DX7
	void Render(IDirect3DDevice7* pdevice);
	int HandleEvent(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam);
	bool LoadSpriteData(IDirectDrawSurface7* sprite, int width, int height, int sliceSizeX = 0, int sliceSizeY = 0, int countX = 0, int countY = 0);
	bool ReadImageFile(const char* filename, IDirectDrawSurface7** pTexture);
#endif // _DX9

	//handler 包府
public:
	bool IsMouseHovering() { return m_bMouseHovering; }
	bool IsEditActive() { return m_bEditActive; }
	struct nk_context* GetContext() { return m_ctx; }
	struct nk_font* GetFont() { return m_font; }
	float GetOriginalFontSize() { return m_original_height; }
private:
	struct nk_context* m_ctx;
	struct nk_font* m_font;
	float m_original_height;

	bool m_bMouseHovering;
	bool m_bEditActive;


	//单捞磐 包府
public:
	void Add(NKBase* type);
	struct nk_image* SearchImage(int SID);

	void Remove(unsigned int id);
	void Remove(const char* name);
	void Remove(NKBase* obj);
	void Remove(int idx);
private:

#ifdef _DX9
	void AddImage(int SID, IDirect3DTexture9* texture);
	void AddImage(int SID, IDirect3DTexture9* texture, uint16_t width, uint16_t height, uint16_t region[]);
#elif _DX7
	void AddImage(int SID, IDirectDrawSurface7* texture);
	void AddImage(int SID, IDirectDrawSurface7* texture, uint16_t width, uint16_t height, uint16_t region[]);
#endif // _DX9

private:
	unsigned int m_primaryIDCheck;

	std::vector<NKBase*> m_vecObject;
	std::vector<NKBase*> m_vecModule;
	std::map<unsigned int, NKBase*> m_mapModuleID;
	std::map<const char*, NKBase*> m_mapModuleName;
	std::map<int, struct nk_image> m_mapImage;

	NKBase* m_selectedNode;
	//lua
public:
	void LoadLuaFile(const char* filePath);
	luabridge::LuaRef GetLuaTable(const char* tableName);
	bool RunFunction(const char* functionName);
	bool RunFunctionArgs(const char* functionName, const luabridge::LuaRef& args);

#ifdef _DEBUG
	void DebugLoadLuaFile(const char* filePath);
#endif // _DEBUG

private:
	void RegisterBase();

private:
	lua_State* m_lua;
#ifdef _DEBUG
	char m_filePath[256];
#endif // _DEBUG
};
#endif //NuklearUI_h__