#pragma once
#ifndef NKProperty_h__
#define NKProperty_h__
#include "NKStyle.h"
#include "NKTransform.h"

class NKStyle;
class NKTransform;

class NKProperty : public NKStyle, public NKTransform
{
public:
	NKProperty();
	NKProperty(const NKProperty& other);
	~NKProperty();

protected:
	void EditInfoWindowProperty(nk_context* ctx);

	//window property
protected:
	nk_flags m_flags;
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
#endif //NKProperty_h__