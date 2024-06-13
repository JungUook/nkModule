#pragma once
#ifndef NKStyleCheckbox_h__
#define NKStyleCheckbox_h__
#include "ComponentToggle.h"

class NKStyleCheckbox
{
public:
	NKStyleCheckbox(nk_context* ctx, nk_style* style);
	~NKStyleCheckbox();

	virtual void EditComponentStyle(nk_context* ctx, NuklearUI* pManager);

protected:
	ComponentToggle* m_pComponent;
};
#endif //NKStyleCheckbox_h__