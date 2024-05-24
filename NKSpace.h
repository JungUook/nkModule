#pragma once
#ifndef NKSpace_h__
#define NKSpace_h__
#include "NKBase.h"
class NKSpace : public NKBase
{
public:
	NKSpace();
	~NKSpace();

public:
	void Layout(nk_context* ctx) override;
	
	void SetLayout(int type);
public:
	nk_layout_format m_layoutFormat;
	int m_widgetCount;
};

#endif //NKSpace_h__