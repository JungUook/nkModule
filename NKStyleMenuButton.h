#pragma once
#ifndef NKStyleMenuButton_h__
#define NKStyleMenuButton_h__
#include "ComponentButton.h"
class NKStyleMenuButton
{
public:
	NKStyleMenuButton(nk_context* ctx, nk_style* style);
	NKStyleMenuButton(const NKStyleMenuButton& other);
	~NKStyleMenuButton();

	virtual void EditComponentStyle(nk_context* ctx, NuklearUI* pManager);

protected:
	ComponentButton* m_pComponent;
};
#endif //NKStyleMenuButton_h__