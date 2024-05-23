#include "pch.h"
#include "NKCheckbox.h"

NKCheckbox::NKCheckbox()
{
    m_type = eCHECKBOX;
    m_checked = 0;
    memset(m_label, 0, sizeof(m_label));
}

NKCheckbox::~NKCheckbox() {}

void NKCheckbox::Layout(nk_context* ctx)
{
    nk_checkbox_label(ctx, m_label, &m_checked);
}

void NKCheckbox::SetLabel(const char* label)
{
    strcpy_s(m_label, label);
}

void NKCheckbox::SetChecked(bool checked)
{
    m_checked = checked ? 1 : 0;
}

bool NKCheckbox::IsChecked() const
{
    return m_checked != 0;
}
