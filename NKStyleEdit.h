#pragma once
#ifndef NKStyleEdit_h__
#define NKStyleEdit_h__
#include "ComponentEdit.h"

class NKStyleEdit
{
public:
	NKStyleEdit(nk_context* ctx, nk_style* style);
	NKStyleEdit(const NKStyleEdit& other);
	~NKStyleEdit();

	virtual void EditComponentStyle(nk_context* ctx, NuklearUI* pManager);

protected:
	ComponentEdit* m_pComponent;
};
#endif //NKStyleEdit_h__