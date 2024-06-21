#pragma once
#ifndef NKStyleContextualButton_h__
#define NKStyleContextualButton_h__
#include "ComponentButton.h"
class NKStyleContextualButton
{
public:
	NKStyleContextualButton();
	NKStyleContextualButton(nk_context* ctx, nk_style* style);
	NKStyleContextualButton(const NKStyleContextualButton& other, nk_context* ctx, nk_style* style);
	virtual ~NKStyleContextualButton();

	virtual void UpdateComponent(nk_context* ctx, NuklearUI* pManager);
	virtual void EditComponentStyle(nk_context* ctx, NuklearUI* pManager);

protected:
	ComponentButton* m_pComponent;

public:
	template <class Archive>
	void serialize(Archive& ar) {
		ar(
			*m_pComponent
		);
	}
};
#endif //NKStyleContextualButton_h__