#pragma once
#ifndef NKStyleHeader_h__
#define NKStyleHeader_h__
#include "ComponentHeader.h"

class NKStyleHeader
{
public:
	NKStyleHeader(nk_context* ctx, nk_style* style);
	NKStyleHeader(const NKStyleHeader& other);
	~NKStyleHeader();

	virtual void EditComponentStyle(nk_context* ctx, NuklearUI* pManager);

protected:
	ComponentHeader* m_pComponent;
};
#endif //NKStyleHeader_h__