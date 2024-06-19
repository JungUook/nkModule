#pragma once
#ifndef NKPopup_h__
#define NKPopup_h__
#include "NKBase.h"
#include "NKBaseWindow.h"
#include "NKStyleHeader.h"
#include "NKStyleWindow.h"
class NKPopup : public NKBase, public NKBaseWindow, public NKStyleHeader, public NKStyleWindow
{
public:
	NKPopup();
	NKPopup(nk_context* ctx, NuklearUI* pManager);
	NKPopup(const NKPopup& other);
	virtual ~NKPopup();

public:
	virtual void Layout(nk_context* ctx) override;
	virtual void EditInfo() override;
	virtual void EditStyle() override;
	virtual void EditComponentStyle(nk_context* ctx, NuklearUI* pManager) override;

public:
	nk_popup_type m_popupType;

public:
	template <class Archive>
	void serialize(Archive& ar) {
		ar(cereal::base_class<NKBase>(this)
			, cereal::base_class<NKBaseWindow>(this)
			, cereal::base_class<NKStyleHeader>(this)
			, cereal::base_class<NKStyleWindow>(this)
		);
	}
};


#endif //NKPopup_h__