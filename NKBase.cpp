#include "pch.h"
#include "NKBase.h"
#include "NKProperty.h"

NKBase::NKBase() : NKProperty()
, m_pManager(nullptr)
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
}

void NKBase::Initialize(NKBase* pParent, bool bStyle)
{
	CHECK_PTR(pParent);
	m_pParent = pParent;
	m_iParentPrimaryID = pParent->GetPrimaryID();
	m_pWindow = m_pParent->m_pWindow;
	m_iWindowPrimaryID = m_pWindow->m_iPrimaryID;
	m_pManager = m_pParent->m_pManager;
	if (bStyle) {
		InitializeStyle(m_pParent->m_font, m_ctx->style, m_pParent->m_pParentStyle);
	}
	else {
		InitializeStyle(m_pParent->m_font, m_style, m_pParent->m_pParentStyle);
	}
	NKProperty::Initialize();
}

void NKBase::Update(nk_context* ctx)
{
	if (m_bActive)
	{
		nk_style original = ctx->style;

		ctx->style = m_pParent != nullptr && m_followParentStyle ? *m_pParentStyle : m_style;

		Layout(ctx);

		ctx->style = original;
	}
}

void NKBase::Layout(nk_context* ctx)
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
	auto it = m_pChildList.begin();
	for (; it != m_pChildList.end();) {
		NKBase* pBase = (*it);
		pBase->Release();
		it = m_pChildList.erase(it);
	}
}

nk_bool NKBase::CheckMouseHover(nk_context* ctx)
{
	struct nk_rect b = nk_layout_space_rect_to_screen(ctx, m_cTransform);

	if (!nk_window_is_active(ctx, m_pWindow->GetPrimaryName()) || !nk_input_is_mouse_hovering_rect(&ctx->input, m_pWindow->GetTransform())) {
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
	return m_bMouseHover;
}

void NKBase::SetActive(bool bActive)
{
	m_bActive = bActive;
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
}

void NKBase::RegistChild(NKBase* pBase)
{
	CHECK_PTR(pBase);
	pBase->RegistInit(this);
	m_pChildList.push_back(pBase);
}

void NKBase::ResetWindowID(NKBase* pBase)
{
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


int NKBase::GetNuklearIndex()
{
	return m_iNKIndex;
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

void NKBase::LayoutEditor(nk_context* ctx)
{
	if (nk_tree_push(ctx, NK_TREE_TAB, "DefaultInfo", NK_MINIMIZED)) {
		nk_layout_row_dynamic(ctx, 44, 1);
		nk_label(ctx, "Window Name", NK_TEXT_LEFT);
		nk_flags result = m_pManager->IMEInputSystem(ctx, m_cprimaryEditName, sizeof(m_cprimaryEditName), &m_cprimaryEditName_len);
		if (result & NK_EDIT_COMMITED)
		{
			EditPrimaryName(m_cprimaryEditName);
		}

		nk_label(ctx, "Node Name", NK_TEXT_LEFT);
		result = m_pManager->IMEInputSystem(ctx, m_cBaseEditName, sizeof(m_cBaseEditName), &m_cBaseEditName_len);

		if (result & NK_EDIT_COMMITED)
		{
			SetBaseName(m_cBaseEditName);
		}

		FollowParentStyle(ctx, m_pParent);

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
		m_pManager->ErrorPopup("There is already a primary name. primaryname cannot be duplicated.");
	}
}

void NKBase::CreateUI(const char* classname)
{
	m_pManager->CreateUI(classname, this);
}