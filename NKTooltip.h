#pragma once
#ifndef NKTooltip_h__
#define NKTooltip_h__
#include "NKBase.h"
class NKTooltip : public NKBase
{
public:
    NKTooltip(nk_context* ctx, NuklearUI* pManager);
    ~NKTooltip();

public:
    void Layout(nk_context* ctx) override;
    void SetTooltip(const char* tooltip);

public:
    char m_tooltip[256];
};
#endif //NKTooltip_h__
