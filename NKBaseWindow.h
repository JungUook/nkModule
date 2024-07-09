#pragma once
#ifndef NKBaseWindow_h__
#define NKBaseWindow_h__

class NuklearUI;

class NKBaseWindow
{
public:
	NKBaseWindow();
	NKBaseWindow(const NKBaseWindow& other);
	virtual ~NKBaseWindow();

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

	float m_fScale;

public:
	template <class Archive>
	void serialize(Archive& ar) {
		ar(m_border
			, m_movable
			, m_scalable
			, m_closable
			, m_minimizable
			, m_no_scrollbar
			, m_title
			, m_scroll_auto_hide
			, m_background
			, m_scale_left
			, m_no_input
			, m_fScale
		);
	}
};
#endif //NKBaseWindow_h__