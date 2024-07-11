#pragma once
#ifndef NKStyleMenuButton_h__
#define NKStyleMenuButton_h__
#include "ComponentButton.h"
class NKStyleMenuButton
{
public:
	NKStyleMenuButton();
	NKStyleMenuButton(nk_context* ctx, nk_style* style);
	NKStyleMenuButton(const NKStyleMenuButton& other, nk_context* ctx, nk_style* style);
	virtual ~NKStyleMenuButton();

	virtual void UpdateComponent(nk_context* ctx, NuklearUI* pManager);
	virtual void EditComponentStyle(nk_context* ctx, NuklearUI* pManager);

protected:
	ComponentButton* m_pComponent;

public:
	template <class Archive>
	void serialize(Archive& ar, const unsigned int version) {
		ar(
			CEREAL_NVP(*m_pComponent)
		);
	}
};
#endif //NKStyleMenuButton_h__