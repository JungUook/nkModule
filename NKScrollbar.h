#pragma once
#ifndef NKScrollbar_h__
#define NKScrollbar_h__
#include "NKBase.h"
#include "NKStyleScrollbarH.h"
#include "NKStyleScrollbarV.h"
class NKScrollbar : public NKBase, public NKStyleScrollbarH, public NKStyleScrollbarV
{
public:
    NKScrollbar(nk_context* ctx, NuklearUI* pManager);
    NKScrollbar(const NKScrollbar& other);
    ~NKScrollbar();

public:
    virtual void Layout(nk_context* ctx) override;
    virtual void EditStyle() override;
    virtual void EditComponentStyle(nk_context* ctx, NuklearUI* pManager) override;

    void SetScroll(float scroll);
    float GetScroll() const;

public:
    float m_scroll;
};
#endif //NKScrollbar_h__
