#pragma once
#ifndef NKStyleCheckbox_h__
#define NKStyleCheckbox_h__
#include "ComponentToggle.h"

class NKStyleCheckbox
{
public:
	NKStyleCheckbox();
	NKStyleCheckbox(nk_context* ctx, nk_style* style);
	NKStyleCheckbox(const NKStyleCheckbox& other);
	virtual ~NKStyleCheckbox();

	virtual void EditComponentStyle(nk_context* ctx, NuklearUI* pManager);

protected:
	ComponentToggle* m_pComponent;

public:
	template <class Archive>
	void serialize(Archive& ar) {
	}
};
#endif //NKStyleCheckbox_h__