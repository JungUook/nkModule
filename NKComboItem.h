#pragma once
#ifndef NKComboItem_h__
#define NKComboItem_h__
#include "NKBase.h"
#include "NKHandler.h"
class NKComboItem : public NKBase, public NKHandler
{
public:
	NKComboItem(nk_context* ctx, NuklearUI* pManager);
	~NKComboItem();

public:
	virtual void Layout(nk_context* ctx) override;
	virtual void EditInfo() override;
		
	void SetComboName(const char* name);
	void SetLabel(int number);
public:
	int m_labelNumber;
	char m_content[64];
};


#endif //NKComboItem_h__