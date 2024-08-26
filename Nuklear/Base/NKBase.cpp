#include "pch.h"
#include "NKBase.h"
#include "NKProperty.h"

std::string RemoveFirstCharacter(const std::string& func, const std::string& classname) {
	std::string prefix = classname + "::C";
	if (func.substr(0, prefix.size()) == prefix) {
		return func.substr(prefix.size());
	}
	return func;
}

NKBase::NKBase() : NKProperty()
, m_pManager(nullptr)
, m_pLuaManager(nullptr)
, m_ctx(nullptr)
, m_flags(0)
, m_bActive(true)
, m_bEditActive(false)
, m_pWindow(nullptr)
, m_pParent(nullptr)
, m_iNKIndex(0)
{
}

NKBase::NKBase(nk_context* ctx, NuklearUI* pManager) : NKProperty()
{
	m_pManager = pManager;
	m_pLuaManager = &pManager->m_luaInterface;
	m_ctx = ctx;
	m_bActive = true;
	m_bEditActive = false;
	m_pWindow = nullptr;
	m_pParent = nullptr;
	m_iNKIndex = 0;
	m_flags = 0;
}

NKBase::NKBase(const NKBase& other) : NKProperty(other)
{
	m_pManager = other.m_pManager;
	m_pLuaManager = &other.m_pManager->m_luaInterface;
	m_ctx = other.m_ctx;
	m_bActive = other.m_bActive;
	m_bEditActive = other.m_bEditActive;
	if (other.m_type == eWINDOW) {
		m_pWindow = this;
		m_iWindowPrimaryID = reinterpret_cast<intptr_t>(this);
	}
	else {
		m_pWindow = other.m_pWindow;
		m_iWindowPrimaryID = m_pWindow->m_iPrimaryID;
	}
	m_pParent = other.m_pParent;
	m_iParentPrimaryID = other.m_iParentPrimaryID;
	m_iNKIndex = other.m_iNKIndex;
	m_flags = other.m_flags;

	for (auto it = other.m_pChildList.begin(); it != other.m_pChildList.end(); ++it) {
		NKBase* base = *it;
		m_pManager->CopyUI(base, this);
	}
}

NKBase::~NKBase()
{
}

void NKBase::Initialize(NuklearUI* pManager, bool bStyle)
{
	CHECK_PTR(pManager);
	if (bStyle) {
		InitializeStyle(m_ctx, pManager);
	}
	if (m_type == eWINDOW) {
		m_pWindow = this;
		m_iWindowPrimaryID = reinterpret_cast<intptr_t>(this);
	}
	else {
		m_pWindow = nullptr;
		m_iWindowPrimaryID = 0;
	}
	NKProperty::Initialize();
	RegistCommand(getClassName().c_str());
}

void NKBase::Initialize(NKBase* pParent, bool bStyle)
{
	CHECK_PTR(pParent);
	m_pParent = pParent;
	m_iParentPrimaryID = pParent->GetPrimaryID();
	m_pWindow = m_pParent->m_pWindow;
	m_iWindowPrimaryID = m_pWindow->m_iPrimaryID;
	m_pManager = m_pParent->m_pManager;
	m_pLuaManager = &m_pManager->m_luaInterface;
	if (bStyle) {
		InitializeStyle(m_pParent->m_font, m_ctx->style, m_pParent->m_pParentStyle);
	}
	else {
		InitializeStyle(m_pParent->m_font, m_style, m_pParent->m_pParentStyle);
	}
	NKProperty::Initialize();
	RegistCommand(getClassName().c_str());
}

void NKBase::Update(nk_context* ctx)
{
	if (m_bActive)
	{
		//nk_style original = ctx->style;
		//ctx->style = m_pParent != nullptr && m_followParentStyle ? *m_pParentStyle : m_style;
		FollowParentStyle(ctx, m_pParent);

		nk_style original;
		StyleUpdateStart(ctx, original, m_pParent);

		LayoutBegin(ctx);

		Layout(ctx);

		LayoutEnd(ctx);

		//ctx->style = original;
		StyleUpdateEnd(ctx, original);
	}
	else {
		m_bMouseHover = false;
	}
}

void NKBase::LayoutBegin(nk_context* ctx)
{
}

void NKBase::Layout(nk_context* ctx)
{
}

void NKBase::LayoutEnd(nk_context* ctx)
{
}

void NKBase::SafeRenderStart(nk_context* ctx)
{
}

void NKBase::SafeRenderEnd(nk_context* ctx)
{
}

void NKBase::Release()
{
	for (auto it = m_pChildList.begin(); it != m_pChildList.end(); ++it) {
		NKBase* pBase = (*it);
		if (pBase) {
			pBase->Release();
			m_pManager->Remove(pBase->GetPrimaryID());
		}
	}
	m_pChildList.clear();
}

nk_bool NKBase::CheckMouseHover(nk_context* ctx)
{
	struct nk_rect b = nk_layout_space_rect_to_screen(ctx, m_sTransform);

	char primary_name[256] = { 0, };
	strncpy_s(primary_name, m_sPrimaryName.c_str(), 256);
	primary_name[255] = '\0';

	if (!nk_window_is_active(ctx, primary_name) || !nk_input_is_mouse_hovering_rect(&ctx->input, m_pWindow->GetTransform())) {
		m_bMouseHover = false;
		return m_bMouseHover;
	}

	if (nk_input_is_mouse_hovering_rect(&ctx->input, b)) {
		m_bMouseHover = true;
	}
	else {
		m_bMouseHover = false;
	}
	return m_bMouseHover;
}

nk_bool NKBase::IsHovering() {

	if (!m_bMouseHover) {
		for (auto it = m_pChildList.begin(); it != m_pChildList.end(); ++it) {
			if (m_bMouseHover) {
				m_bMouseHover = true;
			}
		}
	}

	return m_bMouseHover;
}

void NKBase::SetActive(bool bActive)
{
	m_bActive = bActive;
}

void NKBase::LSetActive(luabridge::LuaRef ref)
{
	CHECK_LUA_REF(ref);
	bool bActive = ref.cast<bool>();
	SetActive(bActive);
}

bool NKBase::CSetActive(void* param)
{
	bool* bActive = static_cast<bool*>(param);

	if (bActive) {
		SetActive(*bActive);
		return true;
	}
	return false;
}

void NKBase::SetEdit(bool bEdit)
{
	m_bEditActive = bEdit;
}

bool NKBase::IsEditActive()
{
	return m_bEditActive;
}

void NKBase::SetContext(nk_context* ctx)
{
	CHECK_PTR(ctx);
	m_ctx = ctx;
}

void NKBase::SetParent(NKBase* nkBase)
{
	CHECK_PTR(nkBase);
	m_pParent = nkBase;
	m_iParentPrimaryID = nkBase->GetPrimaryID();
}

NKBase* NKBase::GetParent()
{
	return m_pParent;
}

std::list<NKBase*>* NKBase::GetChildList()
{
	return &m_pChildList;
}

void NKBase::AddChild(NKBase* nkBase, bool bStyle)
{
	CHECK_PTR(nkBase);
	nkBase->Initialize(this, bStyle);
	m_pManager->Add(nkBase);
	m_pChildList.push_back(nkBase);
}

void NKBase::LAddChild(luabridge::LuaRef ref)
{
	CHECK_LUA_REF(ref);
	NKBase* nkBase = ref.cast<NKBase*>();
	AddChild(nkBase);
}

bool NKBase::CAddChild(void* param)
{
	void** arr = static_cast<void**>(param);

	if (arr) {
		NKBase* ptr = static_cast<NKBase*>(arr[0]);
		bool* bStyle = static_cast<bool*>(arr[1]);

		if (ptr && bStyle) {
			AddChild(ptr, *bStyle);
			return true;
		}
	}

	return false;
}

void NKBase::RemoveChildDisConnect(NKBase* nkBase)
{
	CHECK_PTR(nkBase);
	if (m_pChildList.size() > 0)
	{
		m_pChildList.remove(nkBase);
	}
}

void NKBase::RemoveChild(NKBase* nkBase)
{
	CHECK_PTR(nkBase);
	if(m_pChildList.size() > 0)
		m_pChildList.remove(nkBase);
	m_pManager->Remove(nkBase);
}

void NKBase::LRemoveChild(luabridge::LuaRef ref)
{
	CHECK_LUA_REF(ref);
	NKBase* nkBase = ref.cast<NKBase*>();
	RemoveChild(nkBase);
}

bool NKBase::CRemoveChild(void* param)
{
	NKBase* ptr = static_cast<NKBase*>(param);
	if (ptr) {
		RemoveChild(ptr);
		return true;
	}

	return false;
}

int NKBase::SizeChild()
{
	return m_pChildList.size();
}

void NKBase::ClearChild()
{
#ifdef _NKDEBUG
	m_pManager->EditorSelectorClear();
#endif // _NKDEBUG

	for (auto it = m_pChildList.begin(); it != m_pChildList.end();) {
		NKBase* ptr = *it;
		it = m_pChildList.erase(it);
		m_pManager->Remove(ptr);
	}
}

void NKBase::RegistInit(NKBase* pParent)
{
	CHECK_PTR(pParent);
	m_pParent = pParent;
	m_iParentPrimaryID = pParent->GetPrimaryID();
	m_pManager = m_pParent->m_pManager;
	m_pParentStyle = m_pParent->m_pParentStyle != nullptr ? m_pParent->m_pParentStyle : &this->m_style;

	auto windows = m_pManager->GetNodes();

	for (auto it = windows->begin(); it != windows->end(); ++it) {
		NKBase* pWin = *it;

		if (pWin->GetPrimaryID() == m_iWindowPrimaryID) {
			m_pWindow = pWin;
			m_iWindowPrimaryID = pWin->GetPrimaryID();
		}
	}
	RegistCommand(getClassName().c_str());
}

void NKBase::RegistChild(NKBase* pBase)
{
	CHECK_PTR(pBase);
	pBase->RegistInit(this);
	m_pChildList.push_back(pBase);
}

void NKBase::LRegistChild(luabridge::LuaRef ref)
{
	CHECK_LUA_REF(ref);
	NKBase* nkBase = ref.cast<NKBase*>();
	RemoveChild(nkBase);
}

void NKBase::ResetWindowID(NKBase* pBase)
{
	CHECK_PTR(pBase);
	m_pWindow = pBase->m_pWindow;
	m_iWindowPrimaryID = m_pWindow->m_iPrimaryID;

	
	for (auto it = m_pChildList.begin(); it != m_pChildList.end(); ++it) {
		NKBase* ptr = *it;
		ptr->ResetWindowID(pBase);
	}
}

void NKBase::ResetParentID(NKBase* pBase)
{
	m_pParent = pBase;
	m_iParentPrimaryID = m_pParent->m_iPrimaryID;

	for (auto it = m_pChildList.begin(); it != m_pChildList.end(); ++it) {
		NKBase* ptr = *it;
		ptr->ResetParentID(this);
	}
}

void NKBase::MoveForward()
{
	std::list<NKBase*>* lst = m_pParent->GetChildList();

	auto it = std::find(lst->begin(), lst->end(), this);
	if (it != lst->end() && it != lst->begin()) {
		auto prev_it = std::prev(it);
		m_pManager->SwapElements((*it)->m_iNKIndex, (*prev_it)->m_iNKIndex);
		std::iter_swap(it, prev_it);
	}
}

void NKBase::MoveBackward()
{
	std::list<NKBase*>* lst = m_pParent->GetChildList();

	auto it = std::find(lst->begin(), lst->end(), this);
	if (it != lst->end()) {
		auto next_it = std::next(it);
		if (next_it != lst->end()) {
			m_pManager->SwapElements((*it)->m_iNKIndex, (*next_it)->m_iNKIndex);
			std::iter_swap(it, next_it);
		}
	}
}

void NKBase::MoveFront()
{
	std::list<NKBase*>* lst = m_pParent->GetChildList();

	auto it = std::find(lst->begin(), lst->end(), this);
	if (it != lst->end() && it != lst->begin()) {
		m_pManager->MoveToBefore(this->m_iNKIndex, (*lst->begin())->m_iNKIndex);
		lst->splice(lst->begin(), *lst, it);
	}
}

void NKBase::MoveBack()
{
	std::list<NKBase*>* lst = m_pParent->GetChildList();

	auto it = std::find(lst->begin(), lst->end(), this);
	if (it != lst->end() && it != std::prev(lst->end())) {
		m_pManager->MoveToAfter(this->m_iNKIndex, (*std::prev(lst->end()))->m_iNKIndex);
		lst->splice(lst->end(), *lst, it);
	}
}

void NKBase::RegistCommand(const char* classname)
{
	MAKE_INTERFACE(m_mapFunc, this, NKBase::CSetActive, "NKBase");
	MAKE_INTERFACE(m_mapFunc, this, NKBase::CAddChild, "NKBase");
	MAKE_INTERFACE(m_mapFunc, this, NKBase::CRemoveChild, "NKBase");
	MAKE_INTERFACE(m_mapFunc, this, NKBase::CEditPrimaryName, "NKBase");
	MAKE_INTERFACE(m_mapFunc, this, NKBase::CEditWindowName, "NKBase");

	//transform
	//MAKE_INTERFACE(m_mapFunc, this, NKBase::CEditWindowName, "NKBase");
}

bool NKBase::ProcessCommand(const char* command, void* param)
{
	auto found = m_mapFunc.find(command);
	if (found != m_mapFunc.end()) {
		auto func = found->second;
		return (func)(param);
	}
	return false;
}

int NKBase::GetNuklearIndex()
{
	return m_iNKIndex;
}

void NKBase::Setfont(nk_font* font)
{
	NKBaseStyle::Setfont(font);
	m_style.font = m_ctx->style.font;
}

void NKBase::SetManager(NuklearUI* manager)
{
	CHECK_PTR(manager);
	m_pManager = manager;
}

void NKBase::SetNuklearIndex(int index)
{
	m_iNKIndex = index;
}

void NKBase::ActiveEditor(nk_context* ctx)
{
	nk_layout_row_dynamic(ctx, 22, 1);
	nk_checkbox_label(ctx, "Active", &m_bActive);


	nk_layout_row_dynamic(ctx, 22, 1);
	nk_checkbox_label(ctx, "follow_parent_style", &m_followParentStyle);
}

void NKBase::LayoutEditor(nk_context* ctx)
{
	if (nk_tree_push(ctx, NK_TREE_TAB, "DefaultInfo", NK_MINIMIZED)) {

		nk_layout_row_dynamic(ctx, 44, 1);
		nk_label(ctx, "Primary Name", NK_TEXT_LEFT);

		nk_layout_row_dynamic(ctx, 44, 2);
		nk_label(ctx, "Current:", NK_TEXT_LEFT);
		nk_label(ctx, m_sPrimaryName.c_str(), NK_TEXT_RIGHT);
		nk_layout_row_dynamic(ctx, 44, 1);
		nk_flags result = m_pManager->IMEInputSystem(ctx, m_cPrimaryEditName, sizeof(m_cPrimaryEditName), &m_iPrimaryEditName_len);
		if (result & NK_EDIT_COMMITED)
		{
			EditPrimaryName(m_cPrimaryEditName);
		}

		nk_layout_row_dynamic(ctx, 44, 1);
		nk_label(ctx, "Window Name", NK_TEXT_LEFT);

		nk_layout_row_dynamic(ctx, 44, 2);
		nk_label(ctx, "Current:", NK_TEXT_LEFT);
		nk_label(ctx, m_sWindowName.c_str(), NK_TEXT_RIGHT);
		nk_layout_row_dynamic(ctx, 44, 1);
		result = m_pManager->IMEInputSystem(ctx, m_cWindowEditName, sizeof(m_cWindowEditName), &m_iWindowEditName_len);
		if (result & NK_EDIT_COMMITED)
		{
			EditWindowName(m_cWindowEditName);
		}

		nk_layout_row_dynamic(ctx, 44, 1);
		nk_label(ctx, "Node Name", NK_TEXT_LEFT);

		nk_layout_row_dynamic(ctx, 44, 2);
		nk_label(ctx, "Current:", NK_TEXT_LEFT);
		nk_label(ctx, m_sBaseName.c_str(), NK_TEXT_RIGHT);
		nk_layout_row_dynamic(ctx, 44, 1);
		result = m_pManager->IMEInputSystem(ctx, m_cBaseEditName, sizeof(m_cBaseEditName), &m_iBaseEditName_len);
		if (result & NK_EDIT_COMMITED)
		{
			SetBaseName(m_cBaseEditName);
		}

		if (m_pParent != nullptr) {
			nk_layout_row_dynamic(ctx, 44, 2);
			if (nk_button_label(ctx, "Up")) {
				MoveForward();
			}
			if (nk_button_label(ctx, "Down")) {
				MoveBackward();
			}
			if (nk_button_label(ctx, "Top")) {
				MoveFront();
			}
			if (nk_button_label(ctx, "Bottom")) {
				MoveBack();
			}
		}	

		nk_tree_pop(ctx);
	}

	PropertyTransform(ctx, m_pParent, m_pManager);

	if (nk_tree_push(ctx, NK_TREE_TAB, getClassName().c_str(), NK_MINIMIZED)) {
		EditInfo(ctx);
		nk_tree_pop(ctx);
	}

	if (nk_tree_push(ctx, NK_TREE_TAB, "Style", NK_MINIMIZED)) {
		EditStyle(ctx);
		nk_tree_pop(ctx);
	}
}

void NKBase::EditInfo(nk_context* ctx)
{
}

void NKBase::EditStyle(nk_context* ctx)
{
}

void NKBase::EditPrimaryName(const char* name)
{
	if (!m_pManager->SetPrimaryname(this, name)) {
#ifdef _NKDEBUG
		m_pManager->ErrorPopup("There is already a primary name. primaryname cannot be duplicated.");
#endif
	}
}

void NKBase::LEditPrimaryName(luabridge::LuaRef ref)
{
	CHECK_LUA_REF(ref);
	std::string str = ref.cast<std::string>();
	EditPrimaryName(str.c_str());
}

bool NKBase::CEditPrimaryName(void* param)
{
	const char** text = static_cast<const char**>(param);

	if (text) {
		EditPrimaryName(*text);
		return true;
	}
	return false;
}

void NKBase::EditWindowName(const char* name)
{
	if (m_sWindowName.compare(name) == 0) {
		return;
	}
	SetWindowName(name);
}

void NKBase::LEditWindowName(luabridge::LuaRef ref)
{
	CHECK_LUA_REF(ref);
	std::string str = ref.cast<std::string>();
	EditWindowName(str.c_str());
}

bool NKBase::CEditWindowName(void* param)
{
	const char** text = static_cast<const char**>(param);

	if (text) {
		EditWindowName(*text);
		return true;
	}
	return false;
}

void NKBase::LSetPosition(luabridge::LuaRef ref)
{
	CHECK_LUA_REF(ref);
	struct nk_vec2 pos = ref.cast<struct nk_vec2>();
	SetPosition(m_pParent, m_pManager, pos.x, pos.y);
}

bool NKBase::CSetPosition(void* param)
{
	void** arr = static_cast<void**>(param);
	if (arr) {
		float* fArr = static_cast<float*>(*arr);
		if (fArr) {
			SetPosition(m_pParent, m_pManager, fArr[0], fArr[1]);
			return true;
		}
	}

	return false;
}

struct nk_vec2 NKBase::LGetPosition()
{
	return m_sPosition;
}

bool NKBase::CGetPosition(void* param)
{
	return false;
}

void NKBase::LSetSize(luabridge::LuaRef ref)
{
	CHECK_LUA_REF(ref);
}

bool NKBase::CSetSize(void* param)
{
	return false;
}

void NKBase::LGetSize(luabridge::LuaRef ref)
{
	CHECK_LUA_REF(ref);
}

bool NKBase::CGetSize(void* param)
{
	return false;
}

NKBase* NKBase::CreateUI(const char* classname)
{
	return m_pManager->CreateUI(classname, this);
}

void NKBase::FollowParentStyle(nk_context* ctx, NKBaseStyle* pParent)
{
	NKBaseStyle::FollowParentStyle(ctx, pParent);

	for (auto it = m_pChildList.begin(); it != m_pChildList.end(); ++it) {
		NKBase* pChild = *it;
		pChild->FollowParentStyle(ctx, this);
	}
}

void NKBase::GetPrefab(std::vector<NKBase*>& vecSave)
{
	vecSave.push_back(this);

	for (auto it = m_pChildList.begin(); it != m_pChildList.end(); ++it) {
		NKBase* pChild = *it;
		pChild->GetPrefab(vecSave);
	}
}
