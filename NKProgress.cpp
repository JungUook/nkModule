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
}

NKProgress::NKProgress(const NKProgress& other) : NKBase(other), NKStyleProgress(other, m_ctx, &m_style)
{
    m_type = other.m_type;
    m_progress = other.m_progress;
}

NKProgress::~NKProgress() {}

void NKProgress::Layout(nk_context* ctx)
{
    UpdateComponent(ctx, m_pManager);

    nk_progress(ctx, &m_progress, 100, NK_MODIFIABLE);
}

void NKProgress::SafeRenderStart()
{
    UpdateComponent(m_ctx, m_pManager);
}

void NKProgress::SafeRenderEnd()
{
}

void NKProgress::EditStyle()
{
    EditComponentStyle(m_ctx, m_pManager);
}

void NKProgress::SetProgress(nk_size progress)
{
    m_progress = progress;
}

nk_size NKProgress::GetProgress() const
{
    return m_progress;
}
