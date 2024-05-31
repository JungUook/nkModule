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
	m_followParentStyle = nk_true;
	m_pParentStyle = nullptr;
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
	m_pParentStyle = nullptr;
	Initialize();
}

void NKBase::Initialize(NKBase* pParent)
{
	CHECK_PTR(pParent);
	m_pParent = pParent;
	m_manager = m_pParent->m_manager;
	m_ctx = m_pParent->m_ctx;
	m_font = m_pParent->m_font;
	m_style = m_pParent->m_style;
	m_pParentStyle = m_pParent->m_pParentStyle != nullptr ? m_pParent->m_pParentStyle : &m_pParent->m_style;
	Initialize();
}

void NKBase::Initialize()
{
	std::string className = getClassName().c_str();
	strcpy_s(m_primaryName, className.c_str());
	strcpy_s(m_baseName, className.c_str());

	//header
	m_sHeader.Init(
		&m_style.window.header.normal,
		&m_ctx->style.window.header.normal,
		&m_style.window.header.hover,
		&m_ctx->style.window.header.hover,
		&m_style.window.header.active,
		&m_ctx->style.window.header.active
	);
	m_sButton_close.Init(
		&m_style.window.header.close_button,
		&m_ctx->style.window.header.close_button
	);
	m_sButton_minimize.Init(
		&m_style.window.header.minimize_button,
		&m_ctx->style.window.header.minimize_button
	);

	//window
	m_sBackground.Init(&m_style.window.fixed_background, &m_ctx->style.window.fixed_background);
	m_sScaler.Init(&m_style.window.scaler, &m_ctx->style.window.scaler);

	//component
	m_sButton_default.Init(&m_style.button, &m_ctx->style.button);
	m_sButton_contextual.Init(&m_style.contextual_button, &m_ctx->style.contextual_button);
	m_sButton_menu.Init(&m_style.menu_button, &m_ctx->style.menu_button);
	m_sToggle_option.Init(&m_style.option, &m_ctx->style.option);
	m_sToggle_checkbox.Init(&m_style.checkbox, &m_ctx->style.checkbox);
	m_sSelectable.Init(&m_style.selectable, &m_ctx->style.selectable);
	m_sSlider.Init(&m_style.slider, &m_ctx->style.slider);
	m_sProgress.Init(&m_style.progress, &m_ctx->style.progress);
	m_sProperty.Init(&m_style.property, &m_ctx->style.property);
	m_sEdit.Init(&m_style.edit, &m_ctx->style.edit);

	m_sChart.Init(&m_style.chart.background, &m_ctx->style.chart.background);

	m_sScrollbarh.Init(&m_style.scrollh, &m_ctx->style.scrollh);
	m_sScrollbarv.Init(&m_style.scrollv, &m_ctx->style.scrollv);
	m_sTab.Init(&m_style.tab, &m_ctx->style.tab);
	m_sCombo.Init(&m_style.combo, &m_ctx->style.combo);
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

		nk_layout_row_dynamic(m_ctx, 22, 1);
		nk_checkbox_label(m_ctx, "follow_parent_style", &m_followParentStyle);

		nk_tree_pop(m_ctx);		
	}
	if (nk_tree_push(m_ctx, NK_TREE_TAB, "Transform", NK_MINIMIZED)) {

		PropertyVector2("pivot", m_pivot, .0f, 1.f, 0.01f, 0.01f);
		PropertyVector2("Position", m_position, -1920.f, 1920.f, 1.f, 1.f);

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



void NKBase::HeaderEditor()
{
	if (nk_tree_push(m_ctx, NK_TREE_TAB, "header", NK_MINIMIZED)) {
		if (nk_tree_push(m_ctx, NK_TREE_NODE, "header_bg", NK_MINIMIZED)) {
			if (nk_tree_push(m_ctx, NK_TREE_NODE, "normal", NK_MINIMIZED)) {
				ItemEditor(m_sHeader.normal);
				nk_tree_pop(m_ctx);
			}
			if (nk_tree_push(m_ctx, NK_TREE_NODE, "hover", NK_MINIMIZED)) {
				ItemEditor(m_sHeader.hover);
				nk_tree_pop(m_ctx);
			}
			if (nk_tree_push(m_ctx, NK_TREE_NODE, "active", NK_MINIMIZED)) {
				ItemEditor(m_sHeader.active);
				nk_tree_pop(m_ctx);
			}
			nk_tree_pop(m_ctx);
		}

		HeaderCloseButtonEditor();
		HeaderMinimizeButtonEditor();

		if (nk_tree_push(m_ctx, NK_TREE_NODE, "label", NK_MINIMIZED)) {
			if (nk_tree_push(m_ctx, NK_TREE_NODE, "normal", NK_MINIMIZED)) {
				ColorPicker(m_style.window.header.label_normal);
				nk_tree_pop(m_ctx);
			}
			if (nk_tree_push(m_ctx, NK_TREE_NODE, "hover", NK_MINIMIZED)) {
				ColorPicker(m_style.window.header.label_hover);
				nk_tree_pop(m_ctx);
			}
			if (nk_tree_push(m_ctx, NK_TREE_NODE, "active", NK_MINIMIZED)) {
				ColorPicker(m_style.window.header.label_active);
				nk_tree_pop(m_ctx);
			}
			nk_tree_pop(m_ctx);
		}
		if (nk_tree_push(m_ctx, NK_TREE_NODE, "align", NK_MINIMIZED)) {
			if (nk_option_label(m_ctx, "left", m_style.window.header.align == NK_HEADER_LEFT)) m_style.window.header.align = NK_HEADER_LEFT;
			if (nk_option_label(m_ctx, "right", m_style.window.header.align == NK_HEADER_RIGHT)) m_style.window.header.align = NK_HEADER_RIGHT;
			nk_tree_pop(m_ctx);
		}
		if (nk_tree_push(m_ctx, NK_TREE_NODE, "padding", NK_MINIMIZED)) {
			PropertyVector2("Vector2", m_style.window.header.padding, 0.f, 1000.f, 1.f, 1.f);
			nk_tree_pop(m_ctx);
		}
		if (nk_tree_push(m_ctx, NK_TREE_NODE, "label_padding", NK_MINIMIZED)) {
			PropertyVector2("Vector2", m_style.window.header.label_padding, 0.f, 1000.f, 1.f, 1.f);
			nk_tree_pop(m_ctx);
		}
		if (nk_tree_push(m_ctx, NK_TREE_NODE, "spacing", NK_MINIMIZED)) {
			PropertyVector2("Vector2", m_style.window.header.spacing, 0.f, 1000.f, 1.f, 1.f);
			nk_tree_pop(m_ctx);
		}
		nk_tree_pop(m_ctx);
	}
}
void NKBase::HeaderCloseButtonEditor()
{
	if (nk_tree_push(m_ctx, NK_TREE_NODE, "close_button", NK_MINIMIZED)) {
		if (nk_tree_push(m_ctx, NK_TREE_NODE, "symbol", NK_MINIMIZED)) {
			if (nk_option_label(m_ctx, "NONE", m_style.window.header.close_symbol == NK_SYMBOL_NONE)) 			m_style.window.header.close_symbol = NK_SYMBOL_NONE;
			if (nk_option_label(m_ctx, "X", m_style.window.header.close_symbol == NK_SYMBOL_X)) 				m_style.window.header.close_symbol = NK_SYMBOL_X;
			if (nk_option_label(m_ctx, "UNDERSCORE", m_style.window.header.close_symbol == NK_SYMBOL_UNDERSCORE)) 		m_style.window.header.close_symbol = NK_SYMBOL_UNDERSCORE;
			if (nk_option_label(m_ctx, "CIRCLE_SOLID", m_style.window.header.close_symbol == NK_SYMBOL_CIRCLE_SOLID)) 	m_style.window.header.close_symbol = NK_SYMBOL_CIRCLE_SOLID;
			if (nk_option_label(m_ctx, "CIRCLE_OUTLINE", m_style.window.header.close_symbol == NK_SYMBOL_CIRCLE_OUTLINE))  m_style.window.header.close_symbol = NK_SYMBOL_CIRCLE_OUTLINE;
			if (nk_option_label(m_ctx, "RECT_SOLID", m_style.window.header.close_symbol == NK_SYMBOL_RECT_SOLID)) 		m_style.window.header.close_symbol = NK_SYMBOL_RECT_SOLID;
			if (nk_option_label(m_ctx, "RECT_OUTLINE", m_style.window.header.close_symbol == NK_SYMBOL_RECT_OUTLINE)) 	m_style.window.header.close_symbol = NK_SYMBOL_RECT_OUTLINE;
			if (nk_option_label(m_ctx, "TRIANGLE_UP", m_style.window.header.close_symbol == NK_SYMBOL_TRIANGLE_UP)) 	m_style.window.header.close_symbol = NK_SYMBOL_TRIANGLE_UP;
			if (nk_option_label(m_ctx, "TRIANGLE_DOWN", m_style.window.header.close_symbol == NK_SYMBOL_TRIANGLE_DOWN)) 	m_style.window.header.close_symbol = NK_SYMBOL_TRIANGLE_DOWN;
			if (nk_option_label(m_ctx, "TRIANGLE_LEFT", m_style.window.header.close_symbol == NK_SYMBOL_TRIANGLE_LEFT)) 	m_style.window.header.close_symbol = NK_SYMBOL_TRIANGLE_LEFT;
			if (nk_option_label(m_ctx, "TRIANGLE_RIGHT", m_style.window.header.close_symbol == NK_SYMBOL_TRIANGLE_RIGHT))  m_style.window.header.close_symbol = NK_SYMBOL_TRIANGLE_RIGHT;
			if (nk_option_label(m_ctx, "PLUS", m_style.window.header.close_symbol == NK_SYMBOL_PLUS)) 			m_style.window.header.close_symbol = NK_SYMBOL_PLUS;
			if (nk_option_label(m_ctx, "MINUS", m_style.window.header.close_symbol == NK_SYMBOL_MINUS)) 			m_style.window.header.close_symbol = NK_SYMBOL_MINUS;
			if (nk_option_label(m_ctx, "MAX", m_style.window.header.close_symbol == NK_SYMBOL_MAX)) 			m_style.window.header.close_symbol = NK_SYMBOL_MAX;
			nk_tree_pop(m_ctx);
		}

		CustomComponentsEditor(m_sButton_close, m_style.window.header.close_button, eTreeHeaderCloseButton);
		nk_tree_pop(m_ctx);
	}
}
void NKBase::HeaderMinimizeButtonEditor()
{
	if (nk_tree_push(m_ctx, NK_TREE_NODE, "minimize_button", NK_MINIMIZED)) {
		if (nk_tree_push(m_ctx, NK_TREE_NODE, "symbol", NK_MINIMIZED)) {
			if (nk_option_label(m_ctx, "NONE",				m_style.window.header.minimize_symbol == NK_SYMBOL_NONE)) 			m_style.window.header.minimize_symbol = NK_SYMBOL_NONE;
			if (nk_option_label(m_ctx, "X",					m_style.window.header.minimize_symbol == NK_SYMBOL_X)) 				m_style.window.header.minimize_symbol = NK_SYMBOL_X;
			if (nk_option_label(m_ctx, "UNDERSCORE",		m_style.window.header.minimize_symbol == NK_SYMBOL_UNDERSCORE)) 	m_style.window.header.minimize_symbol = NK_SYMBOL_UNDERSCORE;
			if (nk_option_label(m_ctx, "CIRCLE_SOLID",		m_style.window.header.minimize_symbol == NK_SYMBOL_CIRCLE_SOLID)) 	m_style.window.header.minimize_symbol = NK_SYMBOL_CIRCLE_SOLID;
			if (nk_option_label(m_ctx, "CIRCLE_OUTLINE",	m_style.window.header.minimize_symbol == NK_SYMBOL_CIRCLE_OUTLINE))	m_style.window.header.minimize_symbol = NK_SYMBOL_CIRCLE_OUTLINE;
			if (nk_option_label(m_ctx, "RECT_SOLID",		m_style.window.header.minimize_symbol == NK_SYMBOL_RECT_SOLID)) 	m_style.window.header.minimize_symbol = NK_SYMBOL_RECT_SOLID;
			if (nk_option_label(m_ctx, "RECT_OUTLINE",		m_style.window.header.minimize_symbol == NK_SYMBOL_RECT_OUTLINE)) 	m_style.window.header.minimize_symbol = NK_SYMBOL_RECT_OUTLINE;
			if (nk_option_label(m_ctx, "TRIANGLE_UP",		m_style.window.header.minimize_symbol == NK_SYMBOL_TRIANGLE_UP)) 	m_style.window.header.minimize_symbol = NK_SYMBOL_TRIANGLE_UP;
			if (nk_option_label(m_ctx, "TRIANGLE_DOWN",		m_style.window.header.minimize_symbol == NK_SYMBOL_TRIANGLE_DOWN)) 	m_style.window.header.minimize_symbol = NK_SYMBOL_TRIANGLE_DOWN;
			if (nk_option_label(m_ctx, "TRIANGLE_LEFT",		m_style.window.header.minimize_symbol == NK_SYMBOL_TRIANGLE_LEFT)) 	m_style.window.header.minimize_symbol = NK_SYMBOL_TRIANGLE_LEFT;
			if (nk_option_label(m_ctx, "TRIANGLE_RIGHT",	m_style.window.header.minimize_symbol == NK_SYMBOL_TRIANGLE_RIGHT))	m_style.window.header.minimize_symbol = NK_SYMBOL_TRIANGLE_RIGHT;
			if (nk_option_label(m_ctx, "PLUS",				m_style.window.header.minimize_symbol == NK_SYMBOL_PLUS)) 			m_style.window.header.minimize_symbol = NK_SYMBOL_PLUS;
			if (nk_option_label(m_ctx, "MINUS",				m_style.window.header.minimize_symbol == NK_SYMBOL_MINUS)) 			m_style.window.header.minimize_symbol = NK_SYMBOL_MINUS;
			if (nk_option_label(m_ctx, "MAX",				m_style.window.header.minimize_symbol == NK_SYMBOL_MAX)) 			m_style.window.header.minimize_symbol = NK_SYMBOL_MAX;
			nk_tree_pop(m_ctx);
		}
		CustomComponentsEditor(m_sButton_minimize, m_style.window.header.minimize_button, eTreeHeaderMinimizeButton);
		nk_tree_pop(m_ctx);
	}

	if (nk_tree_push(m_ctx, NK_TREE_NODE, "maximize_symbol", NK_MINIMIZED)) {
		if (nk_option_label(m_ctx, "NONE",			m_style.window.header.maximize_symbol == NK_SYMBOL_NONE)) 			m_style.window.header.maximize_symbol = NK_SYMBOL_NONE;
		if (nk_option_label(m_ctx, "X",				m_style.window.header.maximize_symbol == NK_SYMBOL_X)) 				m_style.window.header.maximize_symbol = NK_SYMBOL_X;
		if (nk_option_label(m_ctx, "UNDERSCORE",	m_style.window.header.maximize_symbol == NK_SYMBOL_UNDERSCORE)) 	m_style.window.header.maximize_symbol = NK_SYMBOL_UNDERSCORE;
		if (nk_option_label(m_ctx, "CIRCLE_SOLID",	m_style.window.header.maximize_symbol == NK_SYMBOL_CIRCLE_SOLID)) 	m_style.window.header.maximize_symbol = NK_SYMBOL_CIRCLE_SOLID;
		if (nk_option_label(m_ctx, "CIRCLE_OUTLINE",m_style.window.header.maximize_symbol == NK_SYMBOL_CIRCLE_OUTLINE))	m_style.window.header.maximize_symbol = NK_SYMBOL_CIRCLE_OUTLINE;
		if (nk_option_label(m_ctx, "RECT_SOLID",	m_style.window.header.maximize_symbol == NK_SYMBOL_RECT_SOLID)) 	m_style.window.header.maximize_symbol = NK_SYMBOL_RECT_SOLID;
		if (nk_option_label(m_ctx, "RECT_OUTLINE",	m_style.window.header.maximize_symbol == NK_SYMBOL_RECT_OUTLINE)) 	m_style.window.header.maximize_symbol = NK_SYMBOL_RECT_OUTLINE;
		if (nk_option_label(m_ctx, "TRIANGLE_UP",	m_style.window.header.maximize_symbol == NK_SYMBOL_TRIANGLE_UP)) 	m_style.window.header.maximize_symbol = NK_SYMBOL_TRIANGLE_UP;
		if (nk_option_label(m_ctx, "TRIANGLE_DOWN",	m_style.window.header.maximize_symbol == NK_SYMBOL_TRIANGLE_DOWN)) 	m_style.window.header.maximize_symbol = NK_SYMBOL_TRIANGLE_DOWN;
		if (nk_option_label(m_ctx, "TRIANGLE_LEFT",	m_style.window.header.maximize_symbol == NK_SYMBOL_TRIANGLE_LEFT)) 	m_style.window.header.maximize_symbol = NK_SYMBOL_TRIANGLE_LEFT;
		if (nk_option_label(m_ctx, "TRIANGLE_RIGHT",m_style.window.header.maximize_symbol == NK_SYMBOL_TRIANGLE_RIGHT))	m_style.window.header.maximize_symbol = NK_SYMBOL_TRIANGLE_RIGHT;
		if (nk_option_label(m_ctx, "PLUS",			m_style.window.header.maximize_symbol == NK_SYMBOL_PLUS)) 			m_style.window.header.maximize_symbol = NK_SYMBOL_PLUS;
		if (nk_option_label(m_ctx, "MINUS",			m_style.window.header.maximize_symbol == NK_SYMBOL_MINUS)) 			m_style.window.header.maximize_symbol = NK_SYMBOL_MINUS;
		if (nk_option_label(m_ctx, "MAX",			m_style.window.header.maximize_symbol == NK_SYMBOL_MAX)) 			m_style.window.header.maximize_symbol = NK_SYMBOL_MAX;
		nk_tree_pop(m_ctx);
	}

}

void NKBase::WindowEditor()
{
	FixedBackgroundEditor();
	BackgroundEditor();
	ScalerEditor();
	PropertiesEditor();
	PopupEditor();
	ComboEditor();
	ContextualEditor();
	MenuEditor();
	GroupEditor();
	TooltipEditor();
}
void NKBase::FixedBackgroundEditor()
{
	if (nk_tree_push(m_ctx, NK_TREE_TAB, "fixed_background", NK_MINIMIZED)) {
		ItemEditor(m_sBackground);
		nk_tree_pop(m_ctx);
	}
}
void NKBase::BackgroundEditor()
{
	if (nk_tree_push(m_ctx, NK_TREE_TAB, "background", NK_MINIMIZED)) {
		ColorPicker(m_style.window.background);
		nk_tree_pop(m_ctx);
	}
}
void NKBase::ScalerEditor()
{
	if (nk_tree_push(m_ctx, NK_TREE_TAB, "scaler", NK_MINIMIZED)) {
		ItemEditor(m_sScaler);
		nk_tree_pop(m_ctx);
	}
}
void NKBase::PropertiesEditor()
{
	if (nk_tree_push(m_ctx, NK_TREE_TAB, "properties", NK_MINIMIZED)) {
		nk_layout_row_dynamic(m_ctx, 22, 1);
		nk_property_float(m_ctx, "#border:", 0.f, &m_style.window.border, 100.f, 1.f, 1.f);
		ColorPicker(m_style.window.border_color);

		nk_property_float(m_ctx, "#rounding:", 0.f, &m_style.window.rounding, 100.f, 1.f, 1.f);
		PropertyVector2("padding", m_style.window.padding, 0.f, 1000.f, 1.f, 1.f);
		PropertyVector2("spacing", m_style.window.spacing, 0.f, 1000.f, 1.f, 1.f);
		PropertyVector2("scrollbar_size", m_style.window.scrollbar_size, 0.f, 1000.f, 1.f, 1.f);
		PropertyVector2("min_size", m_style.window.min_size, 0.f, 1000.f, 1.f, 1.f);
		nk_tree_pop(m_ctx);
	}
}
void NKBase::PopupEditor()
{
	if (nk_tree_push(m_ctx, NK_TREE_TAB, "popup", NK_MINIMIZED)) {
		nk_layout_row_dynamic(m_ctx, 22, 1);
		nk_property_float(m_ctx, "#Border:", 0.f, &m_style.window.popup_border, 100.f, 1.f, 1.f);
		ColorPicker(m_style.window.popup_border_color);
		nk_tree_pop(m_ctx);
	}
}
void NKBase::ComboEditor()
{
	if (nk_tree_push(m_ctx, NK_TREE_TAB, "combo", NK_MINIMIZED)) {
		nk_layout_row_dynamic(m_ctx, 22, 1);
		nk_property_float(m_ctx, "#Border:", 0.f, &m_style.window.combo_border, 100.f, 1.f, 1.f);
		ColorPicker(m_style.window.combo_border_color);
		nk_tree_pop(m_ctx);
	}
}
void NKBase::ContextualEditor()
{
	if (nk_tree_push(m_ctx, NK_TREE_TAB, "contextual", NK_MINIMIZED)) {
		nk_layout_row_dynamic(m_ctx, 22, 1);
		nk_property_float(m_ctx, "#Border:", 0.f, &m_style.window.contextual_border, 100.f, 1.f, 1.f);
		ColorPicker(m_style.window.contextual_border_color);
		nk_tree_pop(m_ctx);
	}
}
void NKBase::MenuEditor()
{
	if (nk_tree_push(m_ctx, NK_TREE_TAB, "menu", NK_MINIMIZED)) {
		nk_layout_row_dynamic(m_ctx, 22, 1);
		nk_property_float(m_ctx, "#Border:", 0.f, &m_style.window.menu_border, 100.f, 1.f, 1.f);
		ColorPicker(m_style.window.menu_border_color);
		nk_tree_pop(m_ctx);
	}
}
void NKBase::GroupEditor()
{
	if (nk_tree_push(m_ctx, NK_TREE_TAB, "group", NK_MINIMIZED)) {
		nk_layout_row_dynamic(m_ctx, 22, 1);
		nk_property_float(m_ctx, "#Border:", 0.f, &m_style.window.group_border, 100.f, 1.f, 1.f);
		ColorPicker(m_style.window.group_border_color);
		nk_tree_pop(m_ctx);
	}
}
void NKBase::TooltipEditor()
{
	if (nk_tree_push(m_ctx, NK_TREE_TAB, "tooltip", NK_MINIMIZED)) {
		nk_layout_row_dynamic(m_ctx, 22, 1);
		nk_property_float(m_ctx, "#Border:", 0.f, &m_style.window.tooltip_border, 100.f, 1.f, 1.f);
		ColorPicker(m_style.window.tooltip_border_color);
		nk_tree_pop(m_ctx);
	}
}

void NKBase::ComponentEditor()
{
	TextComponentEditor();
	ButtonComponentEditor();
	ContextualButtonComponentEditor();
	MenuButtonComponentEditor();
	OptionComponentEditor();
	CheckboxComponentEditor();
	SelectableComponentEditor();
	SliderComponentEditor();
	ProgressComponentEditor();
	PropertyComponentEditor();
	EditComponentEditor();
	ChartComponentEditor();
	ScrollhComponentEditor();
	ScrollvComponentEditor();
	TabComponentEditor();
	ComboComponentEditor();
}

void NKBase::TextComponentEditor()
{
	if (nk_tree_push(m_ctx, NK_TREE_TAB, "Text", NK_MINIMIZED)) {
		ColorPicker(m_style.text.color);
		PropertyVector2("padding", m_style.text.padding, 0.f, 100.f, 1.f, 0.1f);
		nk_property_float(m_ctx, "#color_factor:", 0.f, &m_style.text.color_factor, 1.f, 0.01f, 0.01f);
		nk_property_float(m_ctx, "#disabled_factor:", 0.f, &m_style.text.disabled_factor, 1.f, 0.01f, 0.01f);
		nk_tree_pop(m_ctx);
	}
}
void NKBase::ButtonComponentEditor()
{
	if (nk_tree_push(m_ctx, NK_TREE_NODE, "DefaultButton", NK_MINIMIZED)) {
		CustomComponentsEditor(m_sButton_default, m_style.button, eTreeDefaultButton);
		nk_tree_pop(m_ctx);
	}
}
void NKBase::ContextualButtonComponentEditor()
{
	if (nk_tree_push(m_ctx, NK_TREE_NODE, "ContextualButton", NK_MINIMIZED)) {
		CustomComponentsEditor(m_sButton_contextual, m_style.contextual_button, eTreeContextualButton);
		nk_tree_pop(m_ctx);
	}
}
void NKBase::MenuButtonComponentEditor()
{
	if (nk_tree_push(m_ctx, NK_TREE_NODE, "MenuButton", NK_MINIMIZED)) {
		CustomComponentsEditor(m_sButton_menu, m_style.menu_button, eTreeContextualButton);
		nk_tree_pop(m_ctx);
	}
}
void NKBase::OptionComponentEditor()
{
	if (nk_tree_push(m_ctx, NK_TREE_NODE, "Option", NK_MINIMIZED)) {
		CustomComponentsEditor(m_sToggle_option, m_style.option, eTreeOptionToggle);
		nk_tree_pop(m_ctx);
	}
}
void NKBase::CheckboxComponentEditor()
{
	if (nk_tree_push(m_ctx, NK_TREE_NODE, "Checkbox", NK_MINIMIZED)) {
		CustomComponentsEditor(m_sToggle_checkbox, m_style.checkbox, eTreeCheckboxToggle);
		nk_tree_pop(m_ctx);
	}
}
void NKBase::SelectableComponentEditor()
{
	if (nk_tree_push(m_ctx, NK_TREE_NODE, "Checkbox", NK_MINIMIZED)) {
		CustomComponentsEditor(m_sSelectable, m_style.selectable, eTreeSelectable);
		nk_tree_pop(m_ctx);
	}
}
void NKBase::SliderComponentEditor()
{
	if (nk_tree_push(m_ctx, NK_TREE_NODE, "Slider", NK_MINIMIZED)) {
		CustomComponentsEditor(m_sSlider, m_style.slider, eTreeSlider);
		nk_tree_pop(m_ctx);
	}
}
void NKBase::ProgressComponentEditor()
{
	if (nk_tree_push(m_ctx, NK_TREE_NODE, "Progress", NK_MINIMIZED)) {
		CustomComponentsEditor(m_sProgress, m_style.progress, eTreeSelectable);
		nk_tree_pop(m_ctx);
	}
}
void NKBase::PropertyComponentEditor()
{
	if (nk_tree_push(m_ctx, NK_TREE_NODE, "Property", NK_MINIMIZED)) {
		CustomComponentsEditor(m_sProperty, m_style.property, eTreeProperty);
		nk_tree_pop(m_ctx);
	}
}
void NKBase::EditComponentEditor()
{
	if (nk_tree_push(m_ctx, NK_TREE_NODE, "Edit", NK_MINIMIZED)) {
		CustomComponentsEditor(m_sEdit, m_style.edit, eTreeEdit);
		nk_tree_pop(m_ctx);
	}
}
void NKBase::ChartComponentEditor()
{
	if (nk_tree_push(m_ctx, NK_TREE_NODE, "Chart", NK_MINIMIZED)) {
		CustomComponentsEditor(m_sChart, m_style.chart, eTreeChart);
		nk_tree_pop(m_ctx);
	}
}
void NKBase::ScrollhComponentEditor()
{
	if (nk_tree_push(m_ctx, NK_TREE_NODE, "Scrollh", NK_MINIMIZED)) {
		CustomComponentsEditor(m_sScrollbarh, m_style.scrollh, eTreeScrollh);
		nk_tree_pop(m_ctx);
	}
}
void NKBase::ScrollvComponentEditor()
{
	if (nk_tree_push(m_ctx, NK_TREE_NODE, "Scrollv", NK_MINIMIZED)) {
		CustomComponentsEditor(m_sScrollbarv, m_style.scrollv, eTreeScrollv);
		nk_tree_pop(m_ctx);
	}
}
void NKBase::TabComponentEditor()
{
	if (nk_tree_push(m_ctx, NK_TREE_NODE, "Tab", NK_MINIMIZED)) {
		CustomComponentsEditor(m_sTab, m_style.tab, eTreeTab);
		nk_tree_pop(m_ctx);
	}
}
void NKBase::ComboComponentEditor()
{
	if (nk_tree_push(m_ctx, NK_TREE_NODE, "Combo", NK_MINIMIZED)) {
		CustomComponentsEditor(m_sCombo, m_style.combo, eTreeCombo);
		nk_tree_pop(m_ctx);
	}
}

void NKBase::CustomComponentsEditor(ComponentButton& tpi, struct nk_style_button& button, int id)
{
	int tree_index = 100;
	int tree_index_in = 200;
	int tree_index_in_in = 300;

	if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "background", NK_MINIMIZED, id + tree_index++)) {

		if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "normal", NK_MINIMIZED, id + tree_index_in++)) {
			ItemEditor(tpi.normal);
			nk_tree_pop(m_ctx);
		}
		if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "hover", NK_MINIMIZED, id + tree_index_in++)) {
			ItemEditor(tpi.hover);
			nk_tree_pop(m_ctx);
		}
		if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "active", NK_MINIMIZED, id + tree_index_in++)) {
			ItemEditor(tpi.active);
			nk_tree_pop(m_ctx);
		}
		if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "color_factor", NK_MINIMIZED, id + tree_index_in++)) {
			nk_layout_row_dynamic(m_ctx, 22, 1);
			nk_property_float(m_ctx, "#color:", 0.f, &button.color_factor_background, 1.f, 0.01f, 0.01f);
			nk_tree_pop(m_ctx);
		}
		nk_tree_pop(m_ctx);
	}

	if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "border", NK_MINIMIZED, id + tree_index++)) {
		nk_layout_row_dynamic(m_ctx, 22, 1);
		nk_property_float(m_ctx, "#Border:", 0.f, &button.border, 9.f, 1.f, 0.1f);
		ColorPicker(button.border_color);
		nk_tree_pop(m_ctx);
	}


	if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "text", NK_MINIMIZED, id + tree_index++)) {

		if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "background", NK_MINIMIZED, id + tree_index_in++)) {
			ColorPicker(button.text_background);
			nk_tree_pop(m_ctx);
		}
		if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "normal", NK_MINIMIZED, id + tree_index_in++)) {
			ColorPicker(button.text_normal);
			nk_tree_pop(m_ctx);
		}
		if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "hover", NK_MINIMIZED, id + tree_index_in++)) {
			ColorPicker(button.text_hover);
			nk_tree_pop(m_ctx);
		}
		if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "active", NK_MINIMIZED, id + tree_index_in++)) {
			ColorPicker(button.text_active);
			nk_tree_pop(m_ctx);
		}

		if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "align", NK_MINIMIZED, id + tree_index_in++)) {
			if (nk_option_label(m_ctx, "left", button.text_alignment == NK_HEADER_LEFT))  button.text_alignment = NK_HEADER_LEFT;
			if (nk_option_label(m_ctx, "center", button.text_alignment == NK_TEXT_CENTERED))  button.text_alignment = NK_TEXT_CENTERED;
			if (nk_option_label(m_ctx, "right", button.text_alignment == NK_HEADER_RIGHT))  button.text_alignment = NK_HEADER_RIGHT;
			nk_tree_pop(m_ctx);
		}
		if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "color_factor", NK_MINIMIZED, id + tree_index_in++)) {
			nk_layout_row_dynamic(m_ctx, 22, 1);
			nk_property_float(m_ctx, "#color:", 0.f, &button.color_factor_text, 1.f, 0.01f, 0.01f);
			nk_tree_pop(m_ctx);
		}
		nk_tree_pop(m_ctx);
	}

	if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "properties", NK_MINIMIZED, id + tree_index++)) {
		nk_layout_row_dynamic(m_ctx, 22, 1);
		nk_property_float(m_ctx, "#Rounding:", 0.f, &button.rounding, 100.f, 1.f, 0.1f);

		PropertyVector2("padding", button.padding, 0.f, 1000.f, 1.f, 1.f);
		PropertyVector2("image_padding", button.image_padding, 0.f, 1000.f, 1.f, 1.f);
		PropertyVector2("touch_padding", button.touch_padding, 0.f, 1000.f, 1.f, 1.f);

		nk_layout_row_dynamic(m_ctx, 22, 1);
		nk_label(m_ctx, "disabled_factor", NK_TEXT_LEFT);
		nk_property_float(m_ctx, "#value:", 0.f, &button.disabled_factor, 1.f, 0.01f, 0.01f);
		nk_tree_pop(m_ctx);
	}
}
void NKBase::CustomComponentsEditor(ComponentToggle& tpi, struct nk_style_toggle& toggle, int id)
{
	int tree_index = 100;
	int tree_index_in = 200;
	int tree_index_in_in = 300;

	if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "background", NK_MINIMIZED, id + tree_index++)) {

		if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "normal", NK_MINIMIZED, id + tree_index_in++)) {
			ItemEditor(tpi.normal);
			nk_tree_pop(m_ctx);
		}
		if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "hover", NK_MINIMIZED, id + tree_index_in++)) {
			ItemEditor(tpi.hover);
			nk_tree_pop(m_ctx);
		}
		if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "active", NK_MINIMIZED, id + tree_index_in++)) {
			ItemEditor(tpi.active);
			nk_tree_pop(m_ctx);
		}
		nk_tree_pop(m_ctx);
	}

	if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "border", NK_MINIMIZED, id + tree_index++)) {
		nk_layout_row_dynamic(m_ctx, 22, 1);
		nk_property_float(m_ctx, "#Border:", 0.f, &toggle.border, 9.f, 1.f, 0.1f);
		ColorPicker(toggle.border_color);
		nk_tree_pop(m_ctx);
	}

	if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "cursor", NK_MINIMIZED, id + tree_index++)) {

		if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "normal", NK_MINIMIZED, id + tree_index_in++)) {
			ItemEditor(tpi.cursor_normal);
			nk_tree_pop(m_ctx);
		}
		if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "hover", NK_MINIMIZED, id + tree_index_in++)) {
			ItemEditor(tpi.cursor_hover);
			nk_tree_pop(m_ctx);
		}
		nk_tree_pop(m_ctx);
	}

	if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "text", NK_MINIMIZED, id + tree_index++)) {

		if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "background", NK_MINIMIZED, id + tree_index_in++)) {
			ColorPicker(toggle.text_background);
			nk_tree_pop(m_ctx);
		}
		if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "normal", NK_MINIMIZED, id + tree_index_in++)) {
			ColorPicker(toggle.text_normal);
			nk_tree_pop(m_ctx);
		}
		if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "hover", NK_MINIMIZED, id + tree_index_in++)) {
			ColorPicker(toggle.text_hover);
			nk_tree_pop(m_ctx);
		}
		if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "active", NK_MINIMIZED, id + tree_index_in++)) {
			ColorPicker(toggle.text_active);
			nk_tree_pop(m_ctx);
		}

		if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "align", NK_MINIMIZED, id + tree_index_in++)) {
			if (nk_option_label(m_ctx, "left", toggle.text_alignment == NK_HEADER_LEFT))  toggle.text_alignment = NK_HEADER_LEFT;
			if (nk_option_label(m_ctx, "center", toggle.text_alignment == NK_TEXT_CENTERED))  toggle.text_alignment = NK_TEXT_CENTERED;
			if (nk_option_label(m_ctx, "right", toggle.text_alignment == NK_HEADER_RIGHT))  toggle.text_alignment = NK_HEADER_RIGHT;
			nk_tree_pop(m_ctx);
		}
		nk_tree_pop(m_ctx);
	}

	if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "properties", NK_MINIMIZED, id + tree_index++)) {
		nk_layout_row_dynamic(m_ctx, 22, 1);

		PropertyVector2("padding", toggle.padding, 0.f, 1000.f, 1.f, 1.f);
		PropertyVector2("touch_padding", toggle.touch_padding, 0.f, 1000.f, 1.f, 1.f);

		nk_layout_row_dynamic(m_ctx, 22, 1);
		nk_label(m_ctx, "spacing", NK_TEXT_LEFT);
		nk_property_float(m_ctx, "#value:", 0.f, &toggle.spacing, 100.f, 1.f, 0.1f);
		nk_label(m_ctx, "color_factor", NK_TEXT_LEFT);
		nk_property_float(m_ctx, "#value:", 0.f, &toggle.color_factor, 1.f, 0.01f, 0.01f);
		nk_label(m_ctx, "disabled_factor", NK_TEXT_LEFT);
		nk_property_float(m_ctx, "#value:", 0.f, &toggle.disabled_factor, 1.f, 0.01f, 0.01f);
		nk_tree_pop(m_ctx);
	}
}
void NKBase::CustomComponentsEditor(ComponentSelectable& tpi, struct nk_style_selectable& selectable, int id)
{
	int tree_index = 100;
	int tree_index_in = 200;
	int tree_index_in_in = 300;

	if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "background(inactive)", NK_MINIMIZED, id + tree_index++)) {

		if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "normal", NK_MINIMIZED, id + tree_index_in++)) {
			ItemEditor(tpi.normal);
			nk_tree_pop(m_ctx);
		}
		if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "hover", NK_MINIMIZED, id + tree_index_in++)) {
			ItemEditor(tpi.hover);
			nk_tree_pop(m_ctx);
		}
		if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "pressed", NK_MINIMIZED, id + tree_index_in++)) {
			ItemEditor(tpi.pressed);
			nk_tree_pop(m_ctx);
		}
		nk_tree_pop(m_ctx);
	}

	if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "background(active)", NK_MINIMIZED, id + tree_index++)) {

		if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "normal", NK_MINIMIZED, id + tree_index_in++)) {
			ItemEditor(tpi.normal_active);
			nk_tree_pop(m_ctx);
		}
		if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "hover", NK_MINIMIZED, id + tree_index_in++)) {
			ItemEditor(tpi.hover_active);
			nk_tree_pop(m_ctx);
		}
		if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "pressed", NK_MINIMIZED, id + tree_index_in++)) {
			ItemEditor(tpi.pressed_active);
			nk_tree_pop(m_ctx);
		}
		nk_tree_pop(m_ctx);
	}

	if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "text(inactive)", NK_MINIMIZED, id + tree_index++)) {

		if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "normal", NK_MINIMIZED, id + tree_index_in++)) {
			ColorPicker(selectable.text_normal);
			nk_tree_pop(m_ctx);
		}
		if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "hover", NK_MINIMIZED, id + tree_index_in++)) {
			ColorPicker(selectable.text_hover);
			nk_tree_pop(m_ctx);
		}
		if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "pressed", NK_MINIMIZED, id + tree_index_in++)) {
			ColorPicker(selectable.text_pressed);
			nk_tree_pop(m_ctx);
		}
		nk_tree_pop(m_ctx);
	}

	if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "text(active)", NK_MINIMIZED, id + tree_index++)) {

		if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "normal", NK_MINIMIZED, id + tree_index_in++)) {
			ColorPicker(selectable.text_normal_active);
			nk_tree_pop(m_ctx);
		}
		if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "hover", NK_MINIMIZED, id + tree_index_in++)) {
			ColorPicker(selectable.text_hover_active);
			nk_tree_pop(m_ctx);
		}
		if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "pressed", NK_MINIMIZED, id + tree_index_in++)) {
			ColorPicker(selectable.text_pressed_active);
			nk_tree_pop(m_ctx);
		}
		if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "background", NK_MINIMIZED, id + tree_index_in++)) {
			ColorPicker(selectable.text_background);
			nk_tree_pop(m_ctx);
		}

		if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "align", NK_MINIMIZED, id + tree_index_in++)) {
			if (nk_option_label(m_ctx, "left", selectable.text_alignment == NK_HEADER_LEFT))  selectable.text_alignment = NK_HEADER_LEFT;
			if (nk_option_label(m_ctx, "center", selectable.text_alignment == NK_TEXT_CENTERED))  selectable.text_alignment = NK_TEXT_CENTERED;
			if (nk_option_label(m_ctx, "right", selectable.text_alignment == NK_HEADER_RIGHT))  selectable.text_alignment = NK_HEADER_RIGHT;
			nk_tree_pop(m_ctx);
		}
		nk_tree_pop(m_ctx);
	}

	if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "properties", NK_MINIMIZED, id + tree_index++)) {
		nk_layout_row_dynamic(m_ctx, 22, 1);
		nk_label(m_ctx, "rounding", NK_TEXT_LEFT);
		nk_property_float(m_ctx, "#value:", 0.f, &selectable.rounding, 100.f, 1.f, 0.1f);

		PropertyVector2("padding", selectable.padding, 0.f, 1000.f, 1.f, 1.f);
		PropertyVector2("touch_padding", selectable.touch_padding, 0.f, 1000.f, 1.f, 1.f);
		PropertyVector2("image_padding", selectable.image_padding, 0.f, 1000.f, 1.f, 1.f);

		nk_layout_row_dynamic(m_ctx, 22, 1);
		nk_label(m_ctx, "color_factor", NK_TEXT_LEFT);
		nk_property_float(m_ctx, "#value:", 0.f, &selectable.color_factor, 1.f, 0.01f, 0.01f);
		nk_label(m_ctx, "disabled_factor", NK_TEXT_LEFT);
		nk_property_float(m_ctx, "#value:", 0.f, &selectable.disabled_factor, 1.f, 0.01f, 0.01f);
		nk_tree_pop(m_ctx);
	}
}
void NKBase::CustomComponentsEditor(ComponentSlider& tpi, struct nk_style_slider& slider, int id)
{
	int tree_index = 100;
	int tree_index_in = 200;
	int tree_index_in_in = 300;

	if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "background", NK_MINIMIZED, id + tree_index++)) {

		if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "normal", NK_MINIMIZED, id + tree_index_in++)) {
			ItemEditor(tpi.normal);
			nk_tree_pop(m_ctx);
		}
		if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "hover", NK_MINIMIZED, id + tree_index_in++)) {
			ItemEditor(tpi.hover);
			nk_tree_pop(m_ctx);
		}
		if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "active", NK_MINIMIZED, id + tree_index_in++)) {
			ItemEditor(tpi.active);
			nk_tree_pop(m_ctx);
		}
		nk_tree_pop(m_ctx);
	}

	if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "background(bar)", NK_MINIMIZED, id + tree_index++)) {

		if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "normal", NK_MINIMIZED, id + tree_index_in++)) {
			ColorPicker(slider.bar_normal);
			nk_tree_pop(m_ctx);
		}
		if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "hover", NK_MINIMIZED, id + tree_index_in++)) {
			ColorPicker(slider.bar_hover);
			nk_tree_pop(m_ctx);
		}
		if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "active", NK_MINIMIZED, id + tree_index_in++)) {
			ColorPicker(slider.bar_active);
			nk_tree_pop(m_ctx);
		}
		if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "filled", NK_MINIMIZED, id + tree_index_in++)) {
			ColorPicker(slider.bar_filled);
			nk_tree_pop(m_ctx);
		}
		nk_tree_pop(m_ctx);
	}

	if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "cursor", NK_MINIMIZED, id + tree_index++)) {

		if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "normal", NK_MINIMIZED, id + tree_index_in++)) {
			ItemEditor(tpi.cursor_normal);
			nk_tree_pop(m_ctx);
		}
		if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "hover", NK_MINIMIZED, id + tree_index_in++)) {
			ItemEditor(tpi.cursor_hover);
			nk_tree_pop(m_ctx);
		}
		if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "active", NK_MINIMIZED, id + tree_index_in++)) {
			ItemEditor(tpi.cursor_active);
			nk_tree_pop(m_ctx);
		}
		nk_tree_pop(m_ctx);
	}

	if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "properties", NK_MINIMIZED, id + tree_index++)) {
		nk_layout_row_dynamic(m_ctx, 22, 1);
		nk_label(m_ctx, "border", NK_TEXT_LEFT);
		nk_property_float(m_ctx, "#value:", 0.f, &slider.border, 100.f, 1.f, 0.1f);
		nk_label(m_ctx, "rounding", NK_TEXT_LEFT);
		nk_property_float(m_ctx, "#value:", 0.f, &slider.rounding, 100.f, 1.f, 0.1f);

		PropertyVector2("padding", slider.padding, 0.f, 1000.f, 1.f, 1.f);
		PropertyVector2("spacing", slider.spacing, 0.f, 1000.f, 1.f, 1.f);
		PropertyVector2("cursor_size", slider.cursor_size, 0.f, 1000.f, 1.f, 1.f);

		nk_layout_row_dynamic(m_ctx, 22, 1);
		nk_label(m_ctx, "color_factor", NK_TEXT_LEFT);
		nk_property_float(m_ctx, "#value:", 0.f, &slider.color_factor, 1.f, 0.01f, 0.01f);
		nk_label(m_ctx, "disabled_factor", NK_TEXT_LEFT);
		nk_property_float(m_ctx, "#value:", 0.f, &slider.disabled_factor, 1.f, 0.01f, 0.01f);
		nk_tree_pop(m_ctx);
	}


	if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "optional buttons", NK_MINIMIZED, id + tree_index++)) {

		nk_checkbox_label(m_ctx, "Show_buttons", &slider.show_buttons);

		if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "inc_button", NK_MINIMIZED, id + tree_index_in++)) {
			if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "symbol", NK_MINIMIZED, id + tree_index_in_in++)) {
				if (nk_option_label(m_ctx, "NONE",			slider.inc_symbol == NK_SYMBOL_NONE)) 			slider.inc_symbol = NK_SYMBOL_NONE;
				if (nk_option_label(m_ctx, "X",				slider.inc_symbol == NK_SYMBOL_X)) 				slider.inc_symbol = NK_SYMBOL_X;
				if (nk_option_label(m_ctx, "UNDERSCORE",	slider.inc_symbol == NK_SYMBOL_UNDERSCORE)) 	slider.inc_symbol = NK_SYMBOL_UNDERSCORE;
				if (nk_option_label(m_ctx, "CIRCLE_SOLID",	slider.inc_symbol == NK_SYMBOL_CIRCLE_SOLID)) 	slider.inc_symbol = NK_SYMBOL_CIRCLE_SOLID;
				if (nk_option_label(m_ctx, "CIRCLE_OUTLINE",slider.inc_symbol == NK_SYMBOL_CIRCLE_OUTLINE))	slider.inc_symbol = NK_SYMBOL_CIRCLE_OUTLINE;
				if (nk_option_label(m_ctx, "RECT_SOLID",	slider.inc_symbol == NK_SYMBOL_RECT_SOLID)) 	slider.inc_symbol = NK_SYMBOL_RECT_SOLID;
				if (nk_option_label(m_ctx, "RECT_OUTLINE",	slider.inc_symbol == NK_SYMBOL_RECT_OUTLINE)) 	slider.inc_symbol = NK_SYMBOL_RECT_OUTLINE;
				if (nk_option_label(m_ctx, "TRIANGLE_UP",	slider.inc_symbol == NK_SYMBOL_TRIANGLE_UP)) 	slider.inc_symbol = NK_SYMBOL_TRIANGLE_UP;
				if (nk_option_label(m_ctx, "TRIANGLE_DOWN",	slider.inc_symbol == NK_SYMBOL_TRIANGLE_DOWN)) 	slider.inc_symbol = NK_SYMBOL_TRIANGLE_DOWN;
				if (nk_option_label(m_ctx, "TRIANGLE_LEFT",	slider.inc_symbol == NK_SYMBOL_TRIANGLE_LEFT)) 	slider.inc_symbol = NK_SYMBOL_TRIANGLE_LEFT;
				if (nk_option_label(m_ctx, "TRIANGLE_RIGHT",slider.inc_symbol == NK_SYMBOL_TRIANGLE_RIGHT))	slider.inc_symbol = NK_SYMBOL_TRIANGLE_RIGHT;
				if (nk_option_label(m_ctx, "PLUS",			slider.inc_symbol == NK_SYMBOL_PLUS)) 			slider.inc_symbol = NK_SYMBOL_PLUS;
				if (nk_option_label(m_ctx, "MINUS",			slider.inc_symbol == NK_SYMBOL_MINUS)) 			slider.inc_symbol = NK_SYMBOL_MINUS;
				if (nk_option_label(m_ctx, "MAX",			slider.inc_symbol == NK_SYMBOL_MAX)) 			slider.inc_symbol = NK_SYMBOL_MAX;
				nk_tree_pop(m_ctx);
			}
			CustomComponentsEditor(tpi.inc_button, slider.inc_button, id + tree_index++);
			nk_tree_pop(m_ctx);
		}
		
		if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "dec_button", NK_MINIMIZED, id + tree_index_in++)) {
			if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "symbol", NK_MINIMIZED, id + tree_index_in_in++)) {
				if (nk_option_label(m_ctx, "NONE",			slider.dec_symbol == NK_SYMBOL_NONE)) 			slider.dec_symbol = NK_SYMBOL_NONE;
				if (nk_option_label(m_ctx, "X",				slider.dec_symbol == NK_SYMBOL_X)) 				slider.dec_symbol = NK_SYMBOL_X;
				if (nk_option_label(m_ctx, "UNDERSCORE",	slider.dec_symbol == NK_SYMBOL_UNDERSCORE)) 	slider.dec_symbol = NK_SYMBOL_UNDERSCORE;
				if (nk_option_label(m_ctx, "CIRCLE_SOLID",	slider.dec_symbol == NK_SYMBOL_CIRCLE_SOLID)) 	slider.dec_symbol = NK_SYMBOL_CIRCLE_SOLID;
				if (nk_option_label(m_ctx, "CIRCLE_OUTLINE",slider.dec_symbol == NK_SYMBOL_CIRCLE_OUTLINE))	slider.dec_symbol = NK_SYMBOL_CIRCLE_OUTLINE;
				if (nk_option_label(m_ctx, "RECT_SOLID",	slider.dec_symbol == NK_SYMBOL_RECT_SOLID)) 	slider.dec_symbol = NK_SYMBOL_RECT_SOLID;
				if (nk_option_label(m_ctx, "RECT_OUTLINE",	slider.dec_symbol == NK_SYMBOL_RECT_OUTLINE)) 	slider.dec_symbol = NK_SYMBOL_RECT_OUTLINE;
				if (nk_option_label(m_ctx, "TRIANGLE_UP",	slider.dec_symbol == NK_SYMBOL_TRIANGLE_UP)) 	slider.dec_symbol = NK_SYMBOL_TRIANGLE_UP;
				if (nk_option_label(m_ctx, "TRIANGLE_DOWN",	slider.dec_symbol == NK_SYMBOL_TRIANGLE_DOWN)) 	slider.dec_symbol = NK_SYMBOL_TRIANGLE_DOWN;
				if (nk_option_label(m_ctx, "TRIANGLE_LEFT",	slider.dec_symbol == NK_SYMBOL_TRIANGLE_LEFT)) 	slider.dec_symbol = NK_SYMBOL_TRIANGLE_LEFT;
				if (nk_option_label(m_ctx, "TRIANGLE_RIGHT",slider.dec_symbol == NK_SYMBOL_TRIANGLE_RIGHT))	slider.dec_symbol = NK_SYMBOL_TRIANGLE_RIGHT;
				if (nk_option_label(m_ctx, "PLUS",			slider.dec_symbol == NK_SYMBOL_PLUS)) 			slider.dec_symbol = NK_SYMBOL_PLUS;
				if (nk_option_label(m_ctx, "MINUS",			slider.dec_symbol == NK_SYMBOL_MINUS)) 			slider.dec_symbol = NK_SYMBOL_MINUS;
				if (nk_option_label(m_ctx, "MAX",			slider.dec_symbol == NK_SYMBOL_MAX)) 			slider.dec_symbol = NK_SYMBOL_MAX;
				nk_tree_pop(m_ctx);
			}
			CustomComponentsEditor(tpi.dec_button, slider.dec_button, id + tree_index_in_in++);
			nk_tree_pop(m_ctx);
		}
		nk_tree_pop(m_ctx);
	}
}
void NKBase::CustomComponentsEditor(ComponentProgress& tpi, struct nk_style_progress& progress, int id)
{
	int tree_index = 100;
	int tree_index_in = 200;
	int tree_index_in_in = 300;

	if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "background", NK_MINIMIZED, id + tree_index++)) {

		if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "normal", NK_MINIMIZED, id + tree_index_in++)) {
			ItemEditor(tpi.normal);
			nk_tree_pop(m_ctx);
		}
		if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "hover", NK_MINIMIZED, id + tree_index_in++)) {
			ItemEditor(tpi.hover);
			nk_tree_pop(m_ctx);
		}
		if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "active", NK_MINIMIZED, id + tree_index_in++)) {
			ItemEditor(tpi.active);
			nk_tree_pop(m_ctx);
		}
		if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "border_color", NK_MINIMIZED, id + tree_index_in++)) {
			ColorPicker(progress.border_color);
			nk_tree_pop(m_ctx);
		}
		nk_tree_pop(m_ctx);
	}

	if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "cursor", NK_MINIMIZED, id + tree_index++)) {

		if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "normal", NK_MINIMIZED, id + tree_index_in++)) {
			ItemEditor(tpi.cursor_normal);
			nk_tree_pop(m_ctx);
		}
		if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "hover", NK_MINIMIZED, id + tree_index_in++)) {
			ItemEditor(tpi.cursor_hover);
			nk_tree_pop(m_ctx);
		}
		if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "active", NK_MINIMIZED, id + tree_index_in++)) {
			ItemEditor(tpi.cursor_active);
			nk_tree_pop(m_ctx);
		}
		if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "border_color", NK_MINIMIZED, id + tree_index_in++)) {
			ColorPicker(progress.cursor_border_color);
			nk_tree_pop(m_ctx);
		}
		nk_tree_pop(m_ctx);
	}

	if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "properties", NK_MINIMIZED, id + tree_index++)) {
		nk_layout_row_dynamic(m_ctx, 22, 1);
		nk_label(m_ctx, "rounding", NK_TEXT_LEFT);
		nk_property_float(m_ctx, "#value:", 0.f, &progress.rounding, 100.f, 1.f, 0.1f);
		nk_label(m_ctx, "border", NK_TEXT_LEFT);
		nk_property_float(m_ctx, "#value:", 0.f, &progress.border, 100.f, 1.f, 0.1f);
		nk_label(m_ctx, "cursor_border", NK_TEXT_LEFT);
		nk_property_float(m_ctx, "#value:", 0.f, &progress.cursor_border, 100.f, 1.f, 0.1f);
		nk_label(m_ctx, "cursor_rounding", NK_TEXT_LEFT);
		nk_property_float(m_ctx, "#value:", 0.f, &progress.cursor_rounding, 100.f, 1.f, 0.1f);

		PropertyVector2("padding", progress.padding, 0.f, 1000.f, 1.f, 1.f);

		nk_layout_row_dynamic(m_ctx, 22, 1);
		nk_label(m_ctx, "color_factor", NK_TEXT_LEFT);
		nk_property_float(m_ctx, "#value:", 0.f, &progress.color_factor, 1.f, 0.01f, 0.01f);
		nk_label(m_ctx, "disabled_factor", NK_TEXT_LEFT);
		nk_property_float(m_ctx, "#value:", 0.f, &progress.disabled_factor, 1.f, 0.01f, 0.01f);
		nk_tree_pop(m_ctx);
	}
}
void NKBase::CustomComponentsEditor(ComponentProperty& tpi, struct nk_style_property& property, int id)
{
	int tree_index = 100;
	int tree_index_in = 200;
	int tree_index_in_in = 300;

	if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "background", NK_MINIMIZED, id + tree_index++)) {

		if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "normal", NK_MINIMIZED, id + tree_index_in++)) {
			ItemEditor(tpi.normal);
			nk_tree_pop(m_ctx);
		}
		if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "hover", NK_MINIMIZED, id + tree_index_in++)) {
			ItemEditor(tpi.hover);
			nk_tree_pop(m_ctx);
		}
		if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "active", NK_MINIMIZED, id + tree_index_in++)) {
			ItemEditor(tpi.active);
			nk_tree_pop(m_ctx);
		}
		if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "border_color", NK_MINIMIZED, id + tree_index_in++)) {
			ColorPicker(property.border_color);
			nk_tree_pop(m_ctx);
		}
		nk_tree_pop(m_ctx);
	}

	if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "label", NK_MINIMIZED, id + tree_index++)) {

		if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "normal", NK_MINIMIZED, id + tree_index_in++)) {
			ColorPicker(property.label_normal);
			nk_tree_pop(m_ctx);
		}
		if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "hover", NK_MINIMIZED, id + tree_index_in++)) {
			ColorPicker(property.label_hover);
			nk_tree_pop(m_ctx);
		}
		if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "active", NK_MINIMIZED, id + tree_index_in++)) {
			ColorPicker(property.label_active);
			nk_tree_pop(m_ctx);
		}
		nk_tree_pop(m_ctx);
	}

	if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "properties", NK_MINIMIZED, id + tree_index++)) {
		nk_layout_row_dynamic(m_ctx, 22, 1);
		nk_label(m_ctx, "rounding", NK_TEXT_LEFT);
		nk_property_float(m_ctx, "#value:", 0.f, &property.rounding, 100.f, 1.f, 0.1f);
		nk_label(m_ctx, "border", NK_TEXT_LEFT);
		nk_property_float(m_ctx, "#value:", 0.f, &property.border, 100.f, 1.f, 0.1f);
		PropertyVector2("padding", property.padding, 0.f, 1000.f, 1.f, 1.f);
		nk_layout_row_dynamic(m_ctx, 22, 1);
		nk_label(m_ctx, "color_factor", NK_TEXT_LEFT);
		nk_property_float(m_ctx, "#value:", 0.f, &property.color_factor, 1.f, 0.01f, 0.01f);
		nk_label(m_ctx, "disabled_factor", NK_TEXT_LEFT);
		nk_property_float(m_ctx, "#value:", 0.f, &property.disabled_factor, 1.f, 0.01f, 0.01f);
		nk_tree_pop(m_ctx);
	}


	if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "edit", NK_MINIMIZED, id + tree_index++)) {
		CustomComponentsEditor(tpi.edit, property.edit, id + tree_index_in_in+100);
		CustomComponentsEditor(tpi.inc_button, property.inc_button, id + tree_index_in_in+200);
		CustomComponentsEditor(tpi.dec_button, property.dec_button, id + tree_index_in_in+300);
		nk_tree_pop(m_ctx);
	}
}
void NKBase::CustomComponentsEditor(ComponentEdit& tpi, struct nk_style_edit& edit, int id)
{
	int tree_index = 100;
	int tree_index_in = 200;
	int tree_index_in_in = 300;

	if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "background", NK_MINIMIZED, id + tree_index++)) {

		if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "normal", NK_MINIMIZED, id + tree_index_in++)) {
			ItemEditor(tpi.normal);
			nk_tree_pop(m_ctx);
		}
		if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "hover", NK_MINIMIZED, id + tree_index_in++)) {
			ItemEditor(tpi.hover);
			nk_tree_pop(m_ctx);
		}
		if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "active", NK_MINIMIZED, id + tree_index_in++)) {
			ItemEditor(tpi.active);
			nk_tree_pop(m_ctx);
		}
		if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "border_color", NK_MINIMIZED, id + tree_index_in++)) {
			ColorPicker(edit.border_color);
			nk_tree_pop(m_ctx);
		}
		CustomComponentsEditor(tpi.scrollbar, edit.scrollbar, id + tree_index_in + 100);
		nk_tree_pop(m_ctx);
	}

	if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "cursor", NK_MINIMIZED, id + tree_index++)) {

		if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "normal", NK_MINIMIZED, id + tree_index_in++)) {
			ColorPicker(edit.cursor_normal);
			nk_tree_pop(m_ctx);
		}
		if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "hover", NK_MINIMIZED, id + tree_index_in++)) {
			ColorPicker(edit.cursor_hover);
			nk_tree_pop(m_ctx);
		}
		if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "text_normal", NK_MINIMIZED, id + tree_index_in++)) {
			ColorPicker(edit.cursor_text_normal);
			nk_tree_pop(m_ctx);
		}
		if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "text_hover", NK_MINIMIZED, id + tree_index_in++)) {
			ColorPicker(edit.cursor_text_hover);
			nk_tree_pop(m_ctx);
		}
		nk_tree_pop(m_ctx);
	}

	if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "text(unselected)", NK_MINIMIZED, id + tree_index++)) {

		if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "normal", NK_MINIMIZED, id + tree_index_in++)) {
			ColorPicker(edit.text_normal);
			nk_tree_pop(m_ctx);
		}
		if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "hover", NK_MINIMIZED, id + tree_index_in++)) {
			ColorPicker(edit.text_hover);
			nk_tree_pop(m_ctx);
		}
		if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "active", NK_MINIMIZED, id + tree_index_in++)) {
			ColorPicker(edit.text_active);
			nk_tree_pop(m_ctx);
		}
		nk_tree_pop(m_ctx);
	}

	if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "text(selected)", NK_MINIMIZED, id + tree_index++)) {

		if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "normal", NK_MINIMIZED, id + tree_index_in++)) {
			ColorPicker(edit.selected_normal);
			nk_tree_pop(m_ctx);
		}
		if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "hover", NK_MINIMIZED, id + tree_index_in++)) {
			ColorPicker(edit.selected_hover);
			nk_tree_pop(m_ctx);
		}
		if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "text_normal", NK_MINIMIZED, id + tree_index_in++)) {
			ColorPicker(edit.selected_text_normal);
			nk_tree_pop(m_ctx);
		}
		if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "text_hover", NK_MINIMIZED, id + tree_index_in++)) {
			ColorPicker(edit.selected_text_hover);
			nk_tree_pop(m_ctx);
		}
		nk_tree_pop(m_ctx);
	}

	if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "properties", NK_MINIMIZED, id + tree_index++)) {
		nk_layout_row_dynamic(m_ctx, 22, 1);
		nk_label(m_ctx, "rounding", NK_TEXT_LEFT);
		nk_property_float(m_ctx, "#value:", 0.f, &edit.rounding, 100.f, 1.f, 0.1f);
		nk_label(m_ctx, "border", NK_TEXT_LEFT);
		nk_property_float(m_ctx, "#value:", 0.f, &edit.border, 100.f, 1.f, 0.1f);
		nk_label(m_ctx, "cursor_size", NK_TEXT_LEFT);
		nk_property_float(m_ctx, "#value:", 0.f, &edit.cursor_size, 100.f, 1.f, 0.1f);

		PropertyVector2("scrollbar_size", edit.scrollbar_size, 0.f, 1000.f, 1.f, 1.f);
		PropertyVector2("padding", edit.padding, 0.f, 1000.f, 1.f, 1.f);

		nk_layout_row_dynamic(m_ctx, 22, 1);
		nk_label(m_ctx, "row_padding", NK_TEXT_LEFT);
		nk_property_float(m_ctx, "#value:", 0.f, &edit.row_padding, 100.f, 1.f, 0.1f);
		nk_label(m_ctx, "color_factor", NK_TEXT_LEFT);
		nk_property_float(m_ctx, "#value:", 0.f, &edit.color_factor, 1.f, 0.01f, 0.01f);
		nk_label(m_ctx, "disabled_factor", NK_TEXT_LEFT);
		nk_property_float(m_ctx, "#value:", 0.f, &edit.disabled_factor, 1.f, 0.01f, 0.01f);
		nk_tree_pop(m_ctx);
	}
}
void NKBase::CustomComponentsEditor(NKStyleItem& nsi, nk_style_chart& chart, int id)
{

	int tree_index = 100;
	int tree_index_in = 200;
	int tree_index_in_in = 300;

	if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "colors", NK_MINIMIZED, id + tree_index++)) {

		if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "background", NK_MINIMIZED, id + tree_index_in++)) {
			ItemEditor(nsi);
			nk_tree_pop(m_ctx);
		}

		if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "border", NK_MINIMIZED, id + tree_index_in++)) {
			ColorPicker(chart.border_color);
			nk_tree_pop(m_ctx);
		}
		if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "selected", NK_MINIMIZED, id + tree_index_in++)) {
			ColorPicker(chart.selected_color);
			nk_tree_pop(m_ctx);
		}
		if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "color", NK_MINIMIZED, id + tree_index_in++)) {
			ColorPicker(chart.color);
			nk_tree_pop(m_ctx);
		}
		nk_tree_pop(m_ctx);
	}

	if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "properties", NK_MINIMIZED, id + tree_index++)) {
		nk_layout_row_dynamic(m_ctx, 22, 1);
		nk_label(m_ctx, "rounding", NK_TEXT_LEFT);
		nk_property_float(m_ctx, "#value:", 0.f, &chart.rounding, 100.f, 1.f, 0.1f);
		nk_label(m_ctx, "border", NK_TEXT_LEFT);
		nk_property_float(m_ctx, "#value:", 0.f, &chart.border, 100.f, 1.f, 0.1f);

		PropertyVector2("padding", chart.padding, 0.f, 1000.f, 1.f, 1.f);

		nk_layout_row_dynamic(m_ctx, 22, 1);
		nk_label(m_ctx, "color_factor", NK_TEXT_LEFT);
		nk_property_float(m_ctx, "#value:", 0.f, &chart.color_factor, 1.f, 0.01f, 0.01f);
		nk_label(m_ctx, "disabled_factor", NK_TEXT_LEFT);
		nk_property_float(m_ctx, "#value:", 0.f, &chart.disabled_factor, 1.f, 0.01f, 0.01f);

		nk_checkbox_label(m_ctx, "show_markers", &chart.show_markers);

		nk_tree_pop(m_ctx);
	}
}
void NKBase::CustomComponentsEditor(ComponentScrollbar& tpi, struct nk_style_scrollbar& scrollbar, int id)
{
	int tree_index = 100;
	int tree_index_in = 200;
	int tree_index_in_in = 300;

	if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "background", NK_MINIMIZED, id + tree_index++)) {

		if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "normal", NK_MINIMIZED, id + tree_index_in++)) {
			ItemEditor(tpi.normal);
			nk_tree_pop(m_ctx);
		}
		if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "hover", NK_MINIMIZED, id + tree_index_in++)) {
			ItemEditor(tpi.hover);
			nk_tree_pop(m_ctx);
		}
		if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "active", NK_MINIMIZED, id + tree_index_in++)) {
			ItemEditor(tpi.active);
			nk_tree_pop(m_ctx);
		}
		if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "border_color", NK_MINIMIZED, id + tree_index_in++)) {
			ColorPicker(scrollbar.border_color);
			nk_tree_pop(m_ctx);
		}
		nk_tree_pop(m_ctx);
	}

	if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "cursor", NK_MINIMIZED, id + tree_index++)) {

		if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "normal", NK_MINIMIZED, id + tree_index_in++)) {
			ItemEditor(tpi.cursor_normal);
			nk_tree_pop(m_ctx);
		}
		if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "hover", NK_MINIMIZED, id + tree_index_in++)) {
			ItemEditor(tpi.cursor_hover);
			nk_tree_pop(m_ctx);
		}
		if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "active", NK_MINIMIZED, id + tree_index_in++)) {
			ItemEditor(tpi.cursor_active);
			nk_tree_pop(m_ctx);
		}
		if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "border_color", NK_MINIMIZED, id + tree_index_in++)) {
			ColorPicker(scrollbar.cursor_border_color);
			nk_tree_pop(m_ctx);
		}
		nk_tree_pop(m_ctx);
	}

	if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "properties", NK_MINIMIZED, id + tree_index++)) {
		nk_layout_row_dynamic(m_ctx, 22, 1);
		nk_label(m_ctx, "rounding", NK_TEXT_LEFT);
		nk_property_float(m_ctx, "#value:", 0.f, &scrollbar.rounding, 100.f, 1.f, 0.1f);
		nk_label(m_ctx, "border", NK_TEXT_LEFT);
		nk_property_float(m_ctx, "#value:", 0.f, &scrollbar.border, 100.f, 1.f, 0.1f);
		nk_label(m_ctx, "border_cursor", NK_TEXT_LEFT);
		nk_property_float(m_ctx, "#value:", 0.f, &scrollbar.border_cursor, 100.f, 1.f, 0.1f);
		nk_label(m_ctx, "rounding_cursor", NK_TEXT_LEFT);
		nk_property_float(m_ctx, "#value:", 0.f, &scrollbar.rounding_cursor, 100.f, 1.f, 0.1f);

		PropertyVector2("padding", scrollbar.padding, 0.f, 1000.f, 1.f, 1.f);

		nk_layout_row_dynamic(m_ctx, 22, 1);
		nk_label(m_ctx, "color_factor", NK_TEXT_LEFT);
		nk_property_float(m_ctx, "#value:", 0.f, &scrollbar.color_factor, 1.f, 0.01f, 0.01f);
		nk_label(m_ctx, "disabled_factor", NK_TEXT_LEFT);
		nk_property_float(m_ctx, "#value:", 0.f, &scrollbar.disabled_factor, 1.f, 0.01f, 0.01f);
		nk_tree_pop(m_ctx);
	}
	if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "optional buttons", NK_MINIMIZED, id + tree_index++)) {

		nk_checkbox_label(m_ctx, "Show_buttons", &scrollbar.show_buttons);

		if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "inc_button", NK_MINIMIZED, id + tree_index_in++)) {
			if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "symbol", NK_MINIMIZED, id + tree_index_in_in++)) {
				if (nk_option_label(m_ctx, "NONE",				scrollbar.inc_symbol == NK_SYMBOL_NONE)) 			scrollbar.inc_symbol = NK_SYMBOL_NONE;
				if (nk_option_label(m_ctx, "X",					scrollbar.inc_symbol == NK_SYMBOL_X)) 				scrollbar.inc_symbol = NK_SYMBOL_X;
				if (nk_option_label(m_ctx, "UNDERSCORE",		scrollbar.inc_symbol == NK_SYMBOL_UNDERSCORE)) 		scrollbar.inc_symbol = NK_SYMBOL_UNDERSCORE;
				if (nk_option_label(m_ctx, "CIRCLE_SOLID",		scrollbar.inc_symbol == NK_SYMBOL_CIRCLE_SOLID)) 	scrollbar.inc_symbol = NK_SYMBOL_CIRCLE_SOLID;
				if (nk_option_label(m_ctx, "CIRCLE_OUTLINE",	scrollbar.inc_symbol == NK_SYMBOL_CIRCLE_OUTLINE))	scrollbar.inc_symbol = NK_SYMBOL_CIRCLE_OUTLINE;
				if (nk_option_label(m_ctx, "RECT_SOLID",		scrollbar.inc_symbol == NK_SYMBOL_RECT_SOLID)) 		scrollbar.inc_symbol = NK_SYMBOL_RECT_SOLID;
				if (nk_option_label(m_ctx, "RECT_OUTLINE",		scrollbar.inc_symbol == NK_SYMBOL_RECT_OUTLINE)) 	scrollbar.inc_symbol = NK_SYMBOL_RECT_OUTLINE;
				if (nk_option_label(m_ctx, "TRIANGLE_UP",		scrollbar.inc_symbol == NK_SYMBOL_TRIANGLE_UP)) 	scrollbar.inc_symbol = NK_SYMBOL_TRIANGLE_UP;
				if (nk_option_label(m_ctx, "TRIANGLE_DOWN",		scrollbar.inc_symbol == NK_SYMBOL_TRIANGLE_DOWN)) 	scrollbar.inc_symbol = NK_SYMBOL_TRIANGLE_DOWN;
				if (nk_option_label(m_ctx, "TRIANGLE_LEFT",		scrollbar.inc_symbol == NK_SYMBOL_TRIANGLE_LEFT)) 	scrollbar.inc_symbol = NK_SYMBOL_TRIANGLE_LEFT;
				if (nk_option_label(m_ctx, "TRIANGLE_RIGHT",	scrollbar.inc_symbol == NK_SYMBOL_TRIANGLE_RIGHT))	scrollbar.inc_symbol = NK_SYMBOL_TRIANGLE_RIGHT;
				if (nk_option_label(m_ctx, "PLUS",				scrollbar.inc_symbol == NK_SYMBOL_PLUS)) 			scrollbar.inc_symbol = NK_SYMBOL_PLUS;
				if (nk_option_label(m_ctx, "MINUS",				scrollbar.inc_symbol == NK_SYMBOL_MINUS)) 			scrollbar.inc_symbol = NK_SYMBOL_MINUS;
				if (nk_option_label(m_ctx, "MAX",				scrollbar.inc_symbol == NK_SYMBOL_MAX)) 			scrollbar.inc_symbol = NK_SYMBOL_MAX;
				nk_tree_pop(m_ctx);
			}
			CustomComponentsEditor(tpi.inc_button, scrollbar.inc_button, id + tree_index++);
			nk_tree_pop(m_ctx);
		}
		
		if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "dec_button", NK_MINIMIZED, id + tree_index_in++)) {
			if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "symbol", NK_MINIMIZED, id + tree_index_in_in++)) {
				if (nk_option_label(m_ctx, "NONE",			scrollbar.dec_symbol == NK_SYMBOL_NONE)) 			scrollbar.dec_symbol = NK_SYMBOL_NONE;
				if (nk_option_label(m_ctx, "X",				scrollbar.dec_symbol == NK_SYMBOL_X)) 				scrollbar.dec_symbol = NK_SYMBOL_X;
				if (nk_option_label(m_ctx, "UNDERSCORE",	scrollbar.dec_symbol == NK_SYMBOL_UNDERSCORE)) 		scrollbar.dec_symbol = NK_SYMBOL_UNDERSCORE;
				if (nk_option_label(m_ctx, "CIRCLE_SOLID",	scrollbar.dec_symbol == NK_SYMBOL_CIRCLE_SOLID)) 	scrollbar.dec_symbol = NK_SYMBOL_CIRCLE_SOLID;
				if (nk_option_label(m_ctx, "CIRCLE_OUTLINE",scrollbar.dec_symbol == NK_SYMBOL_CIRCLE_OUTLINE))	scrollbar.dec_symbol = NK_SYMBOL_CIRCLE_OUTLINE;
				if (nk_option_label(m_ctx, "RECT_SOLID",	scrollbar.dec_symbol == NK_SYMBOL_RECT_SOLID)) 		scrollbar.dec_symbol = NK_SYMBOL_RECT_SOLID;
				if (nk_option_label(m_ctx, "RECT_OUTLINE",	scrollbar.dec_symbol == NK_SYMBOL_RECT_OUTLINE)) 	scrollbar.dec_symbol = NK_SYMBOL_RECT_OUTLINE;
				if (nk_option_label(m_ctx, "TRIANGLE_UP",	scrollbar.dec_symbol == NK_SYMBOL_TRIANGLE_UP)) 	scrollbar.dec_symbol = NK_SYMBOL_TRIANGLE_UP;
				if (nk_option_label(m_ctx, "TRIANGLE_DOWN",	scrollbar.dec_symbol == NK_SYMBOL_TRIANGLE_DOWN)) 	scrollbar.dec_symbol = NK_SYMBOL_TRIANGLE_DOWN;
				if (nk_option_label(m_ctx, "TRIANGLE_LEFT",	scrollbar.dec_symbol == NK_SYMBOL_TRIANGLE_LEFT)) 	scrollbar.dec_symbol = NK_SYMBOL_TRIANGLE_LEFT;
				if (nk_option_label(m_ctx, "TRIANGLE_RIGHT",scrollbar.dec_symbol == NK_SYMBOL_TRIANGLE_RIGHT))	scrollbar.dec_symbol = NK_SYMBOL_TRIANGLE_RIGHT;
				if (nk_option_label(m_ctx, "PLUS",			scrollbar.dec_symbol == NK_SYMBOL_PLUS)) 			scrollbar.dec_symbol = NK_SYMBOL_PLUS;
				if (nk_option_label(m_ctx, "MINUS",			scrollbar.dec_symbol == NK_SYMBOL_MINUS)) 			scrollbar.dec_symbol = NK_SYMBOL_MINUS;
				if (nk_option_label(m_ctx, "MAX",			scrollbar.dec_symbol == NK_SYMBOL_MAX)) 			scrollbar.dec_symbol = NK_SYMBOL_MAX;
				nk_tree_pop(m_ctx);
			}
			CustomComponentsEditor(tpi.dec_button, scrollbar.dec_button, id + tree_index_in_in++);
			nk_tree_pop(m_ctx);
		}
		nk_tree_pop(m_ctx);
	}
}
void NKBase::CustomComponentsEditor(ComponentTab& tpi, struct nk_style_tab& tab, int id)
{
	int tree_index = 100;
	int tree_index_in = 200;
	int tree_index_in_in = 300;

	if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "bg_color", NK_MINIMIZED, id + tree_index++)) {

		if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "background", NK_MINIMIZED, id + tree_index_in++)) {
			ItemEditor(tpi.background);
			nk_tree_pop(m_ctx);
		}
		if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "border", NK_MINIMIZED, id + tree_index_in++)) {
			ColorPicker(tab.border_color);
			nk_tree_pop(m_ctx);
		}
		if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "text", NK_MINIMIZED, id + tree_index_in++)) {
			ColorPicker(tab.text);
			nk_tree_pop(m_ctx);
		}
		nk_tree_pop(m_ctx);
	}
	if (nk_tree_push(m_ctx, NK_TREE_NODE, "button", NK_MINIMIZED)) {
		if (nk_tree_push(m_ctx, NK_TREE_NODE, "sym_minimize", NK_MINIMIZED)) {
			if (nk_option_label(m_ctx, "NONE",				tab.sym_minimize == NK_SYMBOL_NONE)) 				tab.sym_minimize = NK_SYMBOL_NONE;
			if (nk_option_label(m_ctx, "X",					tab.sym_minimize == NK_SYMBOL_X)) 					tab.sym_minimize = NK_SYMBOL_X;
			if (nk_option_label(m_ctx, "UNDERSCORE",		tab.sym_minimize == NK_SYMBOL_UNDERSCORE)) 			tab.sym_minimize = NK_SYMBOL_UNDERSCORE;
			if (nk_option_label(m_ctx, "CIRCLE_SOLID",		tab.sym_minimize == NK_SYMBOL_CIRCLE_SOLID)) 		tab.sym_minimize = NK_SYMBOL_CIRCLE_SOLID;
			if (nk_option_label(m_ctx, "CIRCLE_OUTLINE",	tab.sym_minimize == NK_SYMBOL_CIRCLE_OUTLINE))		tab.sym_minimize = NK_SYMBOL_CIRCLE_OUTLINE;
			if (nk_option_label(m_ctx, "RECT_SOLID",		tab.sym_minimize == NK_SYMBOL_RECT_SOLID)) 			tab.sym_minimize = NK_SYMBOL_RECT_SOLID;
			if (nk_option_label(m_ctx, "RECT_OUTLINE",		tab.sym_minimize == NK_SYMBOL_RECT_OUTLINE)) 		tab.sym_minimize = NK_SYMBOL_RECT_OUTLINE;
			if (nk_option_label(m_ctx, "TRIANGLE_UP",		tab.sym_minimize == NK_SYMBOL_TRIANGLE_UP)) 		tab.sym_minimize = NK_SYMBOL_TRIANGLE_UP;
			if (nk_option_label(m_ctx, "TRIANGLE_DOWN",		tab.sym_minimize == NK_SYMBOL_TRIANGLE_DOWN))		tab.sym_minimize = NK_SYMBOL_TRIANGLE_DOWN;
			if (nk_option_label(m_ctx, "TRIANGLE_LEFT",		tab.sym_minimize == NK_SYMBOL_TRIANGLE_LEFT))		tab.sym_minimize = NK_SYMBOL_TRIANGLE_LEFT;
			if (nk_option_label(m_ctx, "TRIANGLE_RIGHT",	tab.sym_minimize == NK_SYMBOL_TRIANGLE_RIGHT))		tab.sym_minimize = NK_SYMBOL_TRIANGLE_RIGHT;
			if (nk_option_label(m_ctx, "PLUS",				tab.sym_minimize == NK_SYMBOL_PLUS)) 				tab.sym_minimize = NK_SYMBOL_PLUS;
			if (nk_option_label(m_ctx, "MINUS",				tab.sym_minimize == NK_SYMBOL_MINUS)) 				tab.sym_minimize = NK_SYMBOL_MINUS;
			if (nk_option_label(m_ctx, "MAX",				tab.sym_minimize == NK_SYMBOL_MAX)) 				tab.sym_minimize = NK_SYMBOL_MAX;
			nk_tree_pop(m_ctx);
		}
		CustomComponentsEditor(tpi.tab_minimize_button, tab.tab_minimize_button, id + tree_index_in_in + 100);
		CustomComponentsEditor(tpi.node_minimize_button, tab.node_minimize_button, id + tree_index_in_in + 200);


		if (nk_tree_push(m_ctx, NK_TREE_NODE, "sym_maximize", NK_MINIMIZED)) {
			if (nk_option_label(m_ctx, "NONE",				tab.sym_maximize == NK_SYMBOL_NONE)) 				tab.sym_maximize = NK_SYMBOL_NONE;
			if (nk_option_label(m_ctx, "X",					tab.sym_maximize == NK_SYMBOL_X)) 					tab.sym_maximize = NK_SYMBOL_X;
			if (nk_option_label(m_ctx, "UNDERSCORE",		tab.sym_maximize == NK_SYMBOL_UNDERSCORE)) 			tab.sym_maximize = NK_SYMBOL_UNDERSCORE;
			if (nk_option_label(m_ctx, "CIRCLE_SOLID",		tab.sym_maximize == NK_SYMBOL_CIRCLE_SOLID)) 		tab.sym_maximize = NK_SYMBOL_CIRCLE_SOLID;
			if (nk_option_label(m_ctx, "CIRCLE_OUTLINE",	tab.sym_maximize == NK_SYMBOL_CIRCLE_OUTLINE))		tab.sym_maximize = NK_SYMBOL_CIRCLE_OUTLINE;
			if (nk_option_label(m_ctx, "RECT_SOLID",		tab.sym_maximize == NK_SYMBOL_RECT_SOLID)) 			tab.sym_maximize = NK_SYMBOL_RECT_SOLID;
			if (nk_option_label(m_ctx, "RECT_OUTLINE",		tab.sym_maximize == NK_SYMBOL_RECT_OUTLINE)) 		tab.sym_maximize = NK_SYMBOL_RECT_OUTLINE;
			if (nk_option_label(m_ctx, "TRIANGLE_UP",		tab.sym_maximize == NK_SYMBOL_TRIANGLE_UP)) 		tab.sym_maximize = NK_SYMBOL_TRIANGLE_UP;
			if (nk_option_label(m_ctx, "TRIANGLE_DOWN",		tab.sym_maximize == NK_SYMBOL_TRIANGLE_DOWN))		tab.sym_maximize = NK_SYMBOL_TRIANGLE_DOWN;
			if (nk_option_label(m_ctx, "TRIANGLE_LEFT",		tab.sym_maximize == NK_SYMBOL_TRIANGLE_LEFT))		tab.sym_maximize = NK_SYMBOL_TRIANGLE_LEFT;
			if (nk_option_label(m_ctx, "TRIANGLE_RIGHT",	tab.sym_maximize == NK_SYMBOL_TRIANGLE_RIGHT))		tab.sym_maximize = NK_SYMBOL_TRIANGLE_RIGHT;
			if (nk_option_label(m_ctx, "PLUS",				tab.sym_maximize == NK_SYMBOL_PLUS)) 				tab.sym_maximize = NK_SYMBOL_PLUS;
			if (nk_option_label(m_ctx, "MINUS",				tab.sym_maximize == NK_SYMBOL_MINUS)) 				tab.sym_maximize = NK_SYMBOL_MINUS;
			if (nk_option_label(m_ctx, "MAX",				tab.sym_maximize == NK_SYMBOL_MAX)) 				tab.sym_maximize = NK_SYMBOL_MAX;
			nk_tree_pop(m_ctx);
		}
		CustomComponentsEditor(tpi.node_maximize_button, tab.node_maximize_button, id + tree_index_in_in + 300);
		CustomComponentsEditor(tpi.node_minimize_button, tab.node_minimize_button, id + tree_index_in_in + 400);
		nk_tree_pop(m_ctx);
	}

	if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "properties", NK_MINIMIZED, id + tree_index++)) {
		nk_layout_row_dynamic(m_ctx, 22, 1);
		nk_label(m_ctx, "rounding", NK_TEXT_LEFT);
		nk_property_float(m_ctx, "#value:", 0.f, &tab.rounding, 100.f, 1.f, 0.1f);
		nk_label(m_ctx, "border", NK_TEXT_LEFT);
		nk_property_float(m_ctx, "#value:", 0.f, &tab.border, 100.f, 1.f, 0.1f);
		nk_label(m_ctx, "indent", NK_TEXT_LEFT);
		nk_property_float(m_ctx, "#value:", 0.f, &tab.indent, 100.f, 1.f, 0.1f);

		PropertyVector2("padding", tab.padding, 0.f, 1000.f, 1.f, 1.f);
		PropertyVector2("spacing", tab.spacing, 0.f, 1000.f, 1.f, 1.f);

		nk_layout_row_dynamic(m_ctx, 22, 1);
		nk_label(m_ctx, "color_factor", NK_TEXT_LEFT);
		nk_property_float(m_ctx, "#value:", 0.f, &tab.color_factor, 1.f, 0.01f, 0.01f);
		nk_label(m_ctx, "disabled_factor", NK_TEXT_LEFT);
		nk_property_float(m_ctx, "#value:", 0.f, &tab.disabled_factor, 1.f, 0.01f, 0.01f);

		nk_tree_pop(m_ctx);
	}
}
void NKBase::CustomComponentsEditor(ComponentCombo& tpi, struct nk_style_combo& combo, int id)
{
	int tree_index = 100;
	int tree_index_in = 200;
	int tree_index_in_in = 300;

	if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "background", NK_MINIMIZED, id + tree_index++)) {

		if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "normal", NK_MINIMIZED, id + tree_index_in++)) {
			ItemEditor(tpi.normal);
			nk_tree_pop(m_ctx);
		}
		if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "hover", NK_MINIMIZED, id + tree_index_in++)) {
			ItemEditor(tpi.hover);
			nk_tree_pop(m_ctx);
		}
		if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "active", NK_MINIMIZED, id + tree_index_in++)) {
			ItemEditor(tpi.active);
			nk_tree_pop(m_ctx);
		}
		if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "border", NK_MINIMIZED, id + tree_index_in++)) {
			ColorPicker(combo.border_color);
			nk_tree_pop(m_ctx);
		}
		nk_tree_pop(m_ctx);
	}

	if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "label", NK_MINIMIZED, id + tree_index++)) {
		if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "normal", NK_MINIMIZED, id + tree_index_in++)) {
			ColorPicker(combo.label_normal);
			nk_tree_pop(m_ctx);
		}
		if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "hover", NK_MINIMIZED, id + tree_index_in++)) {
			ColorPicker(combo.label_hover);
			nk_tree_pop(m_ctx);
		}
		if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "active", NK_MINIMIZED, id + tree_index_in++)) {
			ColorPicker(combo.label_active);
			nk_tree_pop(m_ctx);
		}
		nk_tree_pop(m_ctx);
	}

	if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "symbol", NK_MINIMIZED, id + tree_index++)) {
		if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "normal", NK_MINIMIZED, id + tree_index_in++)) {
			ColorPicker(combo.symbol_normal);
			nk_tree_pop(m_ctx);
		}
		if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "hover", NK_MINIMIZED, id + tree_index_in++)) {
			ColorPicker(combo.symbol_hover);
			nk_tree_pop(m_ctx);
		}
		if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "active", NK_MINIMIZED, id + tree_index_in++)) {
			ColorPicker(combo.symbol_active);
			nk_tree_pop(m_ctx);
		}
		nk_tree_pop(m_ctx);
	}

	if (nk_tree_push(m_ctx, NK_TREE_NODE, "button", NK_MINIMIZED)) {

		CustomComponentsEditor(tpi.button, combo.button, id + tree_index_in_in + 100);

		if (nk_tree_push(m_ctx, NK_TREE_NODE, "sym_normal", NK_MINIMIZED)) {
			if (nk_option_label(m_ctx, "NONE",				combo.sym_normal == NK_SYMBOL_NONE)) 				combo.sym_normal = NK_SYMBOL_NONE;
			if (nk_option_label(m_ctx, "X",					combo.sym_normal == NK_SYMBOL_X)) 					combo.sym_normal = NK_SYMBOL_X;
			if (nk_option_label(m_ctx, "UNDERSCORE",		combo.sym_normal == NK_SYMBOL_UNDERSCORE)) 			combo.sym_normal = NK_SYMBOL_UNDERSCORE;
			if (nk_option_label(m_ctx, "CIRCLE_SOLID",		combo.sym_normal == NK_SYMBOL_CIRCLE_SOLID)) 		combo.sym_normal = NK_SYMBOL_CIRCLE_SOLID;
			if (nk_option_label(m_ctx, "CIRCLE_OUTLINE",	combo.sym_normal == NK_SYMBOL_CIRCLE_OUTLINE))		combo.sym_normal = NK_SYMBOL_CIRCLE_OUTLINE;
			if (nk_option_label(m_ctx, "RECT_SOLID",		combo.sym_normal == NK_SYMBOL_RECT_SOLID)) 			combo.sym_normal = NK_SYMBOL_RECT_SOLID;
			if (nk_option_label(m_ctx, "RECT_OUTLINE",		combo.sym_normal == NK_SYMBOL_RECT_OUTLINE)) 		combo.sym_normal = NK_SYMBOL_RECT_OUTLINE;
			if (nk_option_label(m_ctx, "TRIANGLE_UP",		combo.sym_normal == NK_SYMBOL_TRIANGLE_UP)) 		combo.sym_normal = NK_SYMBOL_TRIANGLE_UP;
			if (nk_option_label(m_ctx, "TRIANGLE_DOWN",		combo.sym_normal == NK_SYMBOL_TRIANGLE_DOWN))		combo.sym_normal = NK_SYMBOL_TRIANGLE_DOWN;
			if (nk_option_label(m_ctx, "TRIANGLE_LEFT",		combo.sym_normal == NK_SYMBOL_TRIANGLE_LEFT))		combo.sym_normal = NK_SYMBOL_TRIANGLE_LEFT;
			if (nk_option_label(m_ctx, "TRIANGLE_RIGHT",	combo.sym_normal == NK_SYMBOL_TRIANGLE_RIGHT))		combo.sym_normal = NK_SYMBOL_TRIANGLE_RIGHT;
			if (nk_option_label(m_ctx, "PLUS",				combo.sym_normal == NK_SYMBOL_PLUS)) 				combo.sym_normal = NK_SYMBOL_PLUS;
			if (nk_option_label(m_ctx, "MINUS",				combo.sym_normal == NK_SYMBOL_MINUS)) 				combo.sym_normal = NK_SYMBOL_MINUS;
			if (nk_option_label(m_ctx, "MAX",				combo.sym_normal == NK_SYMBOL_MAX)) 				combo.sym_normal = NK_SYMBOL_MAX;
			nk_tree_pop(m_ctx);
		}
		if (nk_tree_push(m_ctx, NK_TREE_NODE, "sym_hover", NK_MINIMIZED)) {
			if (nk_option_label(m_ctx, "NONE",				combo.sym_hover == NK_SYMBOL_NONE)) 				combo.sym_hover = NK_SYMBOL_NONE;
			if (nk_option_label(m_ctx, "X",					combo.sym_hover == NK_SYMBOL_X)) 					combo.sym_hover = NK_SYMBOL_X;
			if (nk_option_label(m_ctx, "UNDERSCORE",		combo.sym_hover == NK_SYMBOL_UNDERSCORE)) 			combo.sym_hover = NK_SYMBOL_UNDERSCORE;
			if (nk_option_label(m_ctx, "CIRCLE_SOLID",		combo.sym_hover == NK_SYMBOL_CIRCLE_SOLID)) 		combo.sym_hover = NK_SYMBOL_CIRCLE_SOLID;
			if (nk_option_label(m_ctx, "CIRCLE_OUTLINE",	combo.sym_hover == NK_SYMBOL_CIRCLE_OUTLINE))		combo.sym_hover = NK_SYMBOL_CIRCLE_OUTLINE;
			if (nk_option_label(m_ctx, "RECT_SOLID",		combo.sym_hover == NK_SYMBOL_RECT_SOLID)) 			combo.sym_hover = NK_SYMBOL_RECT_SOLID;
			if (nk_option_label(m_ctx, "RECT_OUTLINE",		combo.sym_hover == NK_SYMBOL_RECT_OUTLINE)) 		combo.sym_hover = NK_SYMBOL_RECT_OUTLINE;
			if (nk_option_label(m_ctx, "TRIANGLE_UP",		combo.sym_hover == NK_SYMBOL_TRIANGLE_UP)) 			combo.sym_hover = NK_SYMBOL_TRIANGLE_UP;
			if (nk_option_label(m_ctx, "TRIANGLE_DOWN",		combo.sym_hover == NK_SYMBOL_TRIANGLE_DOWN))		combo.sym_hover = NK_SYMBOL_TRIANGLE_DOWN;
			if (nk_option_label(m_ctx, "TRIANGLE_LEFT",		combo.sym_hover == NK_SYMBOL_TRIANGLE_LEFT))		combo.sym_hover = NK_SYMBOL_TRIANGLE_LEFT;
			if (nk_option_label(m_ctx, "TRIANGLE_RIGHT",	combo.sym_hover == NK_SYMBOL_TRIANGLE_RIGHT))		combo.sym_hover = NK_SYMBOL_TRIANGLE_RIGHT;
			if (nk_option_label(m_ctx, "PLUS",				combo.sym_hover == NK_SYMBOL_PLUS)) 				combo.sym_hover = NK_SYMBOL_PLUS;
			if (nk_option_label(m_ctx, "MINUS",				combo.sym_hover == NK_SYMBOL_MINUS)) 				combo.sym_hover = NK_SYMBOL_MINUS;
			if (nk_option_label(m_ctx, "MAX",				combo.sym_hover == NK_SYMBOL_MAX)) 					combo.sym_hover = NK_SYMBOL_MAX;
			nk_tree_pop(m_ctx);
		}
		if (nk_tree_push(m_ctx, NK_TREE_NODE, "sym_active", NK_MINIMIZED)) {
			if (nk_option_label(m_ctx, "NONE",				combo.sym_active == NK_SYMBOL_NONE)) 				combo.sym_active = NK_SYMBOL_NONE;
			if (nk_option_label(m_ctx, "X",					combo.sym_active == NK_SYMBOL_X)) 					combo.sym_active = NK_SYMBOL_X;
			if (nk_option_label(m_ctx, "UNDERSCORE",		combo.sym_active == NK_SYMBOL_UNDERSCORE)) 			combo.sym_active = NK_SYMBOL_UNDERSCORE;
			if (nk_option_label(m_ctx, "CIRCLE_SOLID",		combo.sym_active == NK_SYMBOL_CIRCLE_SOLID)) 		combo.sym_active = NK_SYMBOL_CIRCLE_SOLID;
			if (nk_option_label(m_ctx, "CIRCLE_OUTLINE",	combo.sym_active == NK_SYMBOL_CIRCLE_OUTLINE))		combo.sym_active = NK_SYMBOL_CIRCLE_OUTLINE;
			if (nk_option_label(m_ctx, "RECT_SOLID",		combo.sym_active == NK_SYMBOL_RECT_SOLID)) 			combo.sym_active = NK_SYMBOL_RECT_SOLID;
			if (nk_option_label(m_ctx, "RECT_OUTLINE",		combo.sym_active == NK_SYMBOL_RECT_OUTLINE)) 		combo.sym_active = NK_SYMBOL_RECT_OUTLINE;
			if (nk_option_label(m_ctx, "TRIANGLE_UP",		combo.sym_active == NK_SYMBOL_TRIANGLE_UP)) 		combo.sym_active = NK_SYMBOL_TRIANGLE_UP;
			if (nk_option_label(m_ctx, "TRIANGLE_DOWN",		combo.sym_active == NK_SYMBOL_TRIANGLE_DOWN))		combo.sym_active = NK_SYMBOL_TRIANGLE_DOWN;
			if (nk_option_label(m_ctx, "TRIANGLE_LEFT",		combo.sym_active == NK_SYMBOL_TRIANGLE_LEFT))		combo.sym_active = NK_SYMBOL_TRIANGLE_LEFT;
			if (nk_option_label(m_ctx, "TRIANGLE_RIGHT",	combo.sym_active == NK_SYMBOL_TRIANGLE_RIGHT))		combo.sym_active = NK_SYMBOL_TRIANGLE_RIGHT;
			if (nk_option_label(m_ctx, "PLUS",				combo.sym_active == NK_SYMBOL_PLUS)) 				combo.sym_active = NK_SYMBOL_PLUS;
			if (nk_option_label(m_ctx, "MINUS",				combo.sym_active == NK_SYMBOL_MINUS)) 				combo.sym_active = NK_SYMBOL_MINUS;
			if (nk_option_label(m_ctx, "MAX",				combo.sym_active == NK_SYMBOL_MAX)) 				combo.sym_active = NK_SYMBOL_MAX;
			nk_tree_pop(m_ctx);
		}
		nk_tree_pop(m_ctx);
	}

	if (nk_tree_push_id(m_ctx, NK_TREE_NODE, "properties", NK_MINIMIZED, id + tree_index++)) {
		nk_layout_row_dynamic(m_ctx, 22, 1);
		nk_label(m_ctx, "rounding", NK_TEXT_LEFT);
		nk_property_float(m_ctx, "#value:", 0.f, &combo.rounding, 100.f, 1.f, 0.1f);
		nk_label(m_ctx, "border", NK_TEXT_LEFT);
		nk_property_float(m_ctx, "#value:", 0.f, &combo.border, 100.f, 1.f, 0.1f);

		PropertyVector2("content_padding", combo.content_padding, 0.f, 1000.f, 1.f, 1.f);
		PropertyVector2("button_padding", combo.button_padding, 0.f, 1000.f, 1.f, 1.f);
		PropertyVector2("spacing", combo.spacing, 0.f, 1000.f, 1.f, 1.f);

		nk_layout_row_dynamic(m_ctx, 22, 1);
		nk_label(m_ctx, "color_factor", NK_TEXT_LEFT);
		nk_property_float(m_ctx, "#value:", 0.f, &combo.color_factor, 1.f, 0.01f, 0.01f);
		nk_label(m_ctx, "disabled_factor", NK_TEXT_LEFT);
		nk_property_float(m_ctx, "#value:", 0.f, &combo.disabled_factor, 1.f, 0.01f, 0.01f);

		nk_tree_pop(m_ctx);
	}
}

void NKBase::ItemEditor(NKStyleItem& sItem)
{
	nk_layout_row_dynamic(m_ctx, 30, 1);
	if (nk_option_label(m_ctx, "color", sItem.option == 0)) sItem.option = 0;
	if (nk_option_label(m_ctx, "image", sItem.option == 1)) sItem.option = 1;
	if (nk_option_label(m_ctx, "nine_slice", sItem.option == 2)) sItem.option = 2;

	if (sItem.option == 0) {
		sItem.target->type = NK_STYLE_ITEM_COLOR;
		ColorPicker(sItem.target->data.color);
	}
	else if (sItem.option == 1 || sItem.option == 2) {
		auto mapSpr = m_manager->GetSprMap();
		int size = mapSpr->size();

		nk_layout_row_dynamic(m_ctx, 22, 1);
		nk_property_int(m_ctx, "#Index:", 0, &sItem.sprIndex, sItem.sprSize - 1, 1, 1);

		nk_layout_row_dynamic(m_ctx, 22, 2);
		nk_label(m_ctx, "Selected:", NK_TEXT_LEFT);
		std::filesystem::path filePath(sItem.imagePath.c_str());
		nk_label(m_ctx, filePath.filename().string().c_str(), NK_TEXT_RIGHT);
		if (nk_button_label(m_ctx, "apply"))
		{
			if (sItem.option == 1) {
				struct nk_image img;
				m_manager->GetSprite(sItem.imagePath.c_str(), sItem.sprIndex, img, true);
				(*sItem.target) = nk_style_item_image(img);
			}
			else if (sItem.option == 2) {
				struct nk_image img;
				m_manager->GetSprite(sItem.imagePath.c_str(), sItem.sprIndex, img, true);
				struct nk_nine_slice nineslice;
				nineslice.img = img;
				nineslice.l = (nk_ushort)sItem.nineslice[0];
				nineslice.t = (nk_ushort)sItem.nineslice[1];
				nineslice.r = (nk_ushort)sItem.nineslice[2];
				nineslice.b = (nk_ushort)sItem.nineslice[3];
				(*sItem.target) = nk_style_item_nine_slice(nineslice);
			}
		}
		if (nk_button_label(m_ctx, "clear"))
		{
			(*sItem.target) = (*sItem.restore);
		}
		if (sItem.option == 2) {
			nk_label(m_ctx, "nine_slice", NK_TEXT_LEFT);
			nk_layout_row_dynamic(m_ctx, 22, 1);
			nk_property_int(m_ctx, "#Left:", 0, &sItem.nineslice[0], 255, 1, 1);
			nk_property_int(m_ctx, "#Top:", 0, &sItem.nineslice[1], 255, 1, 1);
			nk_property_int(m_ctx, "#Right:", 0, &sItem.nineslice[2], 255, 1, 1);
			nk_property_int(m_ctx, "#Bottom:", 0, &sItem.nineslice[3], 255, 1, 1);
		}

		if (size > 0)
		{
			nk_layout_row_dynamic(m_ctx, 300, 1);
			if (nk_group_begin(m_ctx, "SPR List", NK_WINDOW_TITLE)) {

				float ratio[2] = { 0.8f, 0.2f };
				nk_layout_row(m_ctx, NK_DYNAMIC, 22, 2, ratio);
				int selected = 0;

				for (auto it = mapSpr->begin(); it != mapSpr->end(); ++it) {
					std::filesystem::path filePath((*it).first.c_str());
					nk_label(m_ctx, filePath.filename().string().c_str(), NK_TEXT_LEFT);

					if (nk_button_label(m_ctx, "Load")) {
						sItem.imagePath = (*it).first;
						sItem.sprSize = ((*it).second)->GetSpr()->GetXCount() * ((*it).second)->GetSpr()->GetYCount();
						if (sItem.sprSize <= sItem.sprIndex) {
							sItem.sprIndex = 0;
						}
					}
				}
				nk_group_end(m_ctx);
			}
		}
	}
}

void NKBase::PropertyVector2(const char* name, struct nk_vec2& vec, float max, float min, float step, float inc_per_pixel)
{
	nk_label(m_ctx, name, NK_TEXT_LEFT);
	nk_layout_row_dynamic(m_ctx, 22, 2);
	nk_property_float(m_ctx, "#X:", max, &vec.x, min, step, inc_per_pixel);
	nk_property_float(m_ctx, "#Y:", max, &vec.y, min, step, inc_per_pixel);
}

void NKBase::ColorPicker(struct nk_color& color)
{
	struct nk_colorf colorf;

	colorf.a = ((float)color.a / 255.0f);
	colorf.r = ((float)color.r / 255.0f);
	colorf.g = ((float)color.g / 255.0f);
	colorf.b = ((float)color.b / 255.0f);

	nk_layout_row_dynamic(m_ctx, 300, 1);
	colorf = nk_color_picker(m_ctx, colorf, NK_RGBA);

	nk_layout_row_dynamic(m_ctx, 22, 1);
	nk_property_float(m_ctx, "#r:", 0.f, &colorf.r, 255, 0.01f, 0.01f);
	nk_property_float(m_ctx, "#g:", 0.f, &colorf.g, 255, 0.01f, 0.01f);
	nk_property_float(m_ctx, "#b:", 0.f, &colorf.b, 255, 0.01f, 0.01f);
	nk_property_float(m_ctx, "#a:", 0.f, &colorf.a, 255, 0.01f, 0.01f);

	color.a = ((nk_byte)(colorf.a * 255.0f));
	color.r = ((nk_byte)(colorf.r * 255.0f));
	color.g = ((nk_byte)(colorf.g * 255.0f));
	color.b = ((nk_byte)(colorf.b * 255.0f));


}
