#pragma once
#ifndef NKBaseObject_h__
#define NKBaseObject_h__

#define NK_INCLUDE_FIXED_TYPES
//#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_DEFAULT_FONT
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_STANDARD_VARARGS_h__
#define NK_INCLUDE_DEFAULT_ALLOCATOR_h__
#define NK_BUTTON_TRIGGER_ON_RELEASE
#include <nuklear.h>
#include <list>
#include "LuaLibrary.h"
#include "LuaBridge/LuaBridge.h"
#include "NuklearUI.h"


#ifdef _DEBUG
#define CHECK_PTR(ptr) \
    if ((ptr) == nullptr) { \
        std::cerr << "Error: Null " << __func__ <<" pointer passed to processPointer" << std::endl; \
        return; \
    }

#define CHECK_LUA_REF(ref) \
    if ((ref).isNil() || !(ref).isUserdata()) { \
        lua_State* L = (ref).state(); \
        lua_Debug ar; \
        if (lua_getstack(L, 1, &ar) && lua_getinfo(L, "Sl", &ar)) { \
            std::cerr << "Error: Invalid reference passed at " \
                      << ar.short_src << ":" << ar.currentline << std::endl; \
        } \
        return; \
    }

#else
#define CHECK_PTR(ptr) \
	if ((ptr) == nullptr) { \
		return; \
	}

#define CHECK_LUA_REF(ref) \
	if ((ref).isNil() || !(ref).isUserdata()) { \
        return; \
    }
#endif // _DEBUG
enum eTypeUI {
	eBASE = -1,
	eWINDOW = 0,
	eSPACE = 1,
	eGROUP = 2,
	ePOPUP = 3,
	eCOMBO = 4,
	eBUTTON = 5,
	eEDIT = 6,
	eIMAGE = 7,
	eLABEL = 8,
	eCOMBO_ITEM = 9,
};

class NuklearUI;
class NKBase
{
public:
	NKBase();
	~NKBase();

	virtual std::string getClassName() const;

	//기본함수
public:
	virtual void Initialize(NuklearUI* pManager);
	virtual void Initialize(NKBase* pParent);
	virtual void Update(nk_context* ctx);
	virtual void Layout(nk_context* ctx);
	virtual void SafeRenderStart();
	virtual void SafeRenderEnd();
	virtual void Release();

	//제어함수
public:
	virtual bool IsHovering();
	virtual void SetActive(bool bActive);
	virtual void SetHovering(bool bHovering);
	virtual void SetEdit(bool bEdit);
	virtual bool IsEditActive();

	virtual void SetContext(nk_context* ctx);
	virtual void SetStyle(nk_style* style);
	virtual void Setfont(nk_font* font);
	virtual void SetParent(NKBase* nkBase);
	virtual NKBase* GetParent();
	virtual std::list<NKBase*>* GetChildList();
	virtual void AddChild(NKBase* nkBase);
	virtual void LAddChild(luabridge::LuaRef ref);
	virtual void RemoveChild(NKBase* nkBase);
	virtual void LRemoveChild(luabridge::LuaRef ref);

	//속성관련 함수
public:
	virtual void SetPivot(float x, float y);
	virtual void SetPosition(float x, float y);
	virtual void SetSize(float width, float heigth);
	virtual void SetBackground(int SID);
	virtual struct nk_vec2 GetPivot();
	virtual struct nk_vec2 GetPosition();
	virtual struct nk_rect GetTransform();
	virtual float GetWidth();
	virtual float GetHeight();

	//각 객체의 기본값
public:
	virtual unsigned int GetPrimaryID();
	virtual const char* GetPrimaryName();
	virtual int GetNuklearIndex();
	virtual eTypeUI GetType();

	virtual void SetManager(NuklearUI* manager);
	virtual void SetPrimaryID(unsigned int id);
	virtual void SetPrimaryName(const char* name);
	virtual void SetNuklearIndex(int index);

	// ui 편집용 함수
public:
	virtual nk_tree_type GetTreeType() { return m_nkType; }
	virtual nk_collapse_states GetCollapseState() { return m_nkState; }

protected:
	NuklearUI* m_manager;
	const char* m_cName;

	unsigned int m_primaryID;
	char m_primaryName[64];
	int m_nkIndex;
	nk_flags m_flags;

	nk_context* m_ctx;
	nk_font* m_font;

	eTypeUI m_type;
	struct nk_vec2 m_pivot;
	struct nk_rect m_worldTransform;

	bool m_bActive;
	bool m_bHovering;
	bool m_bEditActive;

	NKBase* m_pParent;
	std::list<NKBase*> m_pChildList;

	nk_tree_type m_nkType;
	nk_collapse_states m_nkState;

#pragma region Style Setup	
public:
	nk_style m_style;

#pragma endregion // Style Setup
};
#endif //NKBaseObject_h__