#pragma once
#ifndef NKStyleScrollbarH_h__
#define NKStyleScrollbarH_h__
#include "ComponentScrollbar.h"

class NKStyleScrollbarH
{
public:
	NKStyleScrollbarH();
	NKStyleScrollbarH(nk_context* ctx, nk_style* style);
	NKStyleScrollbarH(const NKStyleScrollbarH& other);
	virtual ~NKStyleScrollbarH();

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
#endif //NKStyleScrollbarH_h__