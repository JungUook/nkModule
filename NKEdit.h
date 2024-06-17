#pragma once
#ifndef NKEdit_h__
#define NKEdit_h__
#include "NKBase.h"
#include "NKHandler.h"
#include "NKStyleEdit.h"
class NKEdit : public NKBase, public NKHandler, public NKStyleEdit
{
public:
	NKEdit(nk_context* ctx, NuklearUI* pManager);
	NKEdit(const NKEdit& other);
	~NKEdit();

public:
	virtual void Layout(nk_context* ctx) override;
	virtual void EditStyle() override;
	void Clear();

public:
	char m_inputText[256];
	int m_inputTextLength;
	nk_plugin_filter m_filter;
};


#endif //NKEdit_h__