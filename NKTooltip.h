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
    enum {
        eTOOLTIP_STATIC,
        eTOOLTIP_DYNAMIC,
    };
    enum {
        eTOOLTIP_SIMPLE,
        eTOOLTIP_DETAIL,
    };

public:
    NKTooltip();
    NKTooltip(nk_context* ctx, NuklearUI* pManager);
    NKTooltip(const NKTooltip& other);
    virtual ~NKTooltip();    

public:
    virtual void LayoutBegin(nk_context* ctx) override;
    virtual void Layout(nk_context* ctx) override;
    virtual void LayoutEnd(nk_context* ctx) override;
    virtual void SafeRenderStart(nk_context* ctx) override;
    virtual void SafeRenderEnd(nk_context* ctx) override;
    virtual void EditInfo(nk_context* ctx) override;
    virtual void EditStyle(nk_context* ctx) override;

    virtual void UpdateComponent(nk_context* ctx, NuklearUI* pManager) override;
    virtual void EditComponentStyle(nk_context* ctx, NuklearUI* pManager) override;

    virtual void RegistCommand(const char* classname) override;

public:
    int m_iTooltipType;
    int m_iDetailType;

public:
    template <class Archive>
	void serialize(Archive& ar, const unsigned int version) {
		ar(cereal::base_class<NKBase>(this)
			, cereal::base_class<NKBaseLabel>(this)
			, cereal::base_class<NKObjectFinder>(this)
			, cereal::base_class<NKStyleWindow>(this)
			, cereal::base_class<NKStyleText>(this)
			, CEREAL_NVP(m_iTooltipType)
            , CEREAL_NVP(m_iDetailType)
		);
	}
};
#endif //NKTooltip_h__
