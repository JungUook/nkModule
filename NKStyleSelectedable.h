#pragma once
#ifndef NKStyleSelectedable_h__
#define NKStyleSelectedable_h__
#include "ComponentSelectable.h"

class NKStyleSelectedable
{
public:
	NKStyleSelectedable(nk_context* ctx, nk_style* style);
	~NKStyleSelectedable();

	virtual void EditComponentStyle(nk_context* ctx, NuklearUI* pManager);

protected:
	ComponentSelectable* m_pComponent;
};
#endif //NKStyleSelectedable_h__