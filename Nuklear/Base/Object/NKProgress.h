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
    void LSetProgress(luabridge::LuaRef ref);
    bool CSetProgress(void* param);
    nk_size GetProgress() const;
    bool CGetProgress(void* param);

    virtual void RegistCommand(const char* classname) override;
public:
    nk_size m_progress;
    nk_modify m_modify;

public:
    template <class Archive>
    void serialize(Archive& ar, const unsigned int version) {
        if (version <= 13) {
            ar(cereal::base_class<NKBase>(this)
                , cereal::base_class<NKStyleProgress>(this)
                , CEREAL_NVP(m_progress)
                , CEREAL_NVP(m_modify)
            );
        }
        else {
            ar(cereal::base_class<NKBase>(this)
                , cereal::base_class<NKStyleProgress>(this)
                , CEREAL_NVP(m_progress)
            );
        }
    }
};
#endif //NKProgress_h__
