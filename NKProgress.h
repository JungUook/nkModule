#pragma once
#ifndef NKProgress_h__
#define NKProgress_h__
#include "NKBase.h"
class NKProgress : public NKBase
{
public:
    NKProgress();
    ~NKProgress();

public:
    void Layout(nk_context* ctx) override;
    void SetProgress(nk_size progress);
    nk_size GetProgress() const;

public:
    nk_size m_progress;
};
#endif //NKProgress_h__
