#include "pch.h"
#include "NKTooltip.h"

NKTooltip::NKTooltip() : NKBase(), NKBaseLabel(), NKObjectFinder(), NKStyleWindow(), NKStyleText()
{
    m_type = eTOOLTIP;
}

NKTooltip::NKTooltip(nk_context* ctx, NuklearUI* pManager) : NKBase(ctx, pManager), NKBaseLabel(), NKObjectFinder(pManager), NKStyleWindow(ctx, &m_style), NKStyleText(ctx, &m_style)
{
    m_type = eTOOLTIP;
    SetLabel("Tooltip");
    m_cTransform.x = 0.f;
    m_cTransform.y = 0.f;
    m_cTransform.w = 150.f;
    m_cTransform.h = 40.f;
}

NKTooltip::NKTooltip(const NKTooltip& other) : NKBase(other), NKBaseLabel(other), NKObjectFinder(other), NKStyleWindow(other, m_ctx, &m_style), NKStyleText(other, m_ctx, &m_style)
{
    m_type = other.m_type;
}

NKTooltip::~NKTooltip() {}

void NKTooltip::Layout(nk_context* ctx)
{
    if (m_pResultObject == nullptr) return;

    if (!m_pResultObject->IsHovering()) return;

    UpdateComponent(ctx, m_pManager);

    if (nk_tooltip_begin(ctx, GetWidth()))
    {
        nk_layout_row_dynamic(ctx, 22, 1);
        nk_label(ctx, m_cContent, NK_TEXT_CENTERED);

        nk_tooltip_end(ctx);
    }
}

void NKTooltip::SafeRenderStart(nk_context* ctx)
{
    UpdateComponent(ctx, m_pManager);
}

void NKTooltip::SafeRenderEnd(nk_context* ctx)
{
}

void NKTooltip::EditInfo(nk_context* ctx)
{
    EditLabel(ctx, m_pManager);

    FoundObject(ctx, m_pManager);
    SearchObject(ctx, m_pManager);
}

void NKTooltip::EditStyle(nk_context* ctx)
{
    NKBase::EditStyle(ctx);
    EditComponentStyle(ctx, m_pManager);
}

void NKTooltip::UpdateComponent(nk_context* ctx, NuklearUI* pManager)
{
    NKStyleWindow::UpdateComponent(ctx, pManager);
    NKStyleText::UpdateComponent(ctx, pManager);
}

void NKTooltip::EditComponentStyle(nk_context* ctx, NuklearUI* pManager)
{
    NKStyleWindow::EditComponentStyle(ctx, pManager);
    NKStyleText::EditComponentStyle(ctx, pManager);
}
