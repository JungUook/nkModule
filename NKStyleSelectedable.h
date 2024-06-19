#pragma once
#ifndef NKStyleSelectedable_h__
#define NKStyleSelectedable_h__
#include "ComponentSelectable.h"

class NKStyleSelectedable
{
public:
	NKStyleSelectedable();
	NKStyleSelectedable(nk_context* ctx, nk_style* style);
	NKStyleSelectedable(const NKStyleSelectedable& other);
	virtual ~NKStyleSelectedable();

	virtual void EditComponentStyle(nk_context* ctx, NuklearUI* pManager);

protected:
	ComponentSelectable* m_pComponent;

public:
	template <class Archive>
	void serialize(Archive& ar) {
	}
};
#endif //NKStyleSelectedable_h__