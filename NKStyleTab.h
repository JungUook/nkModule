#pragma once
#ifndef NKStyleTab_h__
#define NKStyleTab_h__
#include "ComponentTab.h"

class NKStyleTab
{
public:
	NKStyleTab(nk_context* ctx, nk_style* style);
	~NKStyleTab();

	virtual void EditComponentStyle(nk_context* ctx, NuklearUI* pManager);

protected:
	ComponentTab* m_pComponent;
};
#endif //NKStyleTab_h__