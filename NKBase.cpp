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
	m_pWindow = other.m_pWindow;
	m_pParent = other.m_pParent;
	m_iNKIndex = other.m_iNKIndex;
	m_flags = other.m_flags;
}

NKBase::~NKBase()
{
}

void NKBase::Initialize(NuklearUI* pManager) 
{
	CHECK_PTR(pManager);
	InitializeStyle(m_ctx, pManager);
	if (m_type == eWINDOW) {
		m_pWindow = this;
	}
	else {
		m_pWindow = nullptr;
	}
	NKProperty::Initialize();
}

void NKBase::Initialize(NKBase* pParent)
{
	CHECK_PTR(pParent);
	m_pParent = pParent;
	m_iParentPrimaryID = pParent->GetPrimaryID();
	m_pWindow = m_pParent->m_pWindow;
	m_pManager = m_pParent->m_pManager;
	InitializeStyle(m_pParent->m_font, m_ctx->style, m_pParent->m_pParentStyle);
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

void NKBase::SafeRenderStart()
{
}

void NKBase::SafeRenderEnd()
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

void NKBase::AddChild(NKBase* nkBase)
{
	CHECK_PTR(nkBase);
	nkBase->Initialize(this);
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

//void NKBase::Load(nk_context* ctx, NuklearUI* pManager)
//{
//	NKBaseStyle::Load(ctx, pManager);
//	m_ctx = ctx;
//	m_pManager = pManager;
//	if (m_type == eWINDOW) {
//		m_pWindow = this;
//	}
//	else {
//		m_pWindow = nullptr;
//	}
//}

void NKBase::RegistInit(NKBase* pParent)
{
	CHECK_PTR(pParent);
	m_pParent = pParent;
	m_iParentPrimaryID = pParent->GetPrimaryID();
	m_pWindow = m_pParent->m_pWindow;
	m_pManager = m_pParent->m_pManager;
	m_pParentStyle = m_pParent->m_pParentStyle != nullptr ? m_pParent->m_pParentStyle : &this->m_style;
}

void NKBase::RegistChild(NKBase* nkBase)
{
	CHECK_PTR(nkBase);
	nkBase->RegistInit(this);
	m_pChildList.push_back(nkBase);
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

void NKBase::LayoutEditor()
{
	if (nk_tree_push(m_ctx, NK_TREE_TAB, "DefaultInfo", NK_MINIMIZED)) {
		nk_layout_row_dynamic(m_ctx, 44, 1);
		nk_label(m_ctx, "Window Name", NK_TEXT_LEFT);
		nk_flags result = nk_edit_string(m_ctx, NK_EDIT_SIMPLE | NK_EDIT_SIG_ENTER, m_cprimaryEditName, &m_cprimaryEditName_len, 64, nk_filter_default);

		if (result & NK_EDIT_COMMITED)
		{
			EditPrimaryName(m_cprimaryEditName);
		}

		nk_label(m_ctx, "Node Name", NK_TEXT_LEFT);
		result = nk_edit_string(m_ctx, NK_EDIT_SIMPLE | NK_EDIT_SIG_ENTER, m_cBaseEditName, &m_cBaseEditName_len, 64, nk_filter_default);

		if (result & NK_EDIT_COMMITED)
		{
			SetBaseName(m_cBaseEditName);
		}

		FollowParentStyle(m_ctx, m_pParent);

		nk_tree_pop(m_ctx);		
	}

	PropertyTransform(m_ctx, m_pParent, m_pManager);

	if (nk_tree_push(m_ctx, NK_TREE_TAB, getClassName().c_str(), NK_MINIMIZED)) {
		EditInfo();
		nk_tree_pop(m_ctx);
	}

	if (nk_tree_push(m_ctx, NK_TREE_TAB, "Style", NK_MINIMIZED)) {
		EditStyle();
		nk_tree_pop(m_ctx);
	}
}

void NKBase::EditInfo()
{
}

void NKBase::EditStyle()
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