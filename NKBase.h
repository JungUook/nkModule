#pragma once
#ifndef NKBaseObject_h__
#define NKBaseObject_h__
#include "NuklearUI.h"
#include "NKProperty.h"

std::string RemoveFirstCharacter(const std::string& func, const std::string& classname);
#define MAKE_INTERFACE(map, instance, func, classname) \
	map[RemoveFirstCharacter(#func, classname)] = std::function<bool(void*)>(std::bind(&func, this, std::placeholders::_1))

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
	virtual void Initialize(NuklearUI* pManager, bool bStyle = true);
	virtual void Initialize(NKBase* pParent, bool bStyle = true);
	virtual void Update(nk_context* ctx);
	virtual void LayoutBegin(nk_context* ctx);
	virtual void Layout(nk_context* ctx);
	virtual void LayoutEnd(nk_context* ctx);
	virtual void SafeRenderStart(nk_context* ctx);
	virtual void SafeRenderEnd(nk_context* ctx);
	virtual void Release();

	virtual nk_bool CheckMouseHover(nk_context* ctx);
	virtual nk_bool IsHovering();

	//제어함수
public:
	virtual void SetActive(bool bActive);
	virtual void LSetActive(luabridge::LuaRef ref);
	virtual bool CSetActive(void* param);
	virtual void SetEdit(bool bEdit);
	virtual bool IsEditActive();

	virtual void SetContext(nk_context* ctx);
	virtual void SetParent(NKBase* nkBase);
	virtual NKBase* GetParent();
	virtual std::list<NKBase*>* GetChildList();
	virtual void AddChild(NKBase* nkBase, bool bStyle = true);
	virtual void LAddChild(luabridge::LuaRef ref);
	virtual bool CAddChild(void* param);
	virtual void RemoveChildDisConnect(NKBase* nkBase);
	virtual void RemoveChild(NKBase* nkBase);
	virtual void LRemoveChild(luabridge::LuaRef ref);
	virtual bool CRemoveChild(void* param);

	//virtual void Load(nk_context* ctx, NuklearUI* pManager) override;
	virtual void RegistInit(NKBase* pParent);
	virtual void RegistChild(NKBase* pBase);
	virtual void LRegistChild(luabridge::LuaRef ref);
	virtual void ResetWindowID(NKBase* pBase);
	virtual void ResetParentID(NKBase* pBase);

	virtual void MoveForward();
	virtual void MoveBackward();
	virtual void MoveFront();
	virtual void MoveBack();

	virtual void RegistCommand(const char* classname);
	virtual bool ProcessCommand(const char* command, void* param);

	//각 객체의 기본값
public:
	virtual int GetNuklearIndex();

	virtual void SetManager(NuklearUI* manager);
	virtual void SetNuklearIndex(int index);

	// ui 편집용 함수
public:
	virtual void ActiveEditor(nk_context* ctx);
	virtual void LayoutEditor(nk_context* ctx);
	virtual void EditInfo(nk_context* ctx);
	virtual void EditStyle(nk_context* ctx);
	virtual void EditPrimaryName(const char* name);
	virtual void EditWindowName(const char* name);
	virtual void CreateUI(const char* classname);

	virtual void FollowParentStyle(nk_context* ctx, NKBaseStyle* pParent) override;
	virtual void GetPrefab(std::vector<NKBase*>& vecSave);
protected:
	NuklearUI* m_pManager;
	NKLuaInterface* m_pLuaManager;
	nk_context* m_ctx;

	int m_iNKIndex;
	nk_flags m_flags;
	nk_bool m_bActive;
	bool m_bEditActive;

	NKBase* m_pWindow;
	NKBase* m_pParent;
	std::list<NKBase*> m_pChildList;

	std::map<std::string, std::function<bool(void*)>> m_mapFunc;

public:
	template <class Archive>
	void serialize(Archive& ar, const unsigned int version) {
		ar(cereal::base_class<NKProperty>(this)
			, CEREAL_NVP(m_iNKIndex)
			, CEREAL_NVP(m_flags)
			, CEREAL_NVP(m_bActive)
			, CEREAL_NVP(m_bEditActive)
			);
	}
};
#endif //NKBaseObject_h__