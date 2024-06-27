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
    m_cTransform.x = 0.f;
    m_cTransform.y = 0.f;
    m_cTransform.w = 150.f;
    m_cTransform.h = 150.f;
}

NKColorPicker::NKColorPicker(const NKColorPicker& other)
{
    m_type = other.m_type;
    m_color = other.m_color;
}

NKColorPicker::~NKColorPicker() {}

void NKColorPicker::Layout(nk_context* ctx)
{
    nk_color_pick(ctx, &m_color, NK_RGBA);
}

void NKColorPicker::SafeRenderStart(nk_context* ctx)
{
    //UpdateComponent(ctx, m_pManager);
}

void NKColorPicker::SafeRenderEnd(nk_context* ctx)
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
