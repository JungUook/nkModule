#pragma once
#ifndef NKBaseObject_h__
#define NKBaseObject_h__
#include "NuklearUI.h"
#include "NKProperty.h"

class NuklearUI;
class NKProperty;

class NKBase : public NKProperty
{
public:
	NKBase(nk_context* ctx, NuklearUI* pManager);
	NKBase(const NKBase& other);
	~NKBase();

	virtual std::string getClassName() const;

	//기본함수
public:
	virtual void Initialize(NuklearUI* pManager);
	virtual void Initialize(NKBase* pParent);
	virtual void Initialize();
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
	virtual void SetParent(NKBase* nkBase);
	virtual NKBase* GetParent();
	virtual std::list<NKBase*>* GetChildList();
	virtual void AddChild(NKBase* nkBase);
	virtual void LAddChild(luabridge::LuaRef ref);
	virtual void RemoveChildDisConnect(NKBase* nkBase);
	virtual void RemoveChild(NKBase* nkBase);
	virtual void LRemoveChild(luabridge::LuaRef ref);

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
	virtual void LayoutEditor();
	virtual void EditInfo();
	virtual void EditStyle();

	virtual void EditBaseName(const char* name);
	virtual const char* GetBaseName();

	virtual void CreateUI(const char* classname);
protected:
	NuklearUI* m_pManager;

	unsigned int m_primaryID;
	char m_primaryName[64];

	char m_cBaseName[64];
	char m_cEditName[64];
	int m_cEditName_len;

	int m_iNKIndex;

	nk_context* m_ctx;

	nk_flags m_flags;
	eTypeUI m_type;

	bool m_bActive;
	bool m_bHovering;
	bool m_bEditActive;

	NKBase* m_pParent;
	std::list<NKBase*> m_pChildList;
};
#endif //NKBaseObject_h__