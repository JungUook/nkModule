#pragma once
#ifndef NKPopup_h__
#define NKPopup_h__
#include "NKBase.h"
#include "NKBaseWindow.h"
#include "NKHandler.h"
#include "NKStyleHeader.h"
#include "NKStyleWindow.h"
class NKPopup : public NKBase, public NKBaseWindow, public NKHandler, public NKStyleHeader, public NKStyleWindow
{
public:
	NKPopup();
	NKPopup(nk_context* ctx, NuklearUI* pManager);
	NKPopup(const NKPopup& other);
	virtual ~NKPopup();
	

public:
	virtual void Layout(nk_context* ctx) override;
	virtual void SafeRenderStart(nk_context* ctx) override;
	virtual void SafeRenderEnd(nk_context* ctx) override;
	virtual void EditInfo(nk_context* ctx) override;
	virtual void EditStyle(nk_context* ctx) override;
	virtual void UpdateComponent(nk_context* ctx, NuklearUI* pManager) override;
	virtual void EditComponentStyle(nk_context* ctx, NuklearUI* pManager) override;

	virtual void RegistCommand(const char* classname) override;
public:
	nk_popup_type m_popupType;

public:
	template <class Archive>
	void serialize(Archive& ar, const unsigned int version) {
		if (version >= 6) {
			ar(cereal::base_class<NKBase>(this)
				, cereal::base_class<NKBaseWindow>(this)
				, cereal::base_class<NKHandler>(this)
				, cereal::base_class<NKStyleHeader>(this)
				, cereal::base_class<NKStyleWindow>(this)
				, CEREAL_NVP(m_popupType)
			);
		}
		else {
			ar(cereal::base_class<NKBase>(this)
				, cereal::base_class<NKBaseWindow>(this)
				, cereal::base_class<NKStyleHeader>(this)
				, cereal::base_class<NKStyleWindow>(this)
				, CEREAL_NVP(m_popupType)
			);
		}
	}
};


#endif //NKPopup_h__