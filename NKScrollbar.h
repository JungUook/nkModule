#pragma once
#ifndef NKScrollbar_h__
#define NKScrollbar_h__
#include "NKBase.h"
class NKScrollbar : public NKBase
{
public:
    NKScrollbar();
    ~NKScrollbar();

public:
    void Layout(nk_context* ctx) override;
    void SetScroll(float scroll);
    float GetScroll() const;

public:
    float m_scroll;
};
#endif //NKScrollbar_h__
