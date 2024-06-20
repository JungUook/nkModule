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
	NKBase();
	NKBase(nk_context* ctx, NuklearUI* pManager);
	NKBase(const NKBase& other);
	virtual ~NKBase();

	//기본함수
public:
	virtual void Initialize(NuklearUI* pManager);
	virtual void Initialize(NKBase* pParent);
	virtual void Update(nk_context* ctx);
	virtual void Layout(nk_context* ctx);
	virtual void SafeRenderStart();
	virtual void SafeRenderEnd();
	virtual void Release();

	virtual nk_bool CheckMouseHover(nk_context* ctx);
	virtual nk_bool IsHovering();

	//제어함수
public:
	virtual void SetActive(bool bActive);
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

	//virtual void Load(nk_context* ctx, NuklearUI* pManager) override;
	virtual void RegistInit(NKBase* pParent);
	virtual void RegistChild(NKBase* nkBase);

	//각 객체의 기본값
public:
	virtual int GetNuklearIndex();

	virtual void SetManager(NuklearUI* manager);
	virtual void SetNuklearIndex(int index);

	// ui 편집용 함수
public:
	virtual void LayoutEditor();
	virtual void EditInfo();
	virtual void EditStyle();
	virtual void EditPrimaryName(const char* name);
	virtual void CreateUI(const char* classname);
protected:
	NuklearUI* m_pManager;
	nk_context* m_ctx;

	int m_iNKIndex;
	nk_flags m_flags;
	bool m_bActive;
	bool m_bEditActive;

	NKBase* m_pWindow;
	NKBase* m_pParent;
	std::list<NKBase*> m_pChildList;

public:
	template <class Archive>
	void serialize(Archive& ar) {
		ar(cereal::base_class<NKProperty>(this)
			, m_iNKIndex
			, m_flags
			, m_bActive
			, m_bEditActive);
	}
};
#endif //NKBaseObject_h__