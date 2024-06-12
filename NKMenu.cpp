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
        nk_layout_row_dynamic(ctx, 25, 1);
        for (auto it = m_items.begin(); it != m_items.end(); ++it)
        {
            if (nk_menu_item_label(ctx, it->name, NK_TEXT_LEFT))
            {
                strcpy_s(m_functionName, it->data.name);
                strcpy_s(m_argsName, it->data.tableName);
                CallEvent(m_pManager);
            }
        }
        nk_menu_end(ctx);
    }
}

void NKMenu::EditInfo()
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

    nk_layout_row_dynamic(m_ctx, 33, 1);
    if (nk_button_label(m_ctx, "Add")) {
        MenuItem label;
        strcpy_s(label.name, "Item");
        strcpy_s(label.data.name, "None");
        strcpy_s(label.data.tableName, "None");

        m_items.push_back(label);
    }

    nk_layout_row_dynamic(m_ctx, 500, 1);
    if (nk_group_begin(m_ctx, "Menu Item List", NK_WINDOW_TITLE)) {
        for (auto it = m_items.begin(); it != m_items.end(); ++it)
        {
            MenuItem& item = *it;
            if (nk_tree_push_id(m_ctx, NK_TREE_TAB, it->name, NK_MINIMIZED, reinterpret_cast<intptr_t>(&item))) {
                float tree_layout[2] = { 0.f, };
                tree_layout[0] = 0.3f;
                tree_layout[1] = 0.7f;
                nk_layout_row(m_ctx, NK_DYNAMIC, 55, 2, tree_layout);

                nk_label(m_ctx, "name: ", NK_TEXT_LEFT);
                m_pManager->IMEInputSystem(m_ctx, NK_EDIT_FIELD | NK_EDIT_SIG_ENTER, item.name, sizeof(item.name), nk_filter_default, &item.nameLen);

                nk_label(m_ctx, "funcname: ", NK_TEXT_LEFT);
                m_pManager->IMEInputSystem(m_ctx, NK_EDIT_FIELD | NK_EDIT_SIG_ENTER, item.data.name, sizeof(item.data.name), nk_filter_default, &item.data.nameLen);

                nk_label(m_ctx, "table: ", NK_TEXT_LEFT);
                m_pManager->IMEInputSystem(m_ctx, NK_EDIT_FIELD | NK_EDIT_SIG_ENTER, item.data.tableName, sizeof(item.data.tableName), nk_filter_default, &item.data.tableLen);

                nk_tree_pop(m_ctx);
            }
        }
        nk_group_end(m_ctx);
    }
}

void NKMenu::SetLabel(const char* label)
{
    strcpy_s(m_label, label);
}