#include "pch.h"

#define NK_INCLUDE_STANDARD_VARARGS
#define NK_IMPLEMENTATION
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#include "NKBase.h"

NKBase::NKBase()
{
	m_manager = nullptr;
	m_ctx = nullptr;
	m_font = nullptr;
	m_pivot.x = 0.f;
	m_pivot.y = 0.f;
	m_worldTransform.x = 0.f;
	m_worldTransform.y = 0.f;
	m_worldTransform.w = 0.f;
	m_worldTransform.h = 0.f;
	m_position.x = 0.f;
	m_position.y = 0.f;
	m_type = eBASE;
	m_flags = 0;
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

NKBase::NKBase(const NKBase& other)
{
	m_manager = other.m_manager;
	m_ctx = other.m_ctx;
	m_font = other.m_font;
	m_style = other.m_style;
	m_pivot.x = other.m_pivot.x;
	m_pivot.y = other.m_pivot.y;
	m_worldTransform.x = other.m_worldTransform.x;
	m_worldTransform.y = other.m_worldTransform.y;
	m_worldTransform.w = other.m_worldTransform.w;
	m_worldTransform.h = other.m_worldTransform.h;
	m_position.x = other.m_position.x;
	m_position.y = other.m_position.y;
	m_type = other.m_type;
	m_flags = other.m_flags;
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
	m_font = m_manager->GetFont();
	m_style = m_ctx->style;

	std::string className = getClassName().c_str();
	strcpy_s(m_primaryName, className.c_str());
	strcpy_s(m_baseName, className.c_str());
}

void NKBase::Initialize(NKBase* pParent)
{
	CHECK_PTR(pParent);
	m_pParent = pParent;
	m_manager = m_pParent->m_manager;
	m_ctx = m_pParent->m_ctx;
	m_font = m_pParent->m_font;
	m_style = m_pParent->m_style;

	std::string className = getClassName().c_str();
	strcpy_s(m_primaryName, className.c_str());
	strcpy_s(m_baseName, className.c_str());
}

void NKBase::Update(nk_context* ctx)
{
	if (m_bActive)
	{
		nk_style original = ctx->style;
		ctx->style = m_style;
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

void NKBase::SetStyle(nk_style* style)
{
	CHECK_PTR(style);
	m_style = *style;
}

void NKBase::Setfont(nk_font* font)
{
	CHECK_PTR(font);
	m_font = font;
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

void NKBase::SetPivot(float x, float y)
{
	struct nk_vec2 beforePivot = m_pivot;

	if (x < 0.f) {
		x = 0.f;
	}
	else if (x > 1) {
		x = 1.f;
	}

	if (y < 0.f) {
		y = 0.f;
	}
	else if (y > 1.f) {
		y = 1.f;
	}

	m_pivot.x = x;
	m_pivot.y = y;

	if (m_pParent) {
		m_position.x = m_worldTransform.x + (m_pivot.x * m_worldTransform.w) - (m_pParent->GetPivot().x * m_pParent->GetWidth());
		m_position.y = m_worldTransform.y + (m_pivot.y * m_worldTransform.h) - (m_pParent->GetPivot().y * m_pParent->GetHeight());
	}
	else {
		m_position.x = m_worldTransform.x + (m_pivot.x * m_worldTransform.w) - (m_manager->GetPivot()->x * m_manager->GetViewport()->w);
		m_position.y = m_worldTransform.y + (m_pivot.y * m_worldTransform.h) - (m_manager->GetPivot()->y * m_manager->GetViewport()->h);
	}
}

void NKBase::SetPosition(float x, float y)
{
	m_position.x = x;
	m_position.y = y;

	if (m_pParent) {
		m_worldTransform.x = x - (m_pivot.x * m_worldTransform.w) + (m_pParent->GetPivot().x * m_pParent->GetWidth());
		m_worldTransform.y = y - (m_pivot.y * m_worldTransform.h) + (m_pParent->GetPivot().y * m_pParent->GetHeight());
	}
	else {
		m_worldTransform.x = x - (m_pivot.x * m_worldTransform.w) + (m_manager->GetPivot()->x * m_manager->GetViewport()->w);
		m_worldTransform.y = y - (m_pivot.y * m_worldTransform.h) + (m_manager->GetPivot()->y * m_manager->GetViewport()->h);
	}
}

void NKBase::SetSize(float width, float heigth)
{
	m_worldTransform.w = width;
	m_worldTransform.h = heigth;
}

void NKBase::SetBackground(int SID)
{
	struct nk_image* img = m_manager->SearchImage(SID);

	if (img)
	{
		m_style.window.fixed_background = nk_style_item_image(*img);
	}
}

struct nk_vec2 NKBase::GetPivot()
{
	return m_pivot;
}

struct nk_vec2 NKBase::GetPosition()
{
	if (m_pParent) {
		m_position.x = m_worldTransform.x + (m_pivot.x * m_worldTransform.w) - (m_pParent->GetPivot().x * m_pParent->GetWidth());
		m_position.y = m_worldTransform.y + (m_pivot.y * m_worldTransform.h) - (m_pParent->GetPivot().y * m_pParent->GetHeight());
	}
	else {
		m_position.x = m_worldTransform.x + (m_pivot.x * m_worldTransform.w) - (m_manager->GetPivot()->x * m_manager->GetViewport()->w);
		m_position.y = m_worldTransform.y + (m_pivot.y * m_worldTransform.h) - (m_manager->GetPivot()->y * m_manager->GetViewport()->h);
	}
	return m_position;
}

struct nk_rect NKBase::GetTransform()
{
	return m_worldTransform;
}

float NKBase::GetWidth()
{
	return m_worldTransform.w;
}

float NKBase::GetHeight()
{
	return m_worldTransform.h;
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
		nk_tree_pop(m_ctx);		
	}
	if (nk_tree_push(m_ctx, NK_TREE_TAB, "Transform", NK_MINIMIZED)) {

		nk_label(m_ctx, "Pivot", NK_TEXT_LEFT);
		nk_layout_row_dynamic(m_ctx, 22, 2);
		nk_property_float(m_ctx, "#X:", .0f, &m_pivot.x, 1.f, 0.01f, 0.01f);
		nk_property_float(m_ctx, "#Y:", .0f, &m_pivot.y, 1.f, 0.01f, 0.01f);

		nk_label(m_ctx, "Position", NK_TEXT_LEFT);
		nk_layout_row_dynamic(m_ctx, 22, 2);
		nk_property_float(m_ctx, "#X:", -1920.f, &m_position.x, 1920.f, 1.f, 1.f);
		nk_property_float(m_ctx, "#Y:", -1920.f, &m_position.y, 1920.f, 1.f, 1.f);
		SetPosition(m_position.x, m_position.y);

		nk_label(m_ctx, "Rect", NK_TEXT_LEFT);
		nk_layout_row_dynamic(m_ctx, 22, 2);
		nk_property_float(m_ctx, "#W:", .0f, &m_worldTransform.w, 1920.f, 1.f, 1.f);
		nk_property_float(m_ctx, "#H:", .0f, &m_worldTransform.h, 1920.f, 1.f, 1.f);

		nk_tree_pop(m_ctx);
	}

	if (nk_tree_push(m_ctx, NK_TREE_TAB, getClassName().c_str(), NK_MINIMIZED)) {
		EditInfo();
		nk_tree_pop(m_ctx);
	}
}

void NKBase::EditInfo()
{
}

struct nk_vec2* NKBase::EditPivot()
{
	return &m_pivot;
}

struct nk_rect* NKBase::EditTransform()
{
	return &m_worldTransform;
}

void NKBase::EditBaseName(const char* name)
{
	memset(m_baseName, 0, sizeof(m_baseName));
	strcpy_s(m_baseName, name);
}

const char* NKBase::GetBaseName()
{
	return m_baseName;
}

void NKBase::CreateUI(const char* classname)
{
	m_manager->CreateUI(classname, this);
}
