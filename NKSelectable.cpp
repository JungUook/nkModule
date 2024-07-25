#include "pch.h"
#include "NKSelectable.h"

NKSelectable::NKSelectable() : NKBase(), NKHandler(), NKBaseLabel(), NKStyleSelectedable(), NKBaseImage()
{
    m_type = eSELECTABLE;
    m_selected = 0;
    m_iLeft = 0;
    m_iCenter = 0;
    m_iRight = 0;
    m_iTop = 0;
    m_iMiddle = 0;
    m_iBottom = 0;
    m_fLabelType = 0;

}

NKSelectable::NKSelectable(nk_context* ctx, NuklearUI* pManager) : NKBase(ctx, pManager), NKHandler(), NKBaseLabel(), NKStyleSelectedable(ctx, &m_style), NKBaseImage(pManager)
{
    m_type = eSELECTABLE;
    m_selected = 0;
    m_iLeft = 0;
    m_iCenter = 0;
    m_iRight = 0;
    m_iTop = 0;
    m_iMiddle = 0;
    m_iBottom = 0;
    m_fLabelType = 0;
    SetLabel("Selectable");
    m_cTransform.w = 150.f;
    m_cTransform.h = 40.f;
}

NKSelectable::NKSelectable(const NKSelectable& other) : NKBase(other), NKHandler(other), NKBaseLabel(other), NKStyleSelectedable(other, m_ctx, &m_style), NKBaseImage(other)
{
    m_type = other.m_type;
    m_selected = other.m_selected;
    m_iLeft = other.m_iLeft;
    m_iCenter = other.m_iCenter;
    m_iRight = other.m_iRight;
    m_iTop = other.m_iTop;
    m_iMiddle = other.m_iMiddle;
    m_iBottom = other.m_iBottom;
    m_fLabelType = other.m_fLabelType;
}

NKSelectable::~NKSelectable() {}

void NKSelectable::LayoutBegin(nk_context* ctx)
{
    CustomFontSizeBegin(ctx, m_font);
}

void NKSelectable::Layout(nk_context* ctx)
{
    UpdateComponent(ctx, m_pManager);


    m_fLabelType = 0;
    if (m_iLeft) {
        m_fLabelType |= NK_TEXT_ALIGN_LEFT;
    }
    if (m_iCenter) {
        m_fLabelType |= NK_TEXT_ALIGN_CENTERED;
    }
    if (m_iRight) {
        m_fLabelType |= NK_TEXT_ALIGN_RIGHT;
    }
    if (m_iTop) {
        m_fLabelType |= NK_TEXT_ALIGN_TOP;
    }
    if (m_iMiddle) {
        m_fLabelType |= NK_TEXT_ALIGN_MIDDLE;
    }
    if (m_iBottom) {
        m_fLabelType |= NK_TEXT_ALIGN_BOTTOM;
    }

    if (m_iSelectableType == eSELECTABLE_LABEL) {
        if (nk_selectable_label(ctx, m_sContent.c_str(), m_fLabelType, &m_selected)) {
            CallEvent(m_pLuaManager);
        }
    }
    else if (m_iSelectableType == eSELECTABLE_IMAGELABEL) {

        if (m_imagePath != "None") {
            if (m_sprSize > 0) {
                struct nk_image img;
                bool bResult = m_pManager->GetSprite(m_imagePath.c_str(), m_sprIndex, img);
                if (!bResult) {
#ifdef _NKDEBUG
                    m_pManager->ErrorPopup("Image URL not linked to the editor.");
#endif
                    m_imagePath = "None";
                    return;
                }
                else {
                    if (nk_selectable_image_label(ctx, img, m_sContent.c_str(), m_fLabelType, &m_selected)) {
                        CallEvent(m_pLuaManager);
                    }
                }
            }
            else {
                struct nk_image img;
                bool bResult = m_pManager->GetImage(m_imagePath.c_str(), img);
                if (!bResult) {
#ifdef _NKDEBUG
                    m_pManager->ErrorPopup("Image URL not linked to the editor.");
#endif
                    m_imagePath = "None";
                    return;
                }
                else {
                    if (nk_selectable_image_label(ctx, img, m_sContent.c_str(), m_fLabelType, &m_selected)) {
                        CallEvent(m_pLuaManager);
                    }
                }
            }
        }
    }
}

void NKSelectable::LayoutEnd(nk_context* ctx)
{
    CustomFontSizeEnd(ctx, m_pManager, m_font);
}

void NKSelectable::SafeRenderStart(nk_context* ctx)
{
    UpdateComponent(ctx, m_pManager);
}

void NKSelectable::SafeRenderEnd(nk_context* ctx)
{
}

void NKSelectable::EditInfo(nk_context* ctx)
{
    EditLabel(ctx, m_pManager);

    nk_layout_row_dynamic(ctx, 22, 1);
    nk_label(ctx, "Selectable Type", NK_TEXT_LEFT);
    nk_layout_row_dynamic(ctx, 22, 2);
    if (nk_option_label(ctx, "Label", m_iSelectableType == eSELECTABLE_LABEL)) m_iSelectableType = eSELECTABLE_LABEL;
    if (nk_option_label(ctx, "Image", m_iSelectableType == eSELECTABLE_IMAGELABEL)) m_iSelectableType = eSELECTABLE_IMAGELABEL;

    if (nk_tree_push_id(ctx, NK_TREE_NODE, "Label Type", NK_MINIMIZED, reinterpret_cast<intptr_t>(this))) {
        nk_selectable_label(ctx, "Left", NK_TEXT_LEFT, &m_iLeft);
        nk_selectable_label(ctx, "Center", NK_TEXT_LEFT, &m_iCenter);
        nk_selectable_label(ctx, "Right", NK_TEXT_LEFT, &m_iRight);
        nk_selectable_label(ctx, "Top", NK_TEXT_LEFT, &m_iTop);
        nk_selectable_label(ctx, "Middle", NK_TEXT_LEFT, &m_iMiddle);
        nk_selectable_label(ctx, "Bottom", NK_TEXT_LEFT, &m_iBottom);
        nk_tree_pop(ctx);
    }

    if (m_iSelectableType == eSELECTABLE_IMAGELABEL) {
        int size = m_mapSpr->size();

        if (m_sprSize > 0)
        {
            nk_layout_row_dynamic(ctx, 22, 1);
            nk_property_int(ctx, "#Index:", 0, &m_sprIndex, m_sprSize - 1, 1, 1);
        }

        nk_layout_row_dynamic(ctx, 22, 2);
        nk_label(ctx, "Selected:", NK_TEXT_LEFT);
        std::filesystem::path filePath(m_imagePath.c_str());
        nk_label(ctx, filePath.filename().string().c_str(), NK_TEXT_RIGHT);
        if (nk_button_label(ctx, "apply"))
        {
            struct nk_image img;
            m_pManager->GetSprite(m_imagePath.c_str(), m_sprIndex, img, true);
        }
        if (nk_button_label(ctx, "clear"))
        {
            m_imagePath = "None";
            m_sprIndex = 0;
            m_sprSize = 0;
        }

        if (size > 0)
        {
            nk_layout_row_dynamic(ctx, 300, 1);
            if (nk_group_begin(ctx, "SPR List", NK_WINDOW_TITLE)) {

                float ratio[2] = { 0.8f, 0.2f };
                nk_layout_row(ctx, NK_DYNAMIC, 22, 2, ratio);
                int selected = 0;

                for (auto it = m_mapSpr->begin(); it != m_mapSpr->end(); ++it) {
                    std::filesystem::path filePath((*it).first.c_str());
                    nk_label(ctx, filePath.filename().string().c_str(), NK_TEXT_LEFT);

                    if (nk_button_label(ctx, "Load")) {
                        m_imagePath = (*it).first;
                        m_sprSize = ((*it).second)->GetSpr()->GetXCount() * ((*it).second)->GetSpr()->GetYCount();
                        if (m_sprSize <= m_sprIndex) {
                            m_sprIndex = 0;
                        }
                    }
                }
                nk_group_end(ctx);
            }
        }
    }
}

void NKSelectable::EditStyle(nk_context* ctx)
{
    EditComponentStyle(ctx, m_pManager);
}

void NKSelectable::SetLabel(const char* text)
{
    if (strlen(text) <= 0) {
#ifdef _NKDEBUG
        m_pManager->ErrorPopup("A selectable must have a string.");
#endif
        return;
    }

    NKBaseLabel::SetLabel(text);
}

void NKSelectable::SetSelected(bool selected)
{
    m_selected = selected ? 1 : 0;
}

void NKSelectable::LSetSelected(luabridge::LuaRef ref)
{
    CHECK_LUA_REF(ref);
    bool bSelected = ref.cast<bool>();
    SetSelected(bSelected);
}

bool NKSelectable::CSetSelected(void* param)
{
    bool* selected = static_cast<bool*>(param);

    if (selected) {
        SetSelected(*selected);
        return true;
    }

    return false;
}

bool NKSelectable::IsSelected() const
{
    return m_selected != 0;
}

bool NKSelectable::CIsSelected(void* param) const
{
    bool* selected = static_cast<bool*>(param);
    if (selected) {
        (*selected) = IsSelected();
        return true;
    }

    return false;
}

void NKSelectable::RegistCommand(const char* classname)
{
    NKBase::RegistCommand(classname);
    MAKE_INTERFACE(m_mapFunc, this, NKSelectable::CSetLabel, classname);
    MAKE_INTERFACE(m_mapFunc, this, NKSelectable::CSetSelected, classname);
    MAKE_INTERFACE(m_mapFunc, this, NKSelectable::CIsSelected, classname);
    MAKE_INTERFACE(m_mapFunc, this, NKSelectable::CSetImagePath, classname);
    MAKE_INTERFACE(m_mapFunc, this, NKSelectable::CSetIndex, classname);
}
