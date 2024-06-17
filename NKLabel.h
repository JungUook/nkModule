#pragma once
#ifndef NKLabel_h__
#define NKLabel_h__
#include "NKBase.h"
#include "NKBaseLabel.h"
#include "NKStyleText.h"
class NKLabel : public NKBase, public NKBaseLabel, public NKStyleText
{
public:
	NKLabel(nk_context* ctx, NuklearUI* pManager);
	NKLabel(const NKLabel& other);
	~NKLabel();

public:
	virtual void Layout(nk_context* ctx) override;
	virtual void EditInfo() override;
	virtual void EditStyle() override;
};


#endif //NKLabel_h__