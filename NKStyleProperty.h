#pragma once
#ifndef NKStyleProperty_h__
#define NKStyleProperty_h__
#include "ComponentProperty.h"

class NKStyleProperty
{
public:
	NKStyleProperty();
	NKStyleProperty(nk_context* ctx, nk_style* style);
	NKStyleProperty(const NKStyleProperty& other, nk_context* ctx, nk_style* style);
	virtual ~NKStyleProperty();

	virtual void UpdateComponent(nk_context* ctx, NuklearUI* pManager);
	virtual void EditComponentStyle(nk_context* ctx, NuklearUI* pManager);

protected:
	ComponentProperty* m_pComponent;

public:
	template <class Archive>
	void serialize(Archive& ar) {
		ar(
			*m_pComponent
		);
	}
};
#endif //NKStyleProperty_h__