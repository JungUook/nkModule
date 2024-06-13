#pragma once
#ifndef NKStyleProgress_h__
#define NKStyleProgress_h__
#include "ComponentProgress.h"

class NKStyleProgress
{
public:
	NKStyleProgress(nk_context* ctx, nk_style* style);
	~NKStyleProgress();

	virtual void EditComponentStyle(nk_context* ctx, NuklearUI* pManager);

protected:
	ComponentProgress* m_pComponent;
};
#endif //NKStyleProgress_h__