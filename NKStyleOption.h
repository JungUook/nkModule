#pragma once
#ifndef NKStyleOption_h__
#define NKStyleOption_h__
#include "ComponentToggle.h"

class NKStyleOption
{
public:
	NKStyleOption();
	NKStyleOption(nk_context* ctx, nk_style* style);
	NKStyleOption(const NKStyleOption& other, nk_context* ctx, nk_style* style);
	virtual ~NKStyleOption();

	virtual void UpdateComponent(nk_context* ctx, NuklearUI* pManager);
	virtual void EditComponentStyle(nk_context* ctx, NuklearUI* pManager);

protected:
	ComponentToggle* m_pComponent;

public:
	template <class Archive>
	void serialize(Archive& ar) {
		ar(
			*m_pComponent
		);
	}
};
#endif //NKStyleOption_h__