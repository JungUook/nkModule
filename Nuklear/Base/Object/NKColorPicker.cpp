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

bool NKColorPicker::CSetColor(void* param)
{
    void** arr = static_cast<void**>(param);

    if (arr) {
        float* fArr = static_cast<float*>(*arr);

        if (fArr) {
            m_color.r = fArr[0];
            m_color.g = fArr[1];
            m_color.b = fArr[2];
            m_color.a = fArr[3];
            return true;
        }
    }

    return false;
}

struct nk_colorf NKColorPicker::GetColor() const
{
    return m_color;
}

bool NKColorPicker::CGetColor(void* param) const
{
    void** arr = static_cast<void**>(param);
    if (arr) {
        float* fArr = static_cast<float*>(*arr);

        if (fArr) {
            fArr[0] = m_color.r;
            fArr[1] = m_color.g;
            fArr[2] = m_color.b;
            fArr[3] = m_color.a;
            return true;
        }
    }
    return false;
}

void NKColorPicker::RegistCommand(const char* classname)
{
    NKBase::RegistCommand(classname);
    MAKE_INTERFACE(m_mapFunc, this, NKColorPicker::CSetColor, classname);
    MAKE_INTERFACE(m_mapFunc, this, NKColorPicker::CGetColor, classname);
}
