#pragma once
#ifndef NKCombo_h__
#define NKCombo_h__
#include "NKBase.h"
#include "NKStyleCombo.h"
class NKCombo : public NKBase, public NKStyleCombo
{
public:
	NKCombo(nk_context* ctx, NuklearUI* pManager);
	NKCombo(const NKCombo& other);
	~NKCombo();

public:
	virtual void Layout(nk_context* ctx) override;
	virtual void EditInfo() override;
	virtual void EditStyle() override;

	void SetComboName(const char* name);
	void SetLabelSize(float x, float y);
	void SetCurrentLabel(int number);
public:
	int m_currentLabel;
	nk_text_alignment m_labelAlignment;
	struct nk_vec2 m_labelSize;
	char m_cComboLabel[256];
};


#endif //NKCombo_h__