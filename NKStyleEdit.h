#pragma once
#ifndef NKStyleEdit_h__
#define NKStyleEdit_h__
#include "ComponentEdit.h"

class NKStyleEdit
{
public:
	NKStyleEdit();
	NKStyleEdit(nk_context* ctx, nk_style* style);
	NKStyleEdit(const NKStyleEdit& other);
	virtual ~NKStyleEdit();

	virtual void EditComponentStyle(nk_context* ctx, NuklearUI* pManager);

protected:
	ComponentEdit* m_pComponent;

public:
	template <class Archive>
	void serialize(Archive& ar) {
	}
};
#endif //NKStyleEdit_h__