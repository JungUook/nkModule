#pragma once
#ifndef NKButton_h__
#define NKButton_h__
#include "NKBase.h"
#include "NKHandler.h"
class NKButton : public NKBase, public NKHandler
{
public:
	NKButton(nk_context* ctx, NuklearUI* pManager);
	~NKButton();

public:
	virtual void Layout(nk_context* ctx) override;
	virtual void EditInfo() override;

	void SetButtonName(const char* name);
public:
	char m_content[64];
};
#endif //NKButton_h__