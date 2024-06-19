#include "pch.h"
#include "NKTooltip.h"

NKTooltip::NKTooltip() : NKBase(), NKBaseLabel(), NKObjectFinder(), NKStyleWindow(), NKStyleText()
{
    m_type = eTOOLTIP;
}

NKTooltip::NKTooltip(nk_context* ctx, NuklearUI* pManager) : NKBase(ctx, pManager), NKBaseLabel(), NKObjectFinder(pManager), NKStyleWindow(ctx, &m_style), NKStyleText(ctx, &m_style)
{
    m_type = eTOOLTIP;
}

NKTooltip::NKTooltip(const NKTooltip& other) : NKBase(other), NKBaseLabel(other), NKObjectFinder(other), NKStyleWindow(other), NKStyleText(other)
{
    m_type = other.m_type;
}

NKTooltip::~NKTooltip() {}

void NKTooltip::Layout(nk_context* ctx)
{
    if (m_pResultObject == nullptr) return;

    if (!m_pResultObject->IsHovering()) return;

    if (nk_tooltip_begin(ctx, GetWidth()))
    {
        nk_layout_row_dynamic(ctx, 22, 1);
        nk_label(ctx, m_cContent, NK_TEXT_CENTERED);

        nk_tooltip_end(ctx);
    }
}

void NKTooltip::EditInfo()
{
    EditLabel(m_ctx, m_pManager);

    FoundObject(m_ctx, m_pManager);
    SearchObject(m_ctx, m_pManager);
}

void NKTooltip::EditStyle()
{
    NKBase::EditStyle();
    EditComponentStyle(m_ctx, m_pManager);
}

void NKTooltip::EditComponentStyle(nk_context* ctx, NuklearUI* pManager)
{
    NKStyleWindow::EditComponentStyle(ctx, pManager);
    NKStyleText::EditComponentStyle(ctx, pManager);
}
