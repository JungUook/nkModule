#include "pch.h"
#include "NKBase.h"
#include "NKProperty.h"

NKBase::NKBase() : NKProperty()
, m_pManager(nullptr)
, m_primaryID(0)
, m_ctx(nullptr)
, m_flags(0)
, m_type(eBASE)
, m_bActive(true)
, m_bEditActive(false)
, m_pWindow(nullptr)
, m_pParent(nullptr)
, m_iNKIndex(0)
, m_cprimaryEditName_len(0)
, m_cBaseEditName_len(0)
{
	memset(m_primaryName, 0, sizeof(m_primaryName));
	memset(m_cBaseName, 0, sizeof(m_cBaseName));
	memset(m_cprimaryEditName, 0, sizeof(m_cprimaryEditName));
	memset(m_cBaseEditName, 0, sizeof(m_cBaseEditName));
}

NKBase::NKBase(nk_context* ctx, NuklearUI* pManager) : NKProperty()
{
	m_pManager = pManager;
	m_ctx = ctx;
	m_type = eBASE;
	m_bActive = true;
	m_bEditActive = false;
	m_pWindow = nullptr;
	m_pParent = nullptr;
	m_primaryID = 0;
	m_iNKIndex = 0;
	m_cprimaryEditName_len = 0;
	m_cBaseEditName_len = 0;
	m_flags = 0;
	memset(m_primaryName, 0, sizeof(m_primaryName));
	memset(m_cBaseName, 0, sizeof(m_cBaseName));
	memset(m_cprimaryEditName, 0, sizeof(m_cprimaryEditName));
	memset(m_cBaseEditName, 0, sizeof(m_cBaseEditName));
}

NKBase::NKBase(const NKBase& other) : NKProperty(other)
{
	m_pManager = other.m_pManager;
	m_ctx = other.m_ctx;
	m_type = other.m_type;
	m_bActive = other.m_bActive;
	m_bEditActive = other.m_bEditActive;
	m_pWindow = other.m_pWindow;
	m_pParent = other.m_pParent;
	m_primaryID = 0; // NuklearUI에서 설정
	m_iNKIndex = 0; // NuklearUI에서 설정
	m_cprimaryEditName_len = other.m_cprimaryEditName_len;
	m_cBaseEditName_len = other.m_cBaseEditName_len;
	m_flags = other.m_flags;
	strcpy_s(m_primaryName, other.m_primaryName);
	strcpy_s(m_cBaseName, other.m_cBaseName);
	strcpy_s(m_cprimaryEditName, other.m_cprimaryEditName);
	strcpy_s(m_cBaseEditName, other.m_cBaseEditName);
}

NKBase::~NKBase()
{
}

std::string NKBase::getClassName() const
{
	std::string className = typeid(*this).name();
	std::string prefix = "class ";

	// Remove the prefix if it exists
	if (className.find(prefix) == 0) {
		className = className.substr(prefix.length());
	}

	return className;
}

void NKBase::Initialize(NuklearUI* pManager)
{
	CHECK_PTR(pManager);
	InitializeStyle(m_ctx, pManager);
	m_pWindow = this;
	Initialize();
}

void NKBase::Initialize(NKBase* pParent)
{
	CHECK_PTR(pParent);
	m_pParent = pParent;
	m_pWindow = m_pParent->m_pWindow;
	m_pManager = m_pParent->m_pManager;
	InitializeStyle(m_pParent->m_font, m_ctx->style, m_pParent->m_pParentStyle);
	Initialize();
}

void NKBase::Initialize()
{
	std::string className = getClassName().c_str();
	strcpy_s(m_primaryName, className.c_str());
	strcpy_s(m_cBaseName, className.c_str());
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

void NKBase::Load(nk_context* ctx, NuklearUI* pManager)
{
	NKBaseStyle::Load(ctx, pManager);
	m_ctx = ctx;
	m_pManager = pManager;
	m_pWindow = this;
}

unsigned int NKBase::GetPrimaryID()
{
	return m_primaryID;
}

const char* NKBase::GetPrimaryName()
{
	return m_primaryName;
}

int NKBase::GetNuklearIndex()
{
	return m_iNKIndex;
}

eTypeUI NKBase::GetType()
{
	return m_type;
}

void NKBase::SetManager(NuklearUI* manager)
{
	CHECK_PTR(manager);
	m_pManager = manager;
}

void NKBase::SetPrimaryID(unsigned int id)
{
	m_primaryID = id;
}

void NKBase::SetPrimaryName(const char* name)
{
	memset(m_primaryName, 0, sizeof(m_primaryName));
	strcpy_s(m_primaryName, name);
}

void NKBase::SetBaseName(const char* name)
{
	memset(m_cBaseName, 0, sizeof(m_cBaseName));
	strcpy_s(m_cBaseEditName, name);
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

const char* NKBase::GetBaseName()
{
	return m_cBaseName;
}

void NKBase::CreateUI(const char* classname)
{
	m_pManager->CreateUI(classname, this);
}