#pragma once
#ifndef NKLabel_h__
#define NKLabel_h__
#include "NKBase.h"
class NKLabel : public NKBase
{
public:
	NKLabel();
	~NKLabel();

public:
	virtual void Layout(nk_context* ctx) override;
	virtual void EditInfo() override;

	void SetLabel(const char* text);
public:
	char m_content[256];
};


#endif //NKLabel_h__