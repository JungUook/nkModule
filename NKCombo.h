#pragma once
#ifndef NKCombo_h__
#define NKCombo_h__
#include "NKBase.h"
class NKCombo : public NKBase
{
public:
	NKCombo(nk_context* ctx, NuklearUI* pManager);
	~NKCombo();

public:
	virtual void Layout(nk_context* ctx) override;
	virtual void EditInfo() override;

	void SetComboName(const char* name);
	void SetLabelSize(float x, float y);
	void SetCurrentLabel(int number);
public:
	int m_currentLabel;
	nk_text_alignment m_labelAlignment;
	struct nk_vec2 m_labelSize;
	char m_content[64];
};


#endif //NKCombo_h__