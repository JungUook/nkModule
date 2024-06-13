#pragma once
#include "ComponentCombo.h"

class NKStyleCombo
{
public:
	NKStyleCombo(nk_context* ctx, nk_style* style);
	~NKStyleCombo();

	virtual void EditComponentStyle(nk_context* ctx, NuklearUI* pManager);

protected:
	ComponentCombo* m_pComponent;
};

