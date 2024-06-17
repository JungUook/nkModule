#pragma once
#ifndef NKStyleWindow_h__
#define NKStyleWindow_h__
#include "NKStyleItem.h"
class NKStyleWindow
{
public:
	NKStyleWindow(nk_context* ctx, nk_style* style);
	NKStyleWindow(const NKStyleWindow& other);
	~NKStyleWindow();

	virtual void EditComponentStyle(nk_context* ctx, NuklearUI* pManager);
protected:
	NKStyleItem* m_pFixedBackground;
	NKStyleItem* m_pScaler;
	nk_color* m_pBackground;

	//properties
	float* border;
	struct nk_color* border_color;

	float* rounding;
	struct nk_vec2* spacing;
	struct nk_vec2* scrollbar_size;
	struct nk_vec2* min_size;
	struct nk_vec2* padding;
};
#endif //NKStyleWindow_h__
