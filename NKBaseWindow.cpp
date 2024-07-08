#include "pch.h"
#include "NKBaseWindow.h"
#include "NuklearUI.h"

NKBaseWindow::NKBaseWindow()
{
	m_border = 0;
	m_movable = 0;
	m_scalable = 0;
	m_closable = 0;
	m_minimizable = 0;
	m_no_scrollbar = 0;
	m_title = 1;
	m_scroll_auto_hide = 0;
	m_background = 0;
	m_scale_left = 0;
	m_no_input = 0;
	m_fScale = 1.f;
}

NKBaseWindow::NKBaseWindow(const NKBaseWindow& other)
{
	m_border			= other.m_border;
	m_movable			= other.m_movable;
	m_scalable			= other.m_scalable;
	m_closable			= other.m_closable;
	m_minimizable		= other.m_minimizable;
	m_no_scrollbar		= other.m_no_scrollbar;
	m_title				= other.m_title;
	m_scroll_auto_hide	= other.m_scroll_auto_hide;
	m_background		= other.m_background;
	m_scale_left		= other.m_scale_left;
	m_no_input			= other.m_no_input;
	m_fScale			= other.m_fScale;
}

NKBaseWindow::~NKBaseWindow()
{
}

void NKBaseWindow::EditInfoWindowProperty(nk_context* ctx, nk_flags& flags)
{
	if (nk_tree_push(ctx, NK_TREE_NODE, "Flag", NK_MINIMIZED)) {
		nk_checkbox_label(ctx, "BORDER", &m_border);
		nk_checkbox_label(ctx, "MOVABLE", &m_movable);
		nk_checkbox_label(ctx, "SCALABLE", &m_scalable);
		nk_checkbox_label(ctx, "CLOSABLE", &m_closable);
		nk_checkbox_label(ctx, "MINIMIZABLE", &m_minimizable);
		nk_checkbox_label(ctx, "NO_SCROLLBAR", &m_no_scrollbar);
		nk_checkbox_label(ctx, "TITLE", &m_title);
		nk_checkbox_label(ctx, "SCROLL_AUTO_HIDE", &m_scroll_auto_hide);
		nk_checkbox_label(ctx, "BACKGROUND", &m_background);
		nk_checkbox_label(ctx, "SCALE_LEFT", &m_scale_left);
		nk_checkbox_label(ctx, "NO_INPUT", &m_no_input);
		nk_tree_pop(ctx);
	}

	flags = 0;
	if (m_border)
		flags |= NK_WINDOW_BORDER;
	if (m_movable)
		flags |= NK_WINDOW_MOVABLE;
	if (m_scalable)
		flags |= NK_WINDOW_SCALABLE;
	if (m_closable)
		flags |= NK_WINDOW_CLOSABLE;
	if (m_minimizable)
		flags |= NK_WINDOW_MINIMIZABLE;
	if (m_no_scrollbar)
		flags |= NK_WINDOW_NO_SCROLLBAR;
	if (m_title)
		flags |= NK_WINDOW_TITLE;
	if (m_scroll_auto_hide)
		flags |= NK_WINDOW_SCROLL_AUTO_HIDE;
	if (m_background)
		flags |= NK_WINDOW_BACKGROUND;
	if (m_scale_left)
		flags |= NK_WINDOW_SCALE_LEFT;
	if (m_no_input)
		flags |= NK_WINDOW_NO_INPUT;

	if (m_title) {
		if (nk_tree_push(ctx, NK_TREE_NODE, "Properties", NK_MINIMIZED)) {
			nk_layout_row_dynamic(ctx, 44, 1);
			nk_slider_float(ctx, 0.1f, &m_fScale, 2.f, 0.01f);
			nk_property_float(ctx, "#Title size", 0.1f, &m_fScale, 2.f, 0.1f, 0.01f);
			nk_tree_pop(ctx);
		}
	}
}