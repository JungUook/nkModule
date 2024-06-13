#include "pch.h"
#include "NKProgress.h"

NKProgress::NKProgress(nk_context* ctx, NuklearUI* pManager) : NKBase(ctx, pManager)
{
    m_type = ePROGRESS;
    m_progress = 0;
}

NKProgress::~NKProgress() {}

void NKProgress::Layout(nk_context* ctx)
{
    nk_progress(ctx, &m_progress, 100, NK_MODIFIABLE);
}

void NKProgress::SetProgress(nk_size progress)
{
    m_progress = progress;
}

nk_size NKProgress::GetProgress() const
{
    return m_progress;
}
