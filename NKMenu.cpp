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
    m_cTransform.w = 150.f;
    m_cTransform.h = 40.f;
}

NKMenu::NKMenu(const NKMenu& other) : NKBase(other), NKHandler(other), NKBaseLabel(other), NKStyleMenuButton(other, m_ctx, &m_style)
{
    m_type = eMENU;
    m_items = other.m_items;
}

NKMenu::~NKMenu() {}

void NKMenu::LayoutBegin(nk_context* ctx)
{
    CustomFontSizeBegin(ctx, m_font);
}

void NKMenu::Layout(nk_context* ctx)
{
    if (nk_menu_begin_label(ctx, m_sContent.c_str(), NK_TEXT_LEFT, nk_vec2(120, 200)))
    {
        nk_layout_row_dynamic(ctx, 25, 1);
        for (auto it = m_items.begin(); it != m_items.end(); ++it)
        {
            if (nk_menu_item_label(ctx, it->name, NK_TEXT_LEFT))
            {
                m_functionName = it->functionName;
                m_argsName = it->argsName;
                CallEvent(m_pLuaManager);
            }
        }
        nk_menu_end(ctx);
    }
}

void NKMenu::LayoutEnd(nk_context* ctx)
{
    CustomFontSizeEnd(ctx, m_pManager, m_font);
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
    EditInfoData(ctx, m_pManager, m_pLuaManager);

    nk_layout_row_dynamic(ctx, 33, 1);
    if (nk_button_label(ctx, "Add")) {
        MenuItem label;
        strcpy_s(label.name, "Item");
        strcpy_s(label.functionName, "None");
        strcpy_s(label.argsName, "None");

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
                m_pManager->IMEInputSystem(ctx, item.functionName, sizeof(item.functionName), &item.functionNameLen);

                nk_label(ctx, "table: ", NK_TEXT_LEFT);
                m_pManager->IMEInputSystem(ctx, item.argsName, sizeof(item.argsName), &item.argsNameLen);

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

void NKMenu::RegistCommand(const char* classname)
{
    NKBase::RegistCommand(classname);
    MAKE_INTERFACE(m_mapFunc, this, NKMenu::CSetLabel, classname);
}
