#include "pch.h"
#include "NKSelectable.h"

NKSelectable::NKSelectable()
{
    m_type = eSELECTABLE;
    m_selected = 0;
    memset(m_label, 0, sizeof(m_label));
}

NKSelectable::~NKSelectable() {}

void NKSelectable::Layout(nk_context* ctx)
{
    nk_selectable_label(ctx, m_label, NK_TEXT_CENTERED, &m_selected);
}

void NKSelectable::SetLabel(const char* label)
{
    strcpy_s(m_label, label);
}

void NKSelectable::SetSelected(bool selected)
{
    m_selected = selected ? 1 : 0;
}

bool NKSelectable::IsSelected() const
{
    return m_selected != 0;
}
