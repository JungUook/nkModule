#pragma once
#ifndef NKStyleSlider_h__
#define NKStyleSlider_h__
#include "ComponentSlider.h"

class NKStyleSlider
{
public:
	NKStyleSlider(nk_context* ctx, nk_style* style);
	NKStyleSlider(const NKStyleSlider& other);
	~NKStyleSlider();

	virtual void EditComponentStyle(nk_context* ctx, NuklearUI* pManager);

protected:
	ComponentSlider* m_pComponent;
};
#endif //NKStyleSlider_h__