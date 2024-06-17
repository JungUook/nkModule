#include "pch.h"
#include "NKCheckbox.h"

NKCheckbox::NKCheckbox(nk_context* ctx, NuklearUI* pManager) : NKBase(ctx, pManager), NKBaseLabel(), NKStyleCheckbox(ctx, &m_style)
{
    m_type = eCHECKBOX;
    m_checked = 0;
}

NKCheckbox::NKCheckbox(const NKCheckbox& other) : NKBase(other), NKBaseLabel(other), NKStyleCheckbox(other)
{
	m_type = other.m_type;
	m_checked = other.m_checked;
}

NKCheckbox::~NKCheckbox() {}

void NKCheckbox::Layout(nk_context* ctx)
{
    nk_checkbox_label(ctx, m_cContent, &m_checked);
}

void NKCheckbox::EditInfo()
{
    EditLabel(m_ctx, m_pManager);
}

void NKCheckbox::EditStyle()
{
    EditComponentStyle(m_ctx, m_pManager);
}

void NKCheckbox::SetChecked(bool checked)
{
    m_checked = checked ? 1 : 0;
}

bool NKCheckbox::IsChecked() const
{
    return m_checked != 0;
}
