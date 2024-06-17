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
    NKTooltip(nk_context* ctx, NuklearUI* pManager);
    NKTooltip(const NKTooltip& other);
    ~NKTooltip();

public:
    void Layout(nk_context* ctx) override;
    virtual void EditInfo() override;
    virtual void EditStyle() override;

    virtual void EditComponentStyle(nk_context* ctx, NuklearUI* pManager) override;
};
#endif //NKTooltip_h__
