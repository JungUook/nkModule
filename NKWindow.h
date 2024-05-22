#pragma once
#ifndef NKWindow_h__
#define NKWindow_h__
#include "NKBase.h"
class NKWindow : public NKBase
{
public:
	NKWindow();
	~NKWindow();

public:
	void Layout(nk_context* ctx) override;
};

#endif //NKWindow_h__