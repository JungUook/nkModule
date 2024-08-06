#pragma once
#ifndef NKStyleSlider_h__
#define NKStyleSlider_h__
#include "ComponentSlider.h"

class NKStyleSlider
{
public:
	NKStyleSlider();
	NKStyleSlider(nk_context* ctx, nk_style* style);
	NKStyleSlider(const NKStyleSlider& other, nk_context* ctx, nk_style* style);
	virtual ~NKStyleSlider();

	virtual void UpdateComponent(nk_context* ctx, NuklearUI* pManager);
	virtual void EditComponentStyle(nk_context* ctx, NuklearUI* pManager);

protected:
	ComponentSlider* m_pComponent;

public:
	template <class Archive>
	void serialize(Archive& ar, const unsigned int version) {
		ar(
			CEREAL_NVP(*m_pComponent)
		);
	}
};
#endif //NKStyleSlider_h__