#pragma once
#ifndef NKColorPicker_h__
#define NKColorPicker_h__
#include "NKBase.h"
class NKColorPicker : public NKBase
{
public:
	NKColorPicker();
    NKColorPicker(nk_context* ctx, NuklearUI* pManager);
	virtual ~NKColorPicker();
	

public:
    void Layout(nk_context* ctx) override;
    void SetColor(struct nk_colorf color);
    struct nk_colorf GetColor() const;

public:
    struct nk_colorf m_color;

public:
	template <class Archive>
	void serialize(Archive& ar) {
		ar(cereal::base_class<NKBase>(this)
			, m_color
		);
	}
};
#endif //NKColorPicker_h__
