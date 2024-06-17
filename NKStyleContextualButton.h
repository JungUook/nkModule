#pragma once
#ifndef NKStyleContextualButton_h__
#define NKStyleContextualButton_h__
#include "ComponentButton.h"
class NKStyleContextualButton
{
public:
	NKStyleContextualButton(nk_context* ctx, nk_style* style);
	NKStyleContextualButton(const NKStyleContextualButton& other);
	~NKStyleContextualButton();

	virtual void EditComponentStyle(nk_context* ctx, NuklearUI* pManager);

protected:
	ComponentButton* m_pComponent;
};
#endif //NKStyleContextualButton_h__