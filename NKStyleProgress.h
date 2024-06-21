#pragma once
#ifndef NKStyleProgress_h__
#define NKStyleProgress_h__
#include "ComponentProgress.h"

class NKStyleProgress
{
public:
	NKStyleProgress();
	NKStyleProgress(nk_context* ctx, nk_style* style);
	NKStyleProgress(const NKStyleProgress& other, nk_context* ctx, nk_style* style);
	virtual ~NKStyleProgress();

	virtual void UpdateComponent(nk_context* ctx, NuklearUI* pManager);
	virtual void EditComponentStyle(nk_context* ctx, NuklearUI* pManager);

protected:
	ComponentProgress* m_pComponent;

public:
	template <class Archive>
	void serialize(Archive& ar) {
		ar(
			*m_pComponent
		);
	}
};
#endif //NKStyleProgress_h__