#pragma once
#ifndef NKStyleScrollbarH_h__
#define NKStyleScrollbarH_h__
#include "ComponentScrollbar.h"

class NKStyleScrollbarH
{
public:
	NKStyleScrollbarH();
	NKStyleScrollbarH(nk_context* ctx, nk_style* style);
	NKStyleScrollbarH(const NKStyleScrollbarH& other, nk_context* ctx, nk_style* style);
	virtual ~NKStyleScrollbarH();

	virtual void UpdateComponent(nk_context* ctx, NuklearUI* pManager);
	virtual void EditComponentStyle(nk_context* ctx, NuklearUI* pManager);

protected:
	ComponentScrollbar* m_pComponent;

public:
	template <class Archive>
	void serialize(Archive& ar, const unsigned int version) {
		ar(
			CEREAL_NVP(*m_pComponent)
		);
	}
};
#endif //NKStyleScrollbarH_h__