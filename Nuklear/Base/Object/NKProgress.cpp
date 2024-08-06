#include "pch.h"
#include "NKProgress.h"

NKProgress::NKProgress() : NKBase(), NKStyleProgress()
{
    m_type = ePROGRESS;
    m_progress = 0;
}

NKProgress::NKProgress(nk_context* ctx, NuklearUI* pManager) : NKBase(ctx, pManager), NKStyleProgress(ctx, &m_style)
{
    m_type = ePROGRESS;
    m_progress = 0;
    m_cTransform.w = 150.f;
    m_cTransform.h = 40.f;
}

NKProgress::NKProgress(const NKProgress& other) : NKBase(other), NKStyleProgress(other, m_ctx, &m_style)
{
    m_type = other.m_type;
    m_progress = other.m_progress;
}

NKProgress::~NKProgress() {}

void NKProgress::Layout(nk_context* ctx)
{
    nk_progress(ctx, &m_progress, 100, NK_MODIFIABLE);
}

void NKProgress::SafeRenderStart(nk_context* ctx)
{
    UpdateComponent(ctx, m_pManager);
}

void NKProgress::SafeRenderEnd(nk_context* ctx)
{
}

void NKProgress::EditInfo(nk_context* ctx)
{
    nk_layout_row_dynamic(ctx, 44, 1);
    nk_progress(ctx, &m_progress, 100, NK_MODIFIABLE);
    int iproperty = m_progress;
    nk_property_int(ctx, "#Value", 0, &iproperty, 100, 1, 0.01f);
    m_progress = iproperty;
}

void NKProgress::EditStyle(nk_context* ctx)
{
    EditComponentStyle(ctx, m_pManager);
}

void NKProgress::SetProgress(nk_size progress)
{
    m_progress = progress;
}

void NKProgress::LSetProgress(luabridge::LuaRef ref)
{
    CHECK_LUA_REF(ref);
    nk_size progress = ref.cast<nk_size>();
    SetProgress(progress);
}

bool NKProgress::CSetProgress(void* param)
{
#ifdef _WIN64
    unsigned __int64* progress = static_cast<unsigned __int64*>(param);
#else
    unsigned int* progress = static_cast<unsigned int*>(param);;
#endif

    if (progress) {
        SetProgress(*progress);
        return true;
    }

    return false;
}

nk_size NKProgress::GetProgress() const
{
    return m_progress;
}

bool NKProgress::CGetProgress(void* param)
{
#ifdef _WIN64
    unsigned __int64* progress = static_cast<unsigned __int64*>(param);
#else
    unsigned int* progress = static_cast<unsigned int*>(param);;
#endif

    if (progress) {
        (*progress) = GetProgress();
        return true;
    }

    return false;
}

void NKProgress::RegistCommand(const char* classname)
{
    NKBase::RegistCommand(classname);
    MAKE_INTERFACE(m_mapFunc, this, NKProgress::CSetProgress, classname);
    MAKE_INTERFACE(m_mapFunc, this, NKProgress::CGetProgress, classname);
}
