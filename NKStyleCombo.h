#pragma once
#include "ComponentCombo.h"

class NKStyleCombo
{
public:
	NKStyleCombo();
	NKStyleCombo(nk_context* ctx, nk_style* style);
	NKStyleCombo(const NKStyleCombo& other);
	virtual ~NKStyleCombo();

	virtual void EditComponentStyle(nk_context* ctx, NuklearUI* pManager);

protected:
	ComponentCombo* m_pComponent;

public:
	template <class Archive>
	void serialize(Archive& ar) {
		ar(
			*m_pComponent
		);
	}
};

