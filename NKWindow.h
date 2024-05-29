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
	void Layout(nk_context* ctx) override;
	void SafeRenderStart() override;
	void SafeRenderEnd() override;
	void EditInfo() override;

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

	//title_bg
private:
	std::string m_titlebgImagePath;
	int m_titlebgSprIndex;
	int m_titlebgSprSize;
	bool m_btitlebgCustom;

	//bg
private:
	std::string m_bgImagePath;
	int m_bgSprIndex;
	int m_bgSprSize;
	bool m_bbgCustom;
};

#endif //NKWindow_h__