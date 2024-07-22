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

	float* group_border;
	struct nk_color* group_border_color;
	struct nk_vec2* group_padding;

	float* tooltip_border;
	struct nk_color* tooltip_border_color;
	struct nk_vec2* tooltip_padding;

	float* popup_border;
	struct nk_color* popup_border_color;
	struct nk_vec2* popup_padding;

public:
	template <class Archive>
	void serialize(Archive& ar, const unsigned int version) {
		if (version >= 2) {
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
				, CEREAL_NVP(*group_border)
				, CEREAL_NVP(*group_border_color)
				, CEREAL_NVP(*group_padding)
				, CEREAL_NVP(*tooltip_border)
				, CEREAL_NVP(*tooltip_border_color)
				, CEREAL_NVP(*tooltip_padding)
				, CEREAL_NVP(*popup_border)
				, CEREAL_NVP(*popup_border_color)
				, CEREAL_NVP(*popup_padding)
			);
		}
		else {
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
	}
};
#endif //NKStyleWindow_h__
