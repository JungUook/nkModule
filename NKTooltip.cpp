#include "pch.h"
#include "NKTooltip.h"

NKTooltip::NKTooltip() : NKBase(), NKBaseLabel(), NKObjectFinder(), NKStyleWindow(), NKStyleText()
{
    m_type = eTOOLTIP;
    m_iTooltipType = eTOOLTIP_STATIC;
}

NKTooltip::NKTooltip(nk_context* ctx, NuklearUI* pManager) : NKBase(ctx, pManager), NKBaseLabel(), NKObjectFinder(pManager), NKStyleWindow(ctx, &m_style), NKStyleText(ctx, &m_style)
{
    m_type = eTOOLTIP;
    m_iTooltipType = eTOOLTIP_STATIC;
    SetLabel("Tooltip");
    m_cTransform.w = 150.f;
    m_cTransform.h = 40.f;
}

NKTooltip::NKTooltip(const NKTooltip& other) : NKBase(other), NKBaseLabel(other), NKObjectFinder(other), NKStyleWindow(other, m_ctx, &m_style), NKStyleText(other, m_ctx, &m_style)
{
    m_type = other.m_type;
    m_iTooltipType = other.m_iTooltipType;
}

NKTooltip::~NKTooltip() {}

void NKTooltip::LayoutBegin(nk_context* ctx)
{
    CustomFontSizeBegin(ctx, m_font);
}

void NKTooltip::Layout(nk_context* ctx)
{
    const float SCREEN_WIDTH = m_pManager->GetViewport()->w;
    const float SCREEN_HEIGHT = m_pManager->GetViewport()->h;

    const float mouseX = ctx->input.mouse.pos.x;
    const float mouseY = ctx->input.mouse.pos.y;

    if (mouseX > SCREEN_WIDTH / 2 && mouseY > SCREEN_HEIGHT / 2) {
        // 우측 하단 -> 좌측 상단
        ctx->input.mouse.pos.x += -m_cTransform.x - m_cTransform.w;
        ctx->input.mouse.pos.y += -m_cTransform.y - m_cTransform.h;
    }
    else if (mouseX > SCREEN_WIDTH / 2 && mouseY <= SCREEN_HEIGHT / 2) {
        // 우측 상단 -> 좌측 하단
        ctx->input.mouse.pos.x += -m_cTransform.x - m_cTransform.w;
        ctx->input.mouse.pos.y += m_cTransform.y;
    }
    else if (mouseX <= SCREEN_WIDTH / 2 && mouseY > SCREEN_HEIGHT / 2) {
        // 좌측 하단 -> 우측 상단
        ctx->input.mouse.pos.x += m_cTransform.x;
        ctx->input.mouse.pos.y += -m_cTransform.y - m_cTransform.h;
    }
    else {
        // 좌측 상단 -> 우측 하단
        ctx->input.mouse.pos.x += m_cTransform.x;
        ctx->input.mouse.pos.y += m_cTransform.y;
    }

    if (m_iDetailType == eTOOLTIP_STATIC) {
        if (m_pResultObject && m_pResultObject->IsHovering() && nk_tooltip_begin(ctx, GetWidth()))
        {
            if (m_iTooltipType == eTOOLTIP_SIMPLE) {
                nk_layout_row_dynamic(ctx, 22, 1);
                nk_label(ctx, m_sContent.c_str(), NK_TEXT_CENTERED);
            }
            else if (m_iTooltipType == eTOOLTIP_DETAIL){
                for (std::list<NKBase*>::iterator it = m_pChildList.begin(); it != m_pChildList.end(); ++it)
                {
                    (*it)->CheckMouseHover(ctx);
                    (*it)->Update(ctx);
                }
            }
            nk_tooltip_end(ctx);
        }
    }
    else if (m_iDetailType == eTOOLTIP_DYNAMIC) {
        struct nk_rect label_bounds = nk_layout_widget_bounds(ctx);
        if (nk_input_is_mouse_hovering_rect(&ctx->input, label_bounds)) {
            const struct nk_style* style;
            struct nk_vec2 padding;

            float text_width;
            float text_height;

            style = &ctx->style;
            padding = style->window.padding;

            text_width = style->font->width(style->font->userdata,
                style->font->height, m_sContent.c_str(), m_sContent.length());
            text_width += (4 * padding.x);
            text_height = (style->font->height + 2 * padding.y);

            if (m_iTooltipType == eTOOLTIP_SIMPLE) {
                if (nk_tooltip_begin(ctx, (float)text_width)) {
                    nk_layout_row_dynamic(ctx, (float)text_height, 1);
                    nk_text(ctx, m_sContent.c_str(), m_sContent.length(), NK_TEXT_LEFT);
                    nk_tooltip_end(ctx);
                }
            }
			else if (m_iTooltipType == eTOOLTIP_DETAIL) {
				if (nk_tooltip_begin(ctx, m_cTransform.w)) {
					for (std::list<NKBase*>::iterator it = m_pChildList.begin(); it != m_pChildList.end(); ++it)
					{
						(*it)->CheckMouseHover(ctx);
						(*it)->Update(ctx);
					}
					nk_tooltip_end(ctx);
				}
			}
        }
    }


    if (mouseX > SCREEN_WIDTH / 2 && mouseY > SCREEN_HEIGHT / 2) {
        // 우측 하단 -> 좌측 상단
        ctx->input.mouse.pos.x -= -m_cTransform.x - m_cTransform.w;
        ctx->input.mouse.pos.y -= m_cTransform.y - m_cTransform.h;
    }
    else if (mouseX > SCREEN_WIDTH / 2 && mouseY <= SCREEN_HEIGHT / 2) {
        // 우측 상단 -> 좌측 하단
        ctx->input.mouse.pos.x -= -m_cTransform.x - m_cTransform.w;
        ctx->input.mouse.pos.y -= m_cTransform.y;
    }
    else if (mouseX <= SCREEN_WIDTH / 2 && mouseY > SCREEN_HEIGHT / 2) {
        // 좌측 하단 -> 우측 상단
        ctx->input.mouse.pos.x -= m_cTransform.x;
        ctx->input.mouse.pos.y -= -m_cTransform.y - m_cTransform.h;
    }
    else {
        // 좌측 상단 -> 우측 하단
        ctx->input.mouse.pos.x -= m_cTransform.x;
        ctx->input.mouse.pos.y -= m_cTransform.y;
    }
}

void NKTooltip::LayoutEnd(nk_context* ctx)
{
    CustomFontSizeEnd(ctx, m_pManager, m_font);
}

void NKTooltip::SafeRenderStart(nk_context* ctx)
{
    UpdateComponent(ctx, m_pManager);
}

void NKTooltip::SafeRenderEnd(nk_context* ctx)
{
}

void NKTooltip::EditInfo(nk_context* ctx)
{
    nk_layout_row_dynamic(ctx, 22, 1);
    nk_label(ctx, "Tooltip Type", NK_TEXT_LEFT);
    nk_layout_row_dynamic(ctx, 22, 2);
    if (nk_option_label(ctx, "Simple", m_iTooltipType == eTOOLTIP_SIMPLE)) m_iTooltipType = eTOOLTIP_SIMPLE;
    if (nk_option_label(ctx, "Detail", m_iTooltipType == eTOOLTIP_DETAIL)) m_iTooltipType = eTOOLTIP_DETAIL;


    if (m_iTooltipType == eTOOLTIP_SIMPLE) {
        EditLabel(ctx, m_pManager);
    }
    else if (m_iTooltipType == eTOOLTIP_DETAIL) {
        if (nk_tree_push(ctx, NK_TREE_NODE, "Create UI", NK_MINIMIZED)) {
            if (nk_button_label(ctx, "Space"))
            {
                CreateUI("NKSpace");
            }
            nk_tree_pop(ctx);
        }
    }


    nk_layout_row_dynamic(ctx, 22, 1);
    nk_label(ctx, "Space Type", NK_TEXT_LEFT);
    nk_layout_row_dynamic(ctx, 22, 2);
    if (nk_option_label(ctx, "STATIC", m_iDetailType == eTOOLTIP_STATIC)) m_iDetailType = eTOOLTIP_STATIC;
    if (nk_option_label(ctx, "DYNAMIC", m_iDetailType == eTOOLTIP_DYNAMIC)) m_iDetailType = eTOOLTIP_DYNAMIC;

    if (m_iDetailType == eTOOLTIP_STATIC) {
        FoundObject(ctx, m_pManager);
        SearchObject(ctx, m_pManager);
    }
}

void NKTooltip::EditStyle(nk_context* ctx)
{
    NKBase::EditStyle(ctx);
    EditComponentStyle(ctx, m_pManager);
}

void NKTooltip::UpdateComponent(nk_context* ctx, NuklearUI* pManager)
{
    NKStyleWindow::UpdateComponent(ctx, pManager);
    NKStyleText::UpdateComponent(ctx, pManager);
}

void NKTooltip::EditComponentStyle(nk_context* ctx, NuklearUI* pManager)
{
    NKStyleWindow::EditComponentStyle(ctx, pManager);
    NKStyleText::EditComponentStyle(ctx, pManager);
}

void NKTooltip::RegistCommand(const char* classname)
{
    NKBase::RegistCommand(classname);
}
