#pragma once
#ifndef NKStyleButton_h__
#define NKStyleButton_h__
#include "ComponentButton.h"
class NKStyleButton
{
public:
	NKStyleButton();
	NKStyleButton(nk_context* ctx, nk_style* style);
	NKStyleButton(const NKStyleButton& other);
	virtual ~NKStyleButton();

	virtual void EditComponentStyle(nk_context* ctx, NuklearUI* pManager);

protected:
	ComponentButton* m_pComponent;

public:
	template <class Archive>
	void serialize(Archive& ar) {
	}
};
#endif //NKStyleButton_h__