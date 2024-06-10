#include "pch.h"
#include "NKBase.h"
#include "NKProperty.h"

NKBase::NKBase() : NKProperty()
{
	m_manager = nullptr;
	m_ctx = nullptr;
	m_type = eBASE;
	m_bActive = true;
	m_bEditActive = false;
	m_bHovering = false;
	m_pParent = nullptr;
	m_primaryID = 0;
	m_nkIndex = 0;
	m_editName_len = 0;
	memset(m_primaryName, 0, sizeof(m_primaryName));
	memset(m_baseName, 0, sizeof(m_baseName));
	memset(m_editName, 0, sizeof(m_editName));
}

NKBase::NKBase(const NKBase& other) : NKProperty(other)
{
	m_manager = other.m_manager;
	m_ctx = other.m_ctx;
	m_type = other.m_type;
	m_bActive = other.m_bActive;
	m_bEditActive = other.m_bEditActive;
	m_bHovering = other.m_bHovering;
	m_pParent = other.m_pParent;
	m_primaryID = 0;
	m_nkIndex = 0;
	m_editName_len = other.m_editName_len;
	memset(m_primaryName, 0, sizeof(m_primaryName));
	memset(m_baseName, 0, sizeof(m_baseName));
	memset(m_editName, 0, sizeof(m_editName));
	m_manager->Add(this);
	//m_primaryID = other.m_primaryID;
	//m_nkIndex = other.m_nkIndex;
	//strcpy_s(m_primaryName, other.m_primaryName);
	//strcpy_s(m_editName, other.m_editName);
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
	m_manager = pManager;
	m_ctx = m_manager->GetContext();
	InitializeStyle(m_ctx, pManager);
	Initialize();
}

void NKBase::Initialize(NKBase* pParent)
{
	CHECK_PTR(pParent);
	m_pParent = pParent;
	m_manager = m_pParent->m_manager;
	m_ctx = m_pParent->m_ctx;
	InitializeStyle(m_pParent->m_font, m_pParent->m_style, m_pParent->m_pParentStyle);
	Initialize();
}

void NKBase::Initialize()
{
	std::string className = getClassName().c_str();
	strcpy_s(m_primaryName, className.c_str());
	strcpy_s(m_baseName, className.c_str());

	InitializeStyle(m_ctx);
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

bool NKBase::IsHovering()
{
	return m_bHovering;
}

void NKBase::SetActive(bool bActive)
{
	m_bActive = bActive;
}

void NKBase::SetHovering(bool bHovering)
{
	m_bHovering = bHovering;
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
	m_manager->Add(nkBase);
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
	m_manager->Remove(nkBase);
}

void NKBase::LRemoveChild(luabridge::LuaRef ref)
{
	CHECK_LUA_REF(ref);
	NKBase* nkBase = ref.cast<NKBase*>();
	RemoveChild(nkBase);
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
	return m_nkIndex;
}

eTypeUI NKBase::GetType()
{
	return m_type;
}

void NKBase::SetManager(NuklearUI* manager)
{
	CHECK_PTR(manager);
	m_manager = manager;
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

void NKBase::SetNuklearIndex(int index)
{
	m_nkIndex = index;
}

void NKBase::LayoutEditor()
{
	if (nk_tree_push(m_ctx, NK_TREE_TAB, "DefaultInfo", NK_MINIMIZED)) {

		float ratio[2];
		ratio[0] = 0.3f;
		ratio[1] = 0.7f;
		nk_layout_row(m_ctx, NK_DYNAMIC, 44, 2, ratio);
		nk_label(m_ctx, "Name", NK_TEXT_LEFT);
		nk_flags result = nk_edit_string(m_ctx, NK_EDIT_SIMPLE | NK_EDIT_SIG_ENTER, m_editName, &m_editName_len, 64, nk_filter_default);

		if (result & NK_EDIT_COMMITED)
		{
			EditBaseName(m_editName);
		}

		FollowParentStyle(m_ctx, m_pParent);

		nk_tree_pop(m_ctx);		
	}

	PropertyTransform(m_ctx, m_pParent, m_manager);

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
	HeaderEditor(m_ctx, m_manager);
	WindowEditor(m_ctx, m_manager);
	ComponentEditor(m_ctx, m_manager);
}

void NKBase::EditInfoWindow()
{
	EditInfoWindowProperty(m_ctx);

	if (nk_tree_push(m_ctx, NK_TREE_NODE, "Create UI", NK_MINIMIZED)) {
		if (nk_button_label(m_ctx, "Space"))
		{
			CreateUI("NKSpace");
		}
		nk_tree_pop(m_ctx);
	}
}

void NKBase::EditBaseName(const char* name)
{
	if (!m_manager->SetPrimaryname(this, name)) {
		m_manager->ErrorPopup("There is already a primary name. primaryname cannot be duplicated.");
	}
}

const char* NKBase::GetBaseName()
{
	return m_baseName;
}

void NKBase::CreateUI(const char* classname)
{
	m_manager->CreateUI(classname, this);
}