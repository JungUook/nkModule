#pragma once
#ifndef NKStyleText_h__
#define NKStyleText_h__
#include "NKStyleItem.h"
class NKStyleText
{
public:
	NKStyleText();
	NKStyleText(nk_context* ctx, nk_style* style);
	NKStyleText(const NKStyleText& other, nk_context* ctx, nk_style* style);
	virtual ~NKStyleText();

	virtual void UpdateComponent(nk_context* ctx, NuklearUI* pManager);
	virtual void EditComponentStyle(nk_context* ctx, NuklearUI* pManager);

protected:
	struct nk_color* color;
	struct nk_vec2* padding;
	float* color_factor;
	float* disabled_factor;

public:
	template <class Archive>
	void serialize(Archive& ar, const unsigned int version) {
		ar(CEREAL_NVP(*color)
			, CEREAL_NVP(*padding)
			, CEREAL_NVP(*color_factor)
			, CEREAL_NVP(*disabled_factor)
		);
	}
};
#endif //NKStyleText_h__