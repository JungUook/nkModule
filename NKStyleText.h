#pragma once
#ifndef NKStyleText_h__
#define NKStyleText_h__
#include "NKStyleItem.h"
class NKStyleText
{
public:
	NKStyleText(nk_context* ctx, nk_style* style);
	NKStyleText(const NKStyleText& other);
	~NKStyleText();

	virtual void EditComponentStyle(nk_context* ctx, NuklearUI* pManager);

protected:
	struct nk_color* color;
	struct nk_vec2* padding;
	float* color_factor;
	float* disabled_factor;
};
#endif //NKStyleText_h__