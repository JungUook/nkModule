#pragma once
#ifndef NKStyleOption_h__
#define NKStyleOption_h__
#include "ComponentToggle.h"

class NKStyleOption
{
public:
	NKStyleOption(nk_context* ctx, nk_style* style);
	NKStyleOption(const NKStyleOption& other);
	~NKStyleOption();

	virtual void EditComponentStyle(nk_context* ctx, NuklearUI* pManager);

protected:
	ComponentToggle* m_pComponent;
};
#endif //NKStyleOption_h__