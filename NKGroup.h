#pragma once
#ifndef NKGroup_h__
#define NKGroup_h__
#include "NKBase.h"
class NKGroup : public NKBase
{
public:
	NKGroup();
	~NKGroup();

public:
	void Layout(nk_context* ctx) override;

public:
	nk_layout_format m_layoutFormat;
	int m_width;
	int m_height;
	int m_cols;
	float* m_ratio;
};

#endif //NKGroup_h__
