#pragma once
#ifndef NKBaseWindow_h__
#define NKBaseWindow_h__

#define CEREAL_NVP(T) ::cereal::make_nvp(#T, T)

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
	void serialize(Archive& ar, const unsigned int version) {
		ar(
			  CEREAL_NVP(m_border)
			, CEREAL_NVP(m_movable)
			, CEREAL_NVP(m_scalable)
			, CEREAL_NVP(m_closable)
			, CEREAL_NVP(m_minimizable)
			, CEREAL_NVP(m_no_scrollbar)
			, CEREAL_NVP(m_title)
			, CEREAL_NVP(m_scroll_auto_hide)
			, CEREAL_NVP(m_background)
			, CEREAL_NVP(m_scale_left)
			, CEREAL_NVP(m_no_input)
			, CEREAL_NVP(m_fScale)
		);
	}
};
#endif //NKBaseWindow_h__