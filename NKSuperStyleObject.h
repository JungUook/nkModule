#pragma once
#include "NKBase.h"
#include "NKStyleButton.h"
#include "NKStyleChart.h"
#include "NKStyleCheckbox.h"
#include "NKStyleCombo.h"
#include "NKStyleContextualButton.h"
#include "NKStyleEdit.h"
#include "NKStyleHeader.h"
#include "NKStyleMenuButton.h"
#include "NKStyleOption.h"
#include "NKStyleProgress.h"
#include "NKStyleProperty.h"
#include "NKStyleScrollbarH.h"
#include "NKStyleScrollbarV.h"
#include "NKStyleSelectedable.h"
#include "NKStyleSlider.h"
#include "NKStyleTab.h"
#include "NKStyleText.h"
#include "NKStyleWindow.h"

class NKSuperStyleObject
	: public NKBase
	, public NKStyleButton
	, public NKStyleChart
	, public NKStyleCheckbox
	, public NKStyleCombo
	, public NKStyleContextualButton
	, public NKStyleEdit
	, public NKStyleHeader
	, public NKStyleMenuButton
	, public NKStyleOption
	, public NKStyleProgress
	, public NKStyleProperty
	, public NKStyleScrollbarH
	, public NKStyleScrollbarV
	, public NKStyleSelectedable
	, public NKStyleSlider
	, public NKStyleTab
	, public NKStyleText
	, public NKStyleWindow
{
public:
	NKSuperStyleObject();
	NKSuperStyleObject(nk_context* ctx, NuklearUI* pManager);
	NKSuperStyleObject(const NKSuperStyleObject& other);
	virtual ~NKSuperStyleObject();
	

public:
	virtual void Layout(nk_context* ctx) override;
	virtual void SafeRenderStart() override;
	virtual void SafeRenderEnd() override;
	virtual void EditInfo() override;
	virtual void EditStyle() override;

	virtual void UpdateComponent(nk_context* ctx, NuklearUI* pManager) override;
	virtual void EditComponentStyle(nk_context* ctx, NuklearUI* pManager) override;

public:
	template <class Archive>
	void serialize(Archive& ar) {
		ar(cereal::base_class<NKBase>(this)
			, cereal::base_class<NKStyleButton>(this)
			, cereal::base_class<NKStyleChart>(this)
			, cereal::base_class<NKStyleCheckbox>(this)
			, cereal::base_class<NKStyleCombo>(this)
			, cereal::base_class<NKStyleContextualButton>(this)
			, cereal::base_class<NKStyleEdit>(this)
			, cereal::base_class<NKStyleHeader>(this)
			, cereal::base_class<NKStyleMenuButton>(this)
			, cereal::base_class<NKStyleOption>(this)
			, cereal::base_class<NKStyleProgress>(this)
			, cereal::base_class<NKStyleProperty>(this)
			, cereal::base_class<NKStyleScrollbarH>(this)
			, cereal::base_class<NKStyleScrollbarV>(this)
			, cereal::base_class<NKStyleSelectedable>(this)
			, cereal::base_class<NKStyleSlider>(this)
			, cereal::base_class<NKStyleTab>(this)
			, cereal::base_class<NKStyleText>(this)
			, cereal::base_class<NKStyleWindow>(this)
		);
	}
};

