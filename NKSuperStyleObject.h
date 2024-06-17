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
	NKSuperStyleObject(nk_context* ctx, NuklearUI* pManager);
	NKSuperStyleObject(const NKSuperStyleObject& other);
	~NKSuperStyleObject();

public:
	virtual void Layout(nk_context* ctx) override;
	virtual void EditInfo() override;
	virtual void EditStyle() override;

	virtual void EditComponentStyle(nk_context* ctx, NuklearUI* pManager) override;
};

