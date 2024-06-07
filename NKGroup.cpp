#include "pch.h"
#include "NKGroup.h"

NKGroup::NKGroup() : NKBase()
{
	m_layoutFormat	= NK_DYNAMIC;
	m_width			= 0;
	m_height		= 0;	
	m_cols			= 0;
	m_ratio			= nullptr;
	m_type			= eGROUP;
	m_flags			= 0;
}

NKGroup::NKGroup(const NKGroup& other) : NKBase()
{
	m_layoutFormat	= other.m_layoutFormat;
	m_width			= other.m_width;
	m_height		= other.m_height;
	m_cols			= other.m_cols;
	m_ratio			= other.m_ratio;
	m_type			= other.m_type;
	m_flags			= other.m_flags;
}

NKGroup::~NKGroup()
{
}

void NKGroup::Layout(nk_context* ctx)
{
	if (nk_group_begin(ctx, m_baseName, m_flags))
	{
		if (m_layoutFormat == NK_DYNAMIC)
		{
			nk_layout_row_dynamic(ctx, m_height, m_cols);
		}
		else
		{
			nk_layout_row_static(ctx, m_height, m_width, m_cols);
		}
		for (std::list<NKBase*>::iterator it = m_pChildList.begin(); it != m_pChildList.end(); ++it)
		{
			(*it)->Update(ctx);
		}

		nk_group_end(ctx);
	}
}

void NKGroup::SafeRenderStart()
{
}

void NKGroup::SafeRenderEnd()
{
}

void NKGroup::EditInfo()
{
	if (nk_tree_push(m_ctx, NK_TREE_NODE, "Flag", NK_MINIMIZED)) {
		nk_checkbox_label(m_ctx, "BORDER", &m_border);
		nk_checkbox_label(m_ctx, "MOVABLE", &m_movable);
		nk_checkbox_label(m_ctx, "SCALABLE", &m_scalable);
		nk_checkbox_label(m_ctx, "CLOSABLE", &m_closable);
		nk_checkbox_label(m_ctx, "MINIMIZABLE", &m_minimizable);
		nk_checkbox_label(m_ctx, "NO_SCROLLBAR", &m_no_scrollbar);
		nk_checkbox_label(m_ctx, "TITLE", &m_title);
		nk_checkbox_label(m_ctx, "SCROLL_AUTO_HIDE", &m_scroll_auto_hide);
		nk_checkbox_label(m_ctx, "BACKGROUND", &m_background);
		nk_checkbox_label(m_ctx, "SCALE_LEFT", &m_scale_left);
		nk_checkbox_label(m_ctx, "NO_INPUT", &m_no_input);
		nk_tree_pop(m_ctx);
	}

	m_flags = 0;
	if (m_border)
		m_flags |= NK_WINDOW_BORDER;
	if (m_movable)
		m_flags |= NK_WINDOW_MOVABLE;
	if (m_scalable)
		m_flags |= NK_WINDOW_SCALABLE;
	if (m_closable)
		m_flags |= NK_WINDOW_CLOSABLE;
	if (m_minimizable)
		m_flags |= NK_WINDOW_MINIMIZABLE;
	if (m_no_scrollbar)
		m_flags |= NK_WINDOW_NO_SCROLLBAR;
	if (m_title)
		m_flags |= NK_WINDOW_TITLE;
	if (m_scroll_auto_hide)
		m_flags |= NK_WINDOW_SCROLL_AUTO_HIDE;
	if (m_background)
		m_flags |= NK_WINDOW_BACKGROUND;
	if (m_scale_left)
		m_flags |= NK_WINDOW_SCALE_LEFT;
	if (m_no_input)
		m_flags |= NK_WINDOW_NO_INPUT;

	if (nk_tree_push(m_ctx, NK_TREE_NODE, "Create_UI", NK_MINIMIZED)) {
		if (nk_button_label(m_ctx, "Space"))
		{
			CreateUI("NKSpace");
		}
		nk_tree_pop(m_ctx);
	}
}

void NKGroup::EditStyle()
{
	NKBase::EditStyle();
}
