#pragma once
#ifndef NKStyleChart_h__
#define NKStyleChart_h__
#include "NKStyleItem.h"
class NKStyleChart
{
public:
	NKStyleChart();
	NKStyleChart(nk_context* ctx, nk_style* style);
	NKStyleChart(const NKStyleChart& other, nk_context* ctx, nk_style* style);
	virtual ~NKStyleChart();

	virtual void UpdateComponent(nk_context* ctx, NuklearUI* pManager);
	virtual void EditComponentStyle(nk_context* ctx, NuklearUI* pManager);

protected:
	NKStyleItem* m_pBackground;
	struct nk_color* border_color;
	struct nk_color* selected_color;
	struct nk_color* color;

	float* border;
	float* rounding;
	struct nk_vec2* padding;
	float* color_factor;
	float* disabled_factor;
	nk_bool* show_markers;

public:
	template <class Archive>
	void serialize(Archive& ar, const unsigned int version) {
		ar(
			CEREAL_NVP(*m_pBackground)
			, CEREAL_NVP(*border_color)
			, CEREAL_NVP(*selected_color)
			, CEREAL_NVP(*color)
			, CEREAL_NVP(*border)
			, CEREAL_NVP(*rounding)
			, CEREAL_NVP(*padding)
			, CEREAL_NVP(*color_factor)
			, CEREAL_NVP(*disabled_factor)
			, CEREAL_NVP(*show_markers)
		);
	}
};
#endif //NKStyleChart_h__