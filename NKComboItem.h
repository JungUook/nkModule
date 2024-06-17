#pragma once
#ifndef NKComboItem_h__
#define NKComboItem_h__
#include "NKBase.h"
#include "NKHandler.h"
#include "NKBaseLabel.h"
class NKComboItem : public NKBase, public NKHandler, public NKBaseLabel
{
public:
	NKComboItem(nk_context* ctx, NuklearUI* pManager);
	NKComboItem(const NKComboItem& other);
	~NKComboItem();

public:
	virtual void Layout(nk_context* ctx) override;
	virtual void EditInfo() override;

	void SetLabelNumber(int number);
public:
	int m_labelNumber;
};


#endif //NKComboItem_h__