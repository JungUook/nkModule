#include "pch.h"
#include "NKColorPicker.h"

NKColorPicker::NKColorPicker() : NKBase()
{
    m_type = eCOLOR_PICKER;
    m_color = nk_hsva_colorf(255, 255, 255, 255);
}

NKColorPicker::NKColorPicker(nk_context* ctx, NuklearUI* pManager) : NKBase(ctx, pManager)
{
    m_type = eCOLOR_PICKER;
    m_color = nk_hsva_colorf(255, 255, 255, 255);
}

NKColorPicker::~NKColorPicker() {}

void NKColorPicker::Layout(nk_context* ctx)
{
    nk_color_pick(ctx, &m_color, NK_RGBA);
}

void NKColorPicker::SafeRenderStart()
{
    //UpdateComponent(m_ctx, m_pManager);
}

void NKColorPicker::SafeRenderEnd()
{
}

void NKColorPicker::SetColor(struct nk_colorf color)
{
    m_color = color;
}

struct nk_colorf NKColorPicker::GetColor() const
{
    return m_color;
}
