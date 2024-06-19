#pragma once
#ifndef NKScrollbar_h__
#define NKScrollbar_h__
#include "NKBase.h"
#include "NKStyleScrollbarH.h"
#include "NKStyleScrollbarV.h"
class NKScrollbar : public NKBase, public NKStyleScrollbarH, public NKStyleScrollbarV
{
public:
    NKScrollbar();
    NKScrollbar(nk_context* ctx, NuklearUI* pManager);
    NKScrollbar(const NKScrollbar& other);
    virtual ~NKScrollbar();

public:
    virtual void Layout(nk_context* ctx) override;
    virtual void EditStyle() override;
    virtual void EditComponentStyle(nk_context* ctx, NuklearUI* pManager) override;

    void SetScroll(float scroll);
    float GetScroll() const;

public:
    float m_scroll;

public:
    template <class Archive>
    void serialize(Archive& ar) {
        ar(cereal::base_class<NKBase>(this)
            , cereal::base_class<NKStyleScrollbarH>(this)
            , cereal::base_class<NKStyleScrollbarV>(this)
        );
    }
};
#endif //NKScrollbar_h__
