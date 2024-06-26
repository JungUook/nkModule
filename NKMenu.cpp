#include "pch.h"
#include "NKMenu.h"

NKMenu::NKMenu() : NKBase(), NKHandler(), NKBaseLabel(), NKStyleMenuButton()
{
    m_type = eMENU;
}

NKMenu::NKMenu(nk_context* ctx, NuklearUI* pManager) : NKBase(ctx, pManager), NKHandler(), NKBaseLabel(), NKStyleMenuButton(ctx, &m_style)
{
    m_type = eMENU;
    SetLabel("Menu");
}

NKMenu::NKMenu(const NKMenu& other) : NKBase(other), NKHandler(other), NKBaseLabel(other), NKStyleMenuButton(other, m_ctx, &m_style)
{
    m_type = eMENU;
    m_items = other.m_items;
}

NKMenu::~NKMenu() {}

void NKMenu::Layout(nk_context* ctx)
{
    UpdateComponent(ctx, m_pManager);

    if (nk_menu_begin_label(ctx, m_cContent, NK_TEXT_LEFT, nk_vec2(120, 200)))
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

void NKMenu::SafeRenderStart(nk_context* ctx)
{
    UpdateComponent(ctx, m_pManager);
}

void NKMenu::SafeRenderEnd(nk_context* ctx)
{
}

void NKMenu::EditInfo(nk_context* ctx)
{
    EditLabel(ctx, m_pManager);

    nk_layout_row_dynamic(ctx, 33, 1);
    if (nk_button_label(ctx, "Add")) {
        MenuItem label;
        strcpy_s(label.name, "Item");
        strcpy_s(label.data.name, "None");
        strcpy_s(label.data.tableName, "None");

        m_items.push_back(label);
    }

    nk_layout_row_dynamic(ctx, 500, 1);
    if (nk_group_begin(ctx, "Menu Item List", NK_WINDOW_TITLE)) {
        for (auto it = m_items.begin(); it != m_items.end(); ++it)
        {
            MenuItem& item = *it;
            if (nk_tree_push_id(ctx, NK_TREE_TAB, it->name, NK_MINIMIZED, reinterpret_cast<intptr_t>(&item))) {
                float tree_layout[2] = { 0.f, };
                tree_layout[0] = 0.3f;
                tree_layout[1] = 0.7f;
                nk_layout_row(ctx, NK_DYNAMIC, 55, 2, tree_layout);

                nk_label(ctx, "name: ", NK_TEXT_LEFT);
                m_pManager->IMEInputSystem(ctx, item.name, sizeof(item.name), &item.nameLen);

                nk_label(ctx, "funcname: ", NK_TEXT_LEFT);
                m_pManager->IMEInputSystem(ctx, item.data.name, sizeof(item.data.name), &item.data.nameLen);

                nk_label(ctx, "table: ", NK_TEXT_LEFT);
                m_pManager->IMEInputSystem(ctx, item.data.tableName, sizeof(item.data.tableName), &item.data.tableLen);

                nk_tree_pop(ctx);
            }
        }
        nk_group_end(ctx);
    }
}

void NKMenu::EditStyle(nk_context* ctx)
{
    NKBase::EditStyle(ctx);
    EditComponentStyle(ctx, m_pManager);
}
