#pragma once
#ifndef NKStyleScrollbarV_h__
#define NKStyleScrollbarV_h__
#include "ComponentScrollbar.h"

class NKStyleScrollbarV
{
public:
	NKStyleScrollbarV();
	NKStyleScrollbarV(nk_context* ctx, nk_style* style);
	NKStyleScrollbarV(const NKStyleScrollbarV& other, nk_context* ctx, nk_style* style);
	virtual ~NKStyleScrollbarV();

	virtual void UpdateComponent(nk_context* ctx, NuklearUI* pManager);
	virtual void EditComponentStyle(nk_context* ctx, NuklearUI* pManager);

protected:
	ComponentScrollbar* m_pComponent;

public:
	template <class Archive>
	void serialize(Archive& ar) {
		ar(
			*m_pComponent
		);
	}
};
#endif //NKStyleScrollbarV_h__