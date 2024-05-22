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
	void Layout(nk_context* ctx) override;

public:
	char m_content[256];
};


#endif //NKLabel_h__