#pragma once
#ifndef NKPopup_h__
#define NKPopup_h__
#include "NKBase.h"
class NKPopup : public NKBase
{
public:
	NKPopup();
	~NKPopup();

public:
	void Layout(nk_context* ctx) override;

public:
	nk_popup_type m_popupType;
};


#endif //NKPopup_h__