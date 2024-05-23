#include "pch.h"
#include "NKMenu.h"

NKMenu::NKMenu()
{
    m_type = eMENU;
    memset(m_label, 0, sizeof(m_label));
}

NKMenu::~NKMenu() {}

void NKMenu::Layout(nk_context* ctx)
{
    if (nk_menu_begin_label(ctx, m_label, NK_TEXT_LEFT, nk_vec2(120, 200)))
    {
        for (auto& item : m_items)
        {
            if (nk_menu_item_label(ctx, item, NK_TEXT_LEFT))
            {
                // Handle menu item selection
            }
        }
        nk_menu_end(ctx);
    }
}

void NKMenu::SetLabel(const char* label)
{
    strcpy_s(m_label, label);
}

void NKMenu::AddMenuItem(const char* label, nk_context* ctx)
{
    m_items.push_back(label);
}
