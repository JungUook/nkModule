#include "pch.h"
#include "NKSelectable.h"

NKSelectable::NKSelectable() : NKBase(), NKHandler(), NKBaseLabel(), NKStyleSelectedable()
{
    m_type = eSELECTABLE;
    m_selected = 0;
}

NKSelectable::NKSelectable(nk_context* ctx, NuklearUI* pManager) : NKBase(ctx, pManager), NKHandler(), NKBaseLabel(), NKStyleSelectedable(ctx, &m_style)
{
    m_type = eSELECTABLE;
    m_selected = 0;
    SetLabel("Selectable");
    m_cTransform.w = 150.f;
    m_cTransform.h = 40.f;
}

NKSelectable::NKSelectable(const NKSelectable& other) : NKBase(other), NKHandler(other), NKBaseLabel(other), NKStyleSelectedable(other, m_ctx, &m_style)
{
    m_type = other.m_type;
    m_selected = other.m_selected;
}

NKSelectable::~NKSelectable() {}

void NKSelectable::LayoutBegin(nk_context* ctx)
{
    CustomFontSizeBegin(ctx, m_font);
}

void NKSelectable::Layout(nk_context* ctx)
{
    UpdateComponent(ctx, m_pManager);

    if (nk_selectable_label(ctx, m_sContent.c_str(), NK_TEXT_CENTERED, &m_selected)) {
        CallEvent(m_pLuaManager);
    }
}

void NKSelectable::LayoutEnd(nk_context* ctx)
{
    CustomFontSizeEnd(ctx, m_pManager, m_font);
}

void NKSelectable::SafeRenderStart(nk_context* ctx)
{
    UpdateComponent(ctx, m_pManager);
}

void NKSelectable::SafeRenderEnd(nk_context* ctx)
{
}

void NKSelectable::EditInfo(nk_context* ctx)
{
    EditLabel(ctx, m_pManager);
}

void NKSelectable::EditStyle(nk_context* ctx)
{
    EditComponentStyle(ctx, m_pManager);
}

void NKSelectable::SetLabel(const char* text)
{
    if (strlen(text) <= 0) {
#ifdef _NKDEBUG
        m_pManager->ErrorPopup("A selectable must have a string.");
#endif
        return;
    }

    NKBaseLabel::SetLabel(text);
}

void NKSelectable::SetSelected(bool selected)
{
    m_selected = selected ? 1 : 0;
}

void NKSelectable::LSetSelected(luabridge::LuaRef ref)
{
    CHECK_LUA_REF(ref);
    bool bSelected = ref.cast<bool>();
    SetSelected(bSelected);
}

bool NKSelectable::CSetSelected(void* param)
{
    bool* selected = static_cast<bool*>(param);

    if (selected) {
        SetSelected(*selected);
        return true;
    }

    return false;
}

bool NKSelectable::IsSelected() const
{
    return m_selected != 0;
}

bool NKSelectable::CIsSelected(void* param) const
{
    bool* selected = static_cast<bool*>(param);
    if (selected) {
        (*selected) = IsSelected();
        return true;
    }

    return false;
}

void NKSelectable::RegistCommand(const char* classname)
{
    NKBase::RegistCommand(classname);
    MAKE_INTERFACE(m_mapFunc, this, NKSelectable::CSetLabel, classname);
    MAKE_INTERFACE(m_mapFunc, this, NKSelectable::CSetSelected, classname);
    MAKE_INTERFACE(m_mapFunc, this, NKSelectable::CIsSelected, classname);
}
