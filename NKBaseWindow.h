#pragma once
class NKBaseWindow
{
public:
	NKBaseWindow();
	NKBaseWindow(const NKBaseWindow& other);
	~NKBaseWindow();

protected:
	void EditInfoWindowProperty(nk_context* ctx, nk_flags& flags);

protected:
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
};

