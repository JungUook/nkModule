#pragma once
#ifndef NKStyleScrollbarH_h__
#define NKStyleScrollbarH_h__
#include "ComponentScrollbar.h"

class NKStyleScrollbarH
{
public:
	NKStyleScrollbarH(nk_context* ctx, nk_style* style);
	~NKStyleScrollbarH();

	virtual void EditComponentStyle(nk_context* ctx, NuklearUI* pManager);

protected:
	ComponentScrollbar* m_pComponent;
};
#endif //NKStyleScrollbarH_h__