#pragma once
#ifndef NKGroup_h__
#define NKGroup_h__
#include "NKBase.h"
class NKGroup : public NKBase
{
public:
	NKGroup();
	NKGroup(const NKGroup& other);
	~NKGroup();

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

public:
	nk_layout_format m_layoutFormat;
	int m_width;
	int m_height;
	int m_cols;
	float* m_ratio;
};

#endif //NKGroup_h__
