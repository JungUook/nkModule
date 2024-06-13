#include "pch.h"
#include "NKScrollbar.h"

NKScrollbar::NKScrollbar(nk_context* ctx, NuklearUI* pManager) : NKBase(ctx, pManager)
{
    m_type = eSCROLLBAR;
    m_scroll = 0.0f;
}

NKScrollbar::~NKScrollbar() {}

void NKScrollbar::Layout(nk_context* ctx)
{
    nk_slider_float(ctx, 0.0f, &m_scroll, 1.0f, 0.01f);
}

void NKScrollbar::SetScroll(float scroll)
{
    m_scroll = scroll;
}

float NKScrollbar::GetScroll() const
{
    return m_scroll;
}
