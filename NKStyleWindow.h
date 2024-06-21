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
	void serialize(Archive& ar) {
		ar(*m_pFixedBackground
			, *m_pScaler
			, *m_pBackground
			, *border
			, *border_color
			, *rounding
			, *spacing
			, *scrollbar_size
			, *min_size
			, *padding
		);
	}
};
#endif //NKStyleWindow_h__
