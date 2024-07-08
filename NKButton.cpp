#include "pch.h"
#include "NKButton.h"

NKButton::NKButton() : NKBase(), NKHandler(), NKBaseLabel(), NKStyleButton()
{
	m_type = eBUTTON;
}

NKButton::NKButton(nk_context* ctx, NuklearUI* pManager) : NKBase(ctx, pManager), NKHandler(), NKBaseLabel(), NKStyleButton(ctx, &m_style)
{
	m_type = eBUTTON;
	m_cTransform.x = 0.f;
	m_cTransform.y = 0.f;
	m_cTransform.w = 150.f;
	m_cTransform.h = 40.f;
	SetLabel("Button");
}

NKButton::NKButton(const NKButton& other) : NKBase(other), NKHandler(other), NKBaseLabel(other), NKStyleButton(other, m_ctx, &m_style)
{
	m_type = eBUTTON;
}

NKButton::~NKButton()
{
}

void NKButton::LayoutBegin(nk_context* ctx)
{
	CustomFontSizeBegin(ctx, m_font);
}

void NKButton::Layout(nk_context* ctx)
{
	if (nk_button_label(ctx, m_cContent)) {
		if (!m_bDisabled) {
			CallEvent(m_pLuaManager);
		}
	}
}

void NKButton::LayoutEnd(nk_context* ctx)
{
	CustomFontSizeEnd(ctx, m_pManager, m_font);
}

void NKButton::SafeRenderStart(nk_context* ctx)
{
	UpdateComponent(ctx, m_pManager);
}

void NKButton::SafeRenderEnd(nk_context* ctx)
{
}

void NKButton::EditInfo(nk_context* ctx)
{
	nk_layout_row_dynamic(ctx, 33, 1);
	if (nk_checkbox_label(ctx, "Disabled", &m_bDisabled)) {
		DisableButton(m_bDisabled);
	}
	EditLabel(ctx, m_pManager);
	EditInfoData(ctx, m_pManager, m_pLuaManager);
}

void NKButton::EditStyle(nk_context* ctx)
{
	NKBase::EditStyle(ctx);
	EditComponentStyle(ctx, m_pManager);
}

void NKButton::RegistCommand(const char* classname)
{
	NKBase::RegistCommand(classname);
	MAKE_INTERFACE(m_mapFunc, this, NKButton::CDisableButton, classname);
}

void NKButton::DisableButton(bool bDisabled)
{
	m_bDisabled = bDisabled;
	m_pComponent->DisableButton(m_bDisabled);
}

void NKButton::LDisableButton(luabridge::LuaRef ref)
{
	CHECK_LUA_REF(ref);
	bool bDisabled = ref.cast<bool>();
	DisableButton(bDisabled);
}

bool NKButton::CDisableButton(void* param)
{
	bool* bDisabled = static_cast<bool*>(param);
	if (bDisabled) {
		DisableButton(*bDisabled);
		return true;
	}
	return false;
}
