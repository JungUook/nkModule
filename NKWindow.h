#pragma once
#ifndef NKWindow_h__
#define NKWindow_h__
#include "NKBase.h"
#include "NKBaseWindow.h"
#include "NKStyleHeader.h"
#include "NKStyleWindow.h"
class NKWindow : public NKBase, public NKBaseWindow, public NKStyleHeader, public NKStyleWindow
{
public:
	NKWindow(nk_context* ctx, NuklearUI* pManager);
	NKWindow(const NKWindow& other);
	~NKWindow();

public:
	virtual void Update(nk_context* ctx) override;
	virtual void Layout(nk_context* ctx) override;
	virtual void EditInfo() override;
	virtual void EditStyle() override;

	virtual void EditComponentStyle(nk_context* ctx, NuklearUI* pManager) override;
};

#endif //NKWindow_h__