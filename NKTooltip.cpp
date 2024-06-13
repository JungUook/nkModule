#include "pch.h"
#include "NKTooltip.h"

NKTooltip::NKTooltip(nk_context* ctx, NuklearUI* pManager) : NKBase(ctx, pManager)
{
    m_type = eTOOLTIP;
    memset(m_tooltip, 0, sizeof(m_tooltip));
}

NKTooltip::~NKTooltip() {}

void NKTooltip::Layout(nk_context* ctx)
{
    if (nk_tooltip_begin(ctx, GetWidth()))
    {
        nk_tooltip_end(ctx);
    }
}

void NKTooltip::SetTooltip(const char* tooltip)
{
    strcpy_s(m_tooltip, tooltip);
}
