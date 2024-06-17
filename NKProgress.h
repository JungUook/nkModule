#pragma once
#ifndef NKProgress_h__
#define NKProgress_h__
#include "NKBase.h"
#include "NKStyleProgress.h"
class NKProgress : public NKBase, public NKStyleProgress
{
public:
    NKProgress(nk_context* ctx, NuklearUI* pManager);
    NKProgress(const NKProgress& other);
    ~NKProgress();

public:
    virtual void Layout(nk_context* ctx) override;
    virtual void EditStyle() override;
    void SetProgress(nk_size progress);
    nk_size GetProgress() const;

public:
    nk_size m_progress;
};
#endif //NKProgress_h__
