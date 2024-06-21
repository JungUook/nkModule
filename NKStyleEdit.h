#pragma once
#ifndef NKStyleEdit_h__
#define NKStyleEdit_h__
#include "ComponentEdit.h"

class NKStyleEdit
{
public:
	NKStyleEdit();
	NKStyleEdit(nk_context* ctx, nk_style* style);
	NKStyleEdit(const NKStyleEdit& other, nk_context* ctx, nk_style* style);
	virtual ~NKStyleEdit();

	virtual void UpdateComponent(nk_context* ctx, NuklearUI* pManager);
	virtual void EditComponentStyle(nk_context* ctx, NuklearUI* pManager);

protected:
	ComponentEdit* m_pComponent;

public:
	template <class Archive>
	void serialize(Archive& ar) {
		ar(
			*m_pComponent
		);
	}
};
#endif //NKStyleEdit_h__