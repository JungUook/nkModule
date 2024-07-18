#include "pch.h"
#include "NKCheckbox.h"

NKCheckbox::NKCheckbox() : NKBase(), NKBaseLabel(), NKStyleCheckbox()
{
    m_type = eCHECKBOX;
    m_checked = 0;
}

NKCheckbox::NKCheckbox(nk_context* ctx, NuklearUI* pManager) : NKBase(ctx, pManager), NKBaseLabel(), NKStyleCheckbox(ctx, &m_style)
{
    m_type = eCHECKBOX;
    m_checked = 0;
    SetLabel("Checkbox");
    m_cTransform.w = 150.f;
    m_cTransform.h = 40.f;
}

NKCheckbox::NKCheckbox(const NKCheckbox& other) : NKBase(other), NKBaseLabel(other), NKStyleCheckbox(other, m_ctx, &m_style)
{
	m_type = other.m_type;
	m_checked = other.m_checked;
}

NKCheckbox::~NKCheckbox() {}

void NKCheckbox::LayoutBegin(nk_context* ctx)
{
    CustomFontSizeBegin(ctx, m_font);
}

void NKCheckbox::Layout(nk_context* ctx)
{
    UpdateComponent(ctx, m_pManager);

    nk_checkbox_label(ctx, m_sContent.c_str(), &m_checked);
}

void NKCheckbox::LayoutEnd(nk_context* ctx)
{
    CustomFontSizeEnd(ctx, m_pManager, m_font);
}

void NKCheckbox::SafeRenderStart(nk_context* ctx)
{
    UpdateComponent(ctx, m_pManager);
}

void NKCheckbox::SafeRenderEnd(nk_context* ctx)
{
}

void NKCheckbox::EditInfo(nk_context* ctx)
{
    EditLabel(ctx, m_pManager);
}

void NKCheckbox::EditStyle(nk_context* ctx)
{
    EditComponentStyle(ctx, m_pManager);
}

void NKCheckbox::SetChecked(bool checked)
{
    m_checked = checked ? 1 : 0;
}

void NKCheckbox::LSetChecked(luabridge::LuaRef ref)
{
    CHECK_LUA_REF(ref);
    bool bChecked = ref.cast<bool>();
    SetChecked(bChecked);
}

bool NKCheckbox::CSetChecked(void* param)
{
    bool* checked = static_cast<bool*>(param);

    if (checked) {
        SetChecked(*checked);
        return true;
    }
    return false;
}

bool NKCheckbox::IsChecked() const
{
    return m_checked != 0;
}

bool NKCheckbox::CIsChecked(void* param) const
{
    bool* checked = static_cast<bool*>(param);

    if (checked) {
        (*checked) = IsChecked();
        return true;
    }
    return false;
}

void NKCheckbox::RegistCommand(const char* classname)
{
    NKBase::RegistCommand(classname);
    MAKE_INTERFACE(m_mapFunc, this, NKCheckbox::CSetLabel, classname);
    MAKE_INTERFACE(m_mapFunc, this, NKCheckbox::CSetChecked, classname);
    MAKE_INTERFACE(m_mapFunc, this, NKCheckbox::CIsChecked, classname);
}
