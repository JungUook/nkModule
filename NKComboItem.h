#pragma once
#ifndef NKComboItem_h__
#define NKComboItem_h__
#include "NKBase.h"
class NKComboItem : public NKBase
{
public:
	NKComboItem();
	~NKComboItem();

public:
	void Layout(nk_context* ctx) override;
	void SetComboName(const char* name);
	void RegistFunction(const char* functionName, const char* argsName = nullptr);
	void SetLabel(int number);

	void CallEvent();
public:
	int m_labelNumber;
	char m_content[64];
	char m_functionName[64];
	char m_argsName[64];
};


#endif //NKComboItem_h__