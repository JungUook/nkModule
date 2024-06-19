#include "pch.h"
#include "NKSelectable.h"

NKSelectable::NKSelectable() : NKBase(), NKHandler(), NKBaseLabel(), NKStyleSelectedable()
{
    m_type = eSELECTABLE;
    m_selected = 0;
    memcpy_s(m_cContent, sizeof(m_cContent), "Selectable", sizeof("Selectable"));
}

NKSelectable::NKSelectable(nk_context* ctx, NuklearUI* pManager) : NKBase(ctx, pManager), NKHandler(), NKBaseLabel(), NKStyleSelectedable(ctx, &m_style)
{
    m_type = eSELECTABLE;
    m_selected = 0;
    memcpy_s(m_cContent, sizeof(m_cContent), "Selectable", sizeof("Selectable"));
}

NKSelectable::NKSelectable(const NKSelectable& other) : NKBase(other), NKHandler(other), NKBaseLabel(other), NKStyleSelectedable(other)
{
    m_type = other.m_type;
    m_selected = other.m_selected;
}

NKSelectable::~NKSelectable() {}

void NKSelectable::Layout(nk_context* ctx)
{
    if (nk_selectable_label(ctx, m_cContent, NK_TEXT_CENTERED, &m_selected)) {
        CallEvent(m_pManager);
    }
}

void NKSelectable::EditInfo()
{
    EditLabel(m_ctx, m_pManager);
}

void NKSelectable::EditStyle()
{
    EditComponentStyle(m_ctx, m_pManager);
}

void NKSelectable::SetLabel(const char* text)
{
    if (strlen(text) <= 0) {

        m_pManager->ErrorPopup("A selectable must have a string.");
        return;
    }

    NKBaseLabel::SetLabel(text);
}

void NKSelectable::SetSelected(bool selected)
{
    m_selected = selected ? 1 : 0;
}

bool NKSelectable::IsSelected() const
{
    return m_selected != 0;
}
