#include "pch.h"
#include "NKScrollbar.h"

NKScrollbar::NKScrollbar() : NKBase(), NKStyleScrollbarH(), NKStyleScrollbarV()
{
    m_type = eSCROLLBAR;
    m_scroll = 0.0f;
}

NKScrollbar::NKScrollbar(nk_context* ctx, NuklearUI* pManager) : NKBase(ctx, pManager), NKStyleScrollbarH(ctx, &m_style), NKStyleScrollbarV(ctx, &m_style)
{
    m_type = eSCROLLBAR;
    m_scroll = 0.0f;
    m_cTransform.x = 0.f;
    m_cTransform.y = 0.f;
    m_cTransform.w = 150.f;
    m_cTransform.h = 40.f;
}

NKScrollbar::NKScrollbar(const NKScrollbar& other) : NKBase(other), NKStyleScrollbarH(other, m_ctx, &m_style), NKStyleScrollbarV(other, m_ctx, &m_style)
{
    m_type = other.m_type;
    m_scroll = other.m_scroll;
}

NKScrollbar::~NKScrollbar() {}

void NKScrollbar::Layout(nk_context* ctx)
{
    UpdateComponent(ctx, m_pManager);

    nk_slider_float(ctx, 0.0f, &m_scroll, 1.0f, 0.01f);
}

void NKScrollbar::SafeRenderStart(nk_context* ctx)
{
    UpdateComponent(ctx, m_pManager);
}

void NKScrollbar::SafeRenderEnd(nk_context* ctx)
{
}

void NKScrollbar::EditStyle(nk_context* ctx)
{
    EditComponentStyle(ctx, m_pManager);
}

void NKScrollbar::UpdateComponent(nk_context* ctx, NuklearUI* pManager)
{
    NKStyleScrollbarH::UpdateComponent(ctx, pManager);
    NKStyleScrollbarV::UpdateComponent(ctx, pManager);
}

void NKScrollbar::EditComponentStyle(nk_context* ctx, NuklearUI* pManager)
{
    NKStyleScrollbarH::EditComponentStyle(ctx, pManager);
    NKStyleScrollbarV::EditComponentStyle(ctx, pManager);
}

void NKScrollbar::SetScroll(float scroll)
{
    m_scroll = scroll;
}

bool NKScrollbar::CSetScroll(void* param)
{
    float* scroll = static_cast<float*>(param);
    if (scroll) {
        SetScroll(*scroll);
        return true;
    }

    return false;
}

float NKScrollbar::GetScroll() const
{
    return m_scroll;
}

bool NKScrollbar::CGetScroll(void* param) const
{
    float* scroll = static_cast<float*>(param);
    if (scroll) {
        (*scroll) = GetScroll();
        return true;
    }

    return false;
}

void NKScrollbar::RegistCommand(const char* classname)
{
    NKBase::RegistCommand(classname);
    MAKE_INTERFACE(m_mapFunc, this, NKScrollbar::CSetScroll, classname);
    MAKE_INTERFACE(m_mapFunc, this, NKScrollbar::CGetScroll, classname);
}
