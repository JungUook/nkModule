#pragma once
#ifndef NKStyleSelectedable_h__
#define NKStyleSelectedable_h__
#include "ComponentSelectable.h"

class NKStyleSelectedable
{
public:
	NKStyleSelectedable();
	NKStyleSelectedable(nk_context* ctx, nk_style* style);
	NKStyleSelectedable(const NKStyleSelectedable& other, nk_context* ctx, nk_style* style);
	virtual ~NKStyleSelectedable();

	virtual void UpdateComponent(nk_context* ctx, NuklearUI* pManager);
	virtual void EditComponentStyle(nk_context* ctx, NuklearUI* pManager);

protected:
	ComponentSelectable* m_pComponent;

public:
	template <class Archive>
	void serialize(Archive& ar, const unsigned int version) {
		ar(
			CEREAL_NVP(*m_pComponent)
		);
	}
};
#endif //NKStyleSelectedable_h__