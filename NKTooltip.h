#pragma once
#ifndef NKTooltip_h__
#define NKTooltip_h__
#include "NKBase.h"
#include "NKBaseLabel.h"
#include "NKObjectFinder.h"
#include "NKStyleWindow.h"
#include "NKStyleText.h"
class NKTooltip : public NKBase, public NKBaseLabel, public NKObjectFinder, public NKStyleWindow, public NKStyleText
{
public:
    NKTooltip();
    NKTooltip(nk_context* ctx, NuklearUI* pManager);
    NKTooltip(const NKTooltip& other);
    virtual ~NKTooltip();    

public:
    void Layout(nk_context* ctx) override;
    virtual void SafeRenderStart() override;
    virtual void SafeRenderEnd() override;
    virtual void EditInfo() override;
    virtual void EditStyle() override;

    virtual void UpdateComponent(nk_context* ctx, NuklearUI* pManager) override;
    virtual void EditComponentStyle(nk_context* ctx, NuklearUI* pManager) override;

public:
    template <class Archive>
    void serialize(Archive& ar) {
        ar(cereal::base_class<NKBase>(this)
            , cereal::base_class<NKBaseLabel>(this)
            , cereal::base_class<NKObjectFinder>(this)
            , cereal::base_class<NKStyleWindow>(this)
            , cereal::base_class<NKStyleText>(this)
        );
    }
};
#endif //NKTooltip_h__
