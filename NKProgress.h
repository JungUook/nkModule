#pragma once
#ifndef NKProgress_h__
#define NKProgress_h__
#include "NKBase.h"
#include "NKStyleProgress.h"
class NKProgress : public NKBase, public NKStyleProgress
{
public:
    NKProgress();
    NKProgress(nk_context* ctx, NuklearUI* pManager);
    NKProgress(const NKProgress& other);
    virtual ~NKProgress();
    

public:
    virtual void Layout(nk_context* ctx) override;
    virtual void SafeRenderStart(nk_context* ctx) override;
    virtual void SafeRenderEnd(nk_context* ctx) override;
    virtual void EditInfo(nk_context* ctx) override;
    virtual void EditStyle(nk_context* ctx) override;
    void SetProgress(nk_size progress);
    nk_size GetProgress() const;

public:
    nk_size m_progress;

public:
    template <class Archive>
    void serialize(Archive& ar) {
        ar(cereal::base_class<NKBase>(this)
            , cereal::base_class<NKStyleProgress>(this)
            , m_progress
        );
    }
};
#endif //NKProgress_h__
