#pragma once
#ifndef NKStyleChart_h__
#define NKStyleChart_h__
#include "NKStyleItem.h"
class NKStyleChart
{
public:
	NKStyleChart();
	NKStyleChart(nk_context* ctx, nk_style* style);
	NKStyleChart(const NKStyleChart& other);
	virtual ~NKStyleChart();

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
	void serialize(Archive& ar) {
		ar(
			*m_pBackground
			, *border_color
			, *selected_color
			, *color
			, *border
			, *rounding
			, *padding
			, *color_factor
			, *disabled_factor
			, *show_markers
		);
	}
};
#endif //NKStyleChart_h__