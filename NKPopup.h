#pragma once
#ifndef NKPopup_h__
#define NKPopup_h__
#include "NKBase.h"
#include "NKBaseWindow.h"
class NKPopup : public NKBase, public NKBaseWindow
{
public:
	NKPopup(nk_context* ctx, NuklearUI* pManager);
	~NKPopup();

public:
	virtual void Layout(nk_context* ctx) override;
	virtual void EditInfo() override;
	virtual void EditStyle() override;

public:
	nk_popup_type m_popupType;
};


#endif //NKPopup_h__