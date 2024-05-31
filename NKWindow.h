#pragma once
#ifndef NKWindow_h__
#define NKWindow_h__
#include "NKBase.h"
class NKWindow : public NKBase
{
public:
	NKWindow();
	NKWindow(const NKWindow& other);
	~NKWindow();

public:
	virtual void Layout(nk_context* ctx) override;
	virtual void SafeRenderStart() override;
	virtual void SafeRenderEnd() override;
	virtual void EditInfo() override;
	virtual void EditStyle() override;

private:
	int m_border;
	int m_movable;
	int m_scalable;
	int m_closable;
	int m_minimizable;
	int m_no_scrollbar;
	int m_title;
	int m_scroll_auto_hide;
	int m_background;
	int m_scale_left;
	int m_no_input;

	//bg
private:
};

#endif //NKWindow_h__