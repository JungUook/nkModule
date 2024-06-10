#include "pch.h"
#include "NKProperty.h"

NKProperty::NKProperty() : NKStyle(), NKTransform()
{
	m_flags = 0;
}

NKProperty::NKProperty(const NKProperty& other) : NKStyle(other), NKTransform(other)
{
	m_flags = other.m_flags;
}

NKProperty::~NKProperty()
{
}

void NKProperty::EditInfoWindowProperty(nk_context* ctx)
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
}