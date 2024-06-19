#pragma once
#ifndef NKStyleTab_h__
#define NKStyleTab_h__
#include "ComponentTab.h"

class NKStyleTab
{
public:
	NKStyleTab();
	NKStyleTab(nk_context* ctx, nk_style* style);
	NKStyleTab(const NKStyleTab& other);
	virtual ~NKStyleTab();

	virtual void EditComponentStyle(nk_context* ctx, NuklearUI* pManager);

protected:
	ComponentTab* m_pComponent;

public:
	template <class Archive>
	void serialize(Archive& ar) {
	}
};
#endif //NKStyleTab_h__