#pragma once
#ifndef NuklearUI_h__
#define NuklearUI_h__

#include "sprLoader.h"
#include "NKLuaInterface.h"
#include "NKCereal.h"
#include "DX7Renderer.h"

#ifdef _DX9
#include <d3d9.h>
#elif _DX7
#include <ddraw.h>
#include <d3d.h>
#endif // _DX9

#include "NKBase.h"
#include "NKObjectFinder.h"

class Factory {
public:
	using FactoryMap = std::unordered_map<std::string, std::function<NKBase* (nk_context*, NuklearUI*)>>;

	template <typename T>
	static void registerChild(const std::string& className) {
		getFactoryMap()[className] = [](nk_context* ctx, NuklearUI* pManager) -> NKBase* { return new T(ctx, pManager); };
	}

	NKBase* create(const std::string& className, nk_context* ctx, NuklearUI* pManager) {
		auto it = getFactoryMap().find(className);
		if (it != getFactoryMap().end()) {
			return it->second(ctx, pManager);
		}
		return nullptr;
	}

private:
	static FactoryMap& getFactoryMap() {
		static FactoryMap factoryMap;
		return factoryMap;
	}
};

#define REGISTER_CHILD(CLASS) Factory::registerChild<CLASS>(#CLASS)		

class ObjMaker {
public:
	template <typename T>
	static T* create(nk_context* ctx, NuklearUI* pManager) {
		T* obj = new T(ctx, pManager);
		return obj;
	}
};

class NuklearUI
{
public:
	NuklearUI();
	~NuklearUI();

public:
#ifdef _DX9
	void Initialize(IDirect3DDevice9* device, int width, int height, int lang);
#elif _DX7
	void Initialize(IDirectDraw7* pdd, IDirect3DDevice7* pdevice, int width, int height, int lang, const char* fontPath = nullptr);
#endif // _DX9
	void Release();
	void NKInputBegin();
	void NKInputEnd();
	void Update();
	void FrameSkip();
#ifdef _NKDEBUG
	void DebugLayout();
	void ErrorPopup(const char* content);
#endif
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
	float GetOriginalFontSize() { return m_dx7.d3d7.original_height; }

	nk_flags IMEInputSystem(nk_context* ctx, char* buffer, int max, int* len, nk_flags flag = NK_EDIT_FIELD | NK_EDIT_SIG_ENTER, nk_plugin_filter filter = nk_filter_default);
	void IMEInputSystem(nk_context* ctx, char* memory, int* len);

#ifdef _DX7
	DX7Renderer m_dx7;
#endif

	struct nk_context* m_ctx;
	struct nk_font* m_font;

	bool m_bMouseHovering;
	bool m_bEditActive;


	//单捞磐 包府
public:
	void Register_UI();
	std::vector<NKBase*>* GetNodes();
	NKBase* SimpleCreateUI(const char* classname);
	void CreateUI(const char* classname, NKBase* parent = nullptr);
	void CopyUI(NKBase* pBase, NKBase* parent = nullptr);
	NKBase* RegistUI(NKBase* pBase);
	struct nk_vec2* GetPivot();
	struct nk_rect* GetViewport();
	void SetPrimary(NKBase* pBase);
	bool SetPrimaryname(NKBase* pBase, const char* name);
	bool SetWindowname(NKBase* pBase, const char* name);
	void Add(NKBase* type, bool bStyle = true);
	struct nk_image* SearchImage(int SID);

	bool NKCommand(const char* primaryName, const char* command, void* param);

	template <typename T>
	T* Find(const char* primaryname)
	{
		auto found = m_mapModuleName.find(primaryname);
		if (found != m_mapModuleName.end()) {
			return (T*)found->second;
		}
		else {
			return nullptr;
		}
	}

	void SwapElements(size_t index1, size_t index2);
	void UpdateIndices();
	void MoveToBefore(size_t fromIndex, size_t toIndex);
	void MoveToAfter(size_t fromIndex, size_t toIndex);

	void Move(unsigned int child, unsigned int parent);

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
	Factory m_factory;

	std::vector<NKBase*> &m_vecObject;
	std::vector<NKBase*> &m_vecModule;
	std::map<unsigned int, NKBase*> &m_mapModuleID;
	std::map<std::string, NKBase*> &m_mapModuleName;
	std::map<std::string, NKBase*>& m_mapWindowName;
	std::map<int, struct nk_image> m_mapImage;

	struct nk_vec2 m_pivot;
	struct nk_rect m_viewRect;

	std::map<unsigned int, NKObjectFinder*> m_mapOF;
#ifdef _DX7
	//spr loader
public:
	void Register_spr(sprLoader* pSpr);
	void OpenFileDialog();
	void LoadSprFile(const char* filename);
	bool GetSprite(const char* filename, int index, struct nk_image& outimg, bool bImmortal = false);
	bool GetImage(const char* filename, struct nk_image& outimg, bool bImmortal = false);
	std::map<std::string, sprData*>* GetSprMap();

	bool RegisterRenderData(sprData* pData, bool bImmortal = false);
	void ReleaseRenderData();
	void SetDirectX7(IDirectDraw7* pdd, IDirect3DDevice7* pdevice, int width, int height);

private:
	sprLoader* m_sprLoader;
	std::map<std::string, sprData*> &m_mapSpr;

	std::vector<sprData*> m_vecRenderData;
	std::vector<sprData*> m_vecImmortalRenderData;

	int m_iLanguage;
	std::string m_sFontPath;
#endif // _DX7

public:
	void LoadNode(NKBase* pBase, bool bBegin = false);
	void LoadLinkNode(NKBase* pBase);
	void ResetPrimaryID(NKBase* pBase);

public:
	NKLuaInterface m_luaInterface;
	NKCereal m_cereal;

	std::map<std::string, CustomData>& m_mapVariable;
	std::map<std::string, CustomData>& m_mapFunction;
	//SubWindow
public:
#ifdef _NKDEBUG
	BOOL InitSubWindow(HINSTANCE hInstance, HWND hMainWnd, const char* fontPath = nullptr);
	void EditorRender();
	void EditorRestore();
#endif

public:
	bool CreateDirectoryIfNotExists(const std::string& path);
};
#endif //NuklearUI_h__