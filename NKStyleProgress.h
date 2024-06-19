#pragma once
#ifndef NKStyleProgress_h__
#define NKStyleProgress_h__
#include "ComponentProgress.h"

class NKStyleProgress
{
public:
	NKStyleProgress();
	NKStyleProgress(nk_context* ctx, nk_style* style);
	NKStyleProgress(const NKStyleProgress& other);
	virtual ~NKStyleProgress();

	virtual void EditComponentStyle(nk_context* ctx, NuklearUI* pManager);

protected:
	ComponentProgress* m_pComponent;

public:
	template <class Archive>
	void serialize(Archive& ar) {
	}
};
#endif //NKStyleProgress_h__