#pragma once
#ifndef NKButton_h__
#define NKButton_h__
#include "NKBase.h"
#include "NKHandler.h"
#include "NKBaseLabel.h"
#include "NKStyleButton.h"
class NKButton : public NKBase, public NKHandler, public NKBaseLabel, public NKStyleButton
{
public:
	NKButton();
	NKButton(nk_context* ctx, NuklearUI* pManager);
	NKButton(const NKButton& other);
	virtual ~NKButton();
	

public:
	virtual void Layout(nk_context* ctx) override;
	virtual void SafeRenderStart(nk_context* ctx) override;
	virtual void SafeRenderEnd(nk_context* ctx) override;
	virtual void EditInfo(nk_context* ctx) override;
	virtual void EditStyle(nk_context* ctx) override;

public:
	template <class Archive>
	void serialize(Archive& ar) {
		ar(cereal::base_class<NKBase>(this)
			, cereal::base_class<NKHandler>(this)
			, cereal::base_class<NKBaseLabel>(this)
			, cereal::base_class<NKStyleButton>(this)
		);
	}
};
#endif //NKButton_h__