#pragma once
#ifndef NKStyleCheckbox_h__
#define NKStyleCheckbox_h__
#include "ComponentToggle.h"

class NKStyleCheckbox
{
public:
	NKStyleCheckbox();
	NKStyleCheckbox(nk_context* ctx, nk_style* style);
	NKStyleCheckbox(const NKStyleCheckbox& other, nk_context* ctx, nk_style* style);
	virtual ~NKStyleCheckbox();

	virtual void UpdateComponent(nk_context* ctx, NuklearUI* pManager);
	virtual void EditComponentStyle(nk_context* ctx, NuklearUI* pManager);

protected:
	ComponentToggle* m_pComponent;

public:
	template <class Archive>
	void serialize(Archive& ar, const unsigned int version) {
		ar(
			CEREAL_NVP(*m_pComponent)
		);
	}
};
#endif //NKStyleCheckbox_h__