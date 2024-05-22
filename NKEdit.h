#pragma once
#ifndef NKEdit_h__
#define NKEdit_h__
#include "NKBase.h"
class NKEdit : public NKBase
{
public:
	NKEdit();
	~NKEdit();

public:
	void Layout(nk_context* ctx) override;
	void Clear();
	void RegistFunction(const char* functionName, const char* argsName = nullptr);
	void CallEvent(nk_edit_events edit_event);
public:
	char m_inputText[256];
	int m_inputTextLength;
	nk_plugin_filter m_filter;

	char m_functionName[64];
	char m_argsName[64];
};


#endif //NKEdit_h__