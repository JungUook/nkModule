#pragma once
#ifndef NKCombo_h__
#define NKCombo_h__
#include "NKBase.h"
#include <vector>
class NKCombo : public NKBase
{
public:
	NKCombo();
	~NKCombo();

public:
	void Layout(nk_context* ctx) override;

	void SetComboName(const char* name);
	void SetLabelSize(float x, float y);
	void SetCurrentLabel(int number);
public:
	int m_currentLabel;
	struct nk_vec2 m_labelSize;
	char m_content[64];
};


#endif //NKCombo_h__