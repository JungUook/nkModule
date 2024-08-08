#include "pch.h"
#include "NKCombo.h"
#include "NKComboItem.h"

NKCombo::NKCombo() : NKBase(), NKStyleCombo(), NKStyleContextualButton(), NKStyleWindow(), NKStyleScrollbarH(), NKStyleScrollbarV()
{
	m_type = eCOMBO;

	m_labelSize.x = 150.f;
	m_labelSize.y = 300.f;
	m_currentLabel = 0;
	m_labelAlignment = NK_TEXT_LEFT;

	m_cComboLabel = "None";
	m_iComboFlag = eCOMBO_DYNAMIC;
	m_dynamicLabelSpace = { 0.f, 0.f };
}

NKCombo::NKCombo(nk_context* ctx, NuklearUI* pManager) : NKBase(ctx, pManager), NKStyleCombo(ctx, &m_style), NKStyleContextualButton(ctx, &m_style), NKStyleWindow(ctx, &m_style), NKStyleScrollbarH(ctx, &m_style), NKStyleScrollbarV(ctx, &m_style)
{
	m_type = eCOMBO;

	m_sTransform.w = 150.f;
	m_sTransform.h = 60.f;
	m_labelSize.x = 150.f;
	m_labelSize.y = 300.f;
	m_currentLabel = 0;
	m_labelAlignment = NK_TEXT_LEFT;

	m_cComboLabel = "None";
	m_iComboFlag = eCOMBO_DYNAMIC;
	m_dynamicLabelSpace = { 0.f, 0.f };
}

NKCombo::NKCombo(const NKCombo& other) : NKBase(other), NKStyleCombo(other, m_ctx, &m_style), NKStyleContextualButton(other, m_ctx, &m_style), NKStyleWindow(other, m_ctx, &m_style), NKStyleScrollbarH(other, m_ctx, &m_style), NKStyleScrollbarV(other, m_ctx, &m_style)
{
	m_type = other.m_type;

	m_labelSize.x = other.m_labelSize.x;
	m_labelSize.y = other.m_labelSize.y;
	m_currentLabel = other.m_currentLabel;
	m_labelAlignment = other.m_labelAlignment;

	m_cComboLabel = other.m_cComboLabel;
	m_iComboFlag = other.m_iComboFlag;
	m_dynamicLabelSpace = { 0.f, 0.f };
}

NKCombo::~NKCombo()
{
}

void NKCombo::Layout(nk_context* ctx)
{
	if (m_iComboFlag == eCOMBO_DYNAMIC) {
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
	else {

		if (nk_combo_begin_label(ctx, m_cComboLabel.c_str(), m_labelSize))
		{
			nk_layout_space_begin(ctx, NK_STATIC, m_dynamicLabelSpace.y, m_pChildList.size());

			m_dynamicLabelSpace = nk_vec2(0.f, 0.f);
			int i = 0;
			for (std::list<NKBase*>::iterator it = m_pChildList.begin(); it != m_pChildList.end(); ++it) {
				nk_layout_space_push(ctx, nk_rect(m_dynamicLabelSpace.x, m_dynamicLabelSpace.y, (*it)->GetWidth(),(*it)->GetHeight()));
				m_dynamicLabelSpace.y += (*it)->GetHeight();

				if ((*it)->GetType() == eCOMBO_ITEM)
				{
					NKComboItem* pItem = (NKComboItem*)(*it);
					pItem->SetLabelNumber(i++);
				}
				(*it)->Update(ctx);
			}
			nk_layout_space_end(ctx);
			nk_combo_end(ctx);
		}
	}

	
}

void NKCombo::SafeRenderStart(nk_context* ctx)
{
	NKStyleCombo::UpdateComponent(ctx, m_pManager);
	NKStyleContextualButton::UpdateComponent(ctx, m_pManager);
	NKStyleWindow::UpdateComponent(ctx, m_pManager);
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

	nk_layout_row_dynamic(ctx, 22, 1);
	nk_label(ctx, "combo type", NK_TEXT_LEFT);
	nk_layout_row_dynamic(ctx, 22, 2);
	if (nk_option_label(ctx, "dynamic", m_iComboFlag == eCOMBO_DYNAMIC)) m_iComboFlag = eCOMBO_DYNAMIC;
	if (nk_option_label(ctx, "static", m_iComboFlag == eCOMBO_STATIC)) m_iComboFlag = eCOMBO_STATIC;

	nk_layout_row_dynamic(ctx, 22, 1);
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
	NKStyleCombo::EditComponentStyle(ctx, m_pManager);
	NKStyleContextualButton::EditComponentStyle(ctx, m_pManager);
	NKStyleWindow::EditComponentStyle(ctx, m_pManager);
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

NKComboItem* NKCombo::AddItem(const char* name)
{
	NKBase* pBase = CreateUI("NKComboItem");
	NKComboItem* pItem = static_cast<NKComboItem*>(pBase);

	if (pItem != nullptr) {
		pItem->SetLabel(name);
	}
	else {
		m_pManager->Remove(pBase);
		pBase = nullptr;
		pItem = nullptr;

#ifdef _NKDEBUG
		m_pManager->ErrorPopup("I failed to create the combo item.");
#endif
	}

	return pItem;
}

NKComboItem* NKCombo::LAddItem(luabridge::LuaRef ref)
{
#ifdef _NKDEBUG
	CHECK_LUA_REF_RETURN(ref);
#endif
	std::string name = ref.cast<std::string>();
	NKComboItem* pItem = AddItem(name.c_str());
	return pItem;
}

bool NKCombo::CAddItem(void* param)
{
	const char** name = static_cast<const char**>(param);

	if (name) {
		AddItem(*name);
		return true;
	}
	return false;
}

void NKCombo::UpdateComponent(nk_context* ctx, NuklearUI* pManager)
{
	NKStyleCombo::UpdateComponent(ctx, pManager);
	NKStyleContextualButton::UpdateComponent(ctx, pManager);
	NKStyleWindow::UpdateComponent(ctx, pManager);
	NKStyleScrollbarH::UpdateComponent(ctx, pManager);
	NKStyleScrollbarV::UpdateComponent(ctx, pManager);
}

void NKCombo::EditComponentStyle(nk_context* ctx, NuklearUI* pManager)
{
	NKStyleCombo::EditComponentStyle(ctx, pManager);
	NKStyleContextualButton::EditComponentStyle(ctx, pManager);
	NKStyleWindow::EditComponentStyle(ctx, pManager);
	NKStyleScrollbarH::EditComponentStyle(ctx, pManager);
	NKStyleScrollbarV::EditComponentStyle(ctx, pManager);
}

void NKCombo::RegistCommand(const char* classname)
{
	NKBase::RegistCommand(classname);
	MAKE_INTERFACE(m_mapFunc, this, NKCombo::CSetComboName, classname);
	MAKE_INTERFACE(m_mapFunc, this, NKCombo::CSetLabelSize, classname);
	MAKE_INTERFACE(m_mapFunc, this, NKCombo::CAddItem, classname);
}
