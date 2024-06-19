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
}

NKScrollbar::NKScrollbar(const NKScrollbar& other) : NKBase(other), NKStyleScrollbarH(other), NKStyleScrollbarV(other)
{
    m_type = other.m_type;
    m_scroll = other.m_scroll;
}

NKScrollbar::~NKScrollbar() {}

void NKScrollbar::Layout(nk_context* ctx)
{
    nk_slider_float(ctx, 0.0f, &m_scroll, 1.0f, 0.01f);
}

void NKScrollbar::EditStyle()
{
    EditComponentStyle(m_ctx, m_pManager);
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

float NKScrollbar::GetScroll() const
{
    return m_scroll;
}
