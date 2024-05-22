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
	m_type = eBASE;
	m_flags = 0;
	m_bActive = true;
	m_bEditActive = false;
	m_bHovering = false;
	m_pParent = nullptr;
	m_primaryID = 0;
	m_nkIndex = 0;
	memset(m_primaryName, 0, sizeof(m_primaryName));

	std::string className = getClassName().c_str();
	m_cName = className.c_str();
	strcpy_s(m_primaryName, m_cName);
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
	m_style = m_ctx->style;
	m_font = m_manager->GetFont();
}

void NKBase::Initialize(NKBase* pParent)
{
	CHECK_PTR(pParent);
	m_pParent = pParent;
	m_manager = m_pParent->m_manager;
	m_ctx = m_pParent->m_ctx;
	m_font = m_pParent->m_font;
	m_style = m_pParent->m_style;
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


	char primaryName[256] = { 0, };
	strcpy_s(primaryName, nkBase->GetPrimaryName());
	int idx = 1;
	while (true)
	{
		bool bFound = false;
		auto it = m_pChildList.begin();
		for (; it != m_pChildList.end(); ++it)
		{
			if (strcmp(primaryName, (*it)->GetPrimaryName()) == 0) {
				bFound = true;
				break;
			}
			else {
				bFound = false;
			}
		}

		if (!bFound) {
			nkBase->SetPrimaryName(primaryName);
			break;
		}
		else {
			sprintf_s(primaryName, "%s%d", nkBase->GetPrimaryName(), idx++);
		}
	}

	m_manager->Add(nkBase);
	m_pChildList.push_back(nkBase);
}

void NKBase::LAddChild(luabridge::LuaRef ref)
{
	CHECK_LUA_REF(ref);
	NKBase* nkBase = ref.cast<NKBase*>();
	AddChild(nkBase);
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
}

void NKBase::SetPosition(float x, float y)
{
	m_worldTransform.x = x + (m_pivot.x * m_worldTransform.w);
	m_worldTransform.y = y + (m_pivot.y * m_worldTransform.h);
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
	struct nk_vec2 position;
	position.x = m_worldTransform.x + (m_pivot.x * m_worldTransform.w);
	position.y = m_worldTransform.y + (m_pivot.y * m_worldTransform.h);
	return position;
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