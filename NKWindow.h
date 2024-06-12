#pragma once
#ifndef NKWindow_h__
#define NKWindow_h__
#include "NKBase.h"
#include "NKBaseWindow.h"
class NKWindow : public NKBase, public NKBaseWindow
{
public:
	NKWindow();
	~NKWindow();

public:
	virtual void Layout(nk_context* ctx) override;
	virtual void EditInfo() override;
	virtual void EditStyle() override;
};

#endif //NKWindow_h__