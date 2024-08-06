#pragma once
#ifndef NKStyleHeader_h__
#define NKStyleHeader_h__
#include "ComponentHeader.h"

class NKStyleHeader
{
public:
	NKStyleHeader();
	NKStyleHeader(nk_context* ctx, nk_style* style);
	NKStyleHeader(const NKStyleHeader& other, nk_context* ctx, nk_style* style);
	virtual ~NKStyleHeader();

	virtual void UpdateComponent(nk_context* ctx, NuklearUI* pManager);
	virtual void EditComponentStyle(nk_context* ctx, NuklearUI* pManager);

protected:
	ComponentHeader* m_pComponent;

public:
	template <class Archive>
	void serialize(Archive& ar, const unsigned int version) {
		ar(
			CEREAL_NVP(*m_pComponent)
		);
	}
};
#endif //NKStyleHeader_h__