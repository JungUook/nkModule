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
	NKButton(nk_context* ctx, NuklearUI* pManager);
	NKButton(const NKButton& other);
	~NKButton();

public:
	virtual void Layout(nk_context* ctx) override;
	virtual void EditInfo() override;
	virtual void EditStyle() override;

};
#endif //NKButton_h__