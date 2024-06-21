#pragma once
#ifndef NKStyleButton_h__
#define NKStyleButton_h__
#include "ComponentButton.h"
class NKStyleButton
{
public:
	NKStyleButton();
	NKStyleButton(nk_context* ctx, nk_style* style);
	NKStyleButton(const NKStyleButton& other, nk_context* ctx, nk_style* style);
	virtual ~NKStyleButton();

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
#endif //NKStyleButton_h__