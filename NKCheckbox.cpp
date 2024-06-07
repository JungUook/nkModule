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

void NKCheckbox::EditInfo()
{
	float ratio[2];
	ratio[0] = 0.3f;
	ratio[1] = 0.7f;
	nk_layout_row(m_ctx, NK_DYNAMIC, 44, 2, ratio);
	nk_label(m_ctx, "Text: ", NK_TEXT_LEFT);

	//nk_flags result = nk_edit_string(m_ctx, NK_EDIT_FIELD, m_editName, &m_editName_len, 64, nk_filter_default);
	nk_flags result = nk_edit_string_zero_terminated(m_ctx, NK_EDIT_FIELD, m_editName, sizeof(m_editName), nk_filter_default);
	if (result & NK_EDIT_ACTIVE) {
		m_manager->IMEInputSystem(m_editName, &m_editName_len);
	}

	if (result & NK_EDIT_COMMITED) {
		SetLabel(m_editName);
	}
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
