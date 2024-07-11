#pragma once
#ifndef NKStyleTab_h__
#define NKStyleTab_h__
#include "ComponentTab.h"

class NKStyleTab
{
public:
	NKStyleTab();
	NKStyleTab(nk_context* ctx, nk_style* style);
	NKStyleTab(const NKStyleTab& other, nk_context* ctx, nk_style* style);
	virtual ~NKStyleTab();

	virtual void UpdateComponent(nk_context* ctx, NuklearUI* pManager);
	virtual void EditComponentStyle(nk_context* ctx, NuklearUI* pManager);

protected:
	ComponentTab* m_pComponent;

public:
	template <class Archive>
	void serialize(Archive& ar, const unsigned int version) {
		ar(
			CEREAL_NVP(*m_pComponent)
		);
	}
};
#endif //NKStyleTab_h__