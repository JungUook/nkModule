#pragma once
#ifndef NKStyleScrollbarV_h__
#define NKStyleScrollbarV_h__
#include "ComponentScrollbar.h"

class NKStyleScrollbarV
{
public:
	NKStyleScrollbarV(nk_context* ctx, nk_style* style);
	NKStyleScrollbarV(const NKStyleScrollbarV& other);
	~NKStyleScrollbarV();

	virtual void EditComponentStyle(nk_context* ctx, NuklearUI* pManager);

protected:
	ComponentScrollbar* m_pComponent;
};
#endif //NKStyleScrollbarV_h__