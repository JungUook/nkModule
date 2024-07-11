#pragma once
#include "ComponentCombo.h"

class NKStyleCombo
{
public:
	NKStyleCombo();
	NKStyleCombo(nk_context* ctx, nk_style* style);
	NKStyleCombo(const NKStyleCombo& other, nk_context* ctx, nk_style* style);
	virtual ~NKStyleCombo();

	virtual void UpdateComponent(nk_context* ctx, NuklearUI* pManager);
	virtual void EditComponentStyle(nk_context* ctx, NuklearUI* pManager);

protected:
	ComponentCombo* m_pComponent;

public:
	template <class Archive>
	void serialize(Archive& ar, const unsigned int version) {
		ar(
			CEREAL_NVP(*m_pComponent)
		);
	}
};

