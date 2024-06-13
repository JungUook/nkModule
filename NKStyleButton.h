#pragma once
#ifndef NKStyleButton_h__
#define NKStyleButton_h__
#include "ComponentButton.h"
class NKStyleButton
{
public:
	NKStyleButton(nk_context* ctx, nk_style* style);
	~NKStyleButton();

	virtual void EditComponentStyle(nk_context* ctx, NuklearUI* pManager);

protected:
	ComponentButton* m_pComponent;
};
#endif //NKStyleButton_h__