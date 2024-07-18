#include "pch.h"
#include "NKCombo.h"
#include "NKComboItem.h"

NKCombo::NKCombo() : NKBase(), NKStyleCombo()
{
	m_type = eCOMBO;

	m_labelSize.x = 150.f;
	m_labelSize.y = 300.f;
	m_currentLabel = 0;
	m_labelAlignment = NK_TEXT_LEFT;

	m_cComboLabel = "None";
}

NKCombo::NKCombo(nk_context* ctx, NuklearUI* pManager) : NKBase(ctx, pManager), NKStyleCombo(ctx, &m_style)
{
	m_type = eCOMBO;

	m_cTransform.w = 150.f;
	m_cTransform.h = 60.f;
	m_labelSize.x = 150.f;
	m_labelSize.y = 300.f;
	m_currentLabel = 0;
	m_labelAlignment = NK_TEXT_LEFT;

	m_cComboLabel = "None";
}

NKCombo::NKCombo(const NKCombo& other) : NKBase(other), NKStyleCombo(other, m_ctx, &m_style)
{
	m_type = other.m_type;

	m_labelSize.x = other.m_labelSize.x;
	m_labelSize.y = other.m_labelSize.y;
	m_currentLabel = other.m_currentLabel;
	m_labelAlignment = other.m_labelAlignment;

	m_cComboLabel = other.m_cComboLabel;
}

NKCombo::~NKCombo()
{
}

void NKCombo::Layout(nk_context* ctx)
{
	if (nk_combo_begin_label(ctx, m_cComboLabel.c_str(), m_labelSize))
	{
		nk_layout_space_begin(ctx, NK_STATIC, m_labelSize.y, m_pChildList.size());

		int i = 0;
		for (std::list<NKBase*>::iterator it = m_pChildList.begin(); it != m_pChildList.end(); ++it) {
			nk_layout_space_push(ctx, nk_rect(0, (m_labelSize.y / m_pChildList.size()) * i++, m_labelSize.x, m_labelSize.y / m_pChildList.size()));
			
			if ((*it)->GetType() == eCOMBO_ITEM)
			{
				NKComboItem* pItem = (NKComboItem*)(*it);
				pItem->SetLabelNumber(i);
			}
			(*it)->Update(ctx);
		}
		nk_layout_space_end(ctx);
		nk_combo_end(ctx);
	}
}

void NKCombo::SafeRenderStart(nk_context* ctx)
{
	UpdateComponent(ctx, m_pManager);
}

void NKCombo::SafeRenderEnd(nk_context* ctx)
{
}

void NKCombo::EditInfo(nk_context* ctx)
{
	nk_layout_row_dynamic(ctx, 22, 1);
	nk_label(ctx, "Create_UI", NK_TEXT_LEFT);
	if (nk_button_label(ctx, "ComboItem"))
	{
		CreateUI("NKComboItem");
	}

	nk_label(ctx, "alignment", NK_TEXT_LEFT);
	nk_layout_row_dynamic(ctx, 22, 3);
	if (nk_option_label(ctx, "left", m_labelAlignment == NK_TEXT_LEFT)) m_labelAlignment = NK_TEXT_LEFT;
	if (nk_option_label(ctx, "center", m_labelAlignment == NK_TEXT_CENTERED)) m_labelAlignment = NK_TEXT_CENTERED;
	if (nk_option_label(ctx, "right", m_labelAlignment == NK_TEXT_RIGHT)) m_labelAlignment = NK_TEXT_RIGHT;

	PropertyVector2(ctx, "Label Size", m_labelSize, .0f, 500.f, 0.01f, 0.01f);

	if (nk_tree_push(ctx, NK_TREE_NODE, "Combo Item List", NK_MINIMIZED)) {
		for (std::list<NKBase*>::iterator it = m_pChildList.begin(); it != m_pChildList.end(); ++it) {
			if ((*it)->GetType() == eCOMBO_ITEM)
			{
				NKComboItem* pItem = (NKComboItem*)(*it);
				pItem->EditInfo(ctx);
			}
		}
		nk_tree_pop(ctx);
	}	
}

void NKCombo::EditStyle(nk_context* ctx)
{
	EditComponentStyle(ctx, m_pManager);
}

void NKCombo::SetComboName(const char* name)
{
	m_cComboLabel = name;
}

void NKCombo::LSetComboName(luabridge::LuaRef ref)
{
	CHECK_LUA_REF(ref);
	std::string name = ref.cast<std::string>();
	SetComboName(name.c_str());
}

bool NKCombo::CSetComboName(void* param)
{
	const char** name = static_cast<const char**>(param);

	if (name) {
		SetComboName(*name);
		return true;
	}
	return false;
}

void NKCombo::SetLabelSize(float x, float y)
{
	m_labelSize.x = x;
	m_labelSize.y = y;
}

void NKCombo::LSetLabelSize(luabridge::LuaRef ref)
{
	CHECK_LUA_REF(ref);
	float x = ref["x"].cast<float>();
	float y = ref["y"].cast<float>();
	SetLabelSize(x, y);
}

bool NKCombo::CSetLabelSize(void* param)
{
	void** arr = static_cast<void**>(param);

	if (arr) {
		float* x = static_cast<float*>(arr[0]);
		float* y = static_cast<float*>(arr[1]);

		if (x && y) {
			SetLabelSize(*x, *y);
			return true;
		}
		else {
			return false;
		}
	}

	return false;
}

void NKCombo::SetCurrentLabel(int number)
{
	m_currentLabel = number;
}

void NKCombo::RegistCommand(const char* classname)
{
	NKBase::RegistCommand(classname);
	MAKE_INTERFACE(m_mapFunc, this, NKCombo::CSetComboName, classname);
	MAKE_INTERFACE(m_mapFunc, this, NKCombo::CSetLabelSize, classname);
}
