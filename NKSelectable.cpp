#include "pch.h"
#include "NKSelectable.h"

NKSelectable::NKSelectable()
{
    m_type = eSELECTABLE;
    m_selected = 0;
    memset(m_label, 0, sizeof(m_label));
    memcpy_s(m_label, sizeof(m_label), "Selectable", sizeof("Selectable"));
}

NKSelectable::~NKSelectable() {}

void NKSelectable::Layout(nk_context* ctx)
{
    if (nk_selectable_label(ctx, m_label, NK_TEXT_CENTERED, &m_selected)) {
        CallEvent(m_pManager);
    }
}

void NKSelectable::EditInfo()
{
    float ratio[2];
    ratio[0] = 0.3f;
    ratio[1] = 0.7f;
    nk_layout_row(m_ctx, NK_DYNAMIC, 44, 2, ratio);
    nk_label(m_ctx, "Text: ", NK_TEXT_LEFT);
    nk_flags result = m_pManager->IMEInputSystem(m_ctx, NK_EDIT_FIELD | NK_EDIT_SIG_ENTER, m_cEditName, sizeof(m_cEditName), nk_filter_default, &m_cEditName_len);
    if (result & NK_EDIT_COMMITED) {
        SetLabel(m_cEditName);
    }
}

void NKSelectable::SetLabel(const char* label)
{
    if (strlen(label) <= 0) {

        m_pManager->ErrorPopup("A selectable must have a string.");
        return;
    }

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
