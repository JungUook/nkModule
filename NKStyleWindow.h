#pragma once
#ifndef NKStyleWindow_h__
#define NKStyleWindow_h__
#include "NKStyleItem.h"
class NKStyleWindow
{
public:
	NKStyleWindow();
	NKStyleWindow(nk_context* ctx, nk_style* style);
	NKStyleWindow(const NKStyleWindow& other, nk_context* ctx, nk_style* style);
	virtual ~NKStyleWindow();

	virtual void UpdateComponent(nk_context* ctx, NuklearUI* pManager);
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

public:
	template <class Archive>
	void serialize(Archive& ar, const unsigned int version) {
		ar(CEREAL_NVP(*m_pFixedBackground)
			, CEREAL_NVP(*m_pScaler)
			, CEREAL_NVP(*m_pBackground)
			, CEREAL_NVP(*border)
			, CEREAL_NVP(*border_color)
			, CEREAL_NVP(*rounding)
			, CEREAL_NVP(*spacing)
			, CEREAL_NVP(*scrollbar_size)
			, CEREAL_NVP(*min_size)
			, CEREAL_NVP(*padding)
		);
	}
};
#endif //NKStyleWindow_h__
