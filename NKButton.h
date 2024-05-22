#pragma once
#ifndef NKButton_h__
#define NKButton_h__
#include "NKBase.h"
class NKButton : public NKBase
{
public:
	NKButton();
	~NKButton();

public:
	void Layout(nk_context* ctx) override;

	void SetButtonName(const char* name);
	void RegistFunction(const char* functionName, const char* argsName = nullptr);
	void CallEvent();
public:
	char m_content[64];
	char m_functionName[64];
	char m_argsName[64];
};
#endif //NKButton_h__