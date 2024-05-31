#include "pch.h"
#include "NKWindow.h"

NKWindow::NKWindow() : NKBase()
{
	m_type = eWINDOW;
	m_flags = NK_WINDOW_TITLE;

	m_pivot.x = 0.f;
	m_pivot.y = 0.f;
	m_worldTransform.x = 50.f;
	m_worldTransform.y = 50.f;
	m_worldTransform.w = 300.f;
	m_worldTransform.h = 600.f;

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
}

NKWindow::NKWindow(const NKWindow& other) : NKBase()
{
	m_type					= other.m_type;
	m_flags					= other.m_flags;
	m_pivot.x				= other.m_pivot.x;
	m_pivot.y				= other.m_pivot.y;
	m_worldTransform.x		= other.m_worldTransform.x;
	m_worldTransform.y		= other.m_worldTransform.y;
	m_worldTransform.w		= other.m_worldTransform.w;
	m_worldTransform.h		= other.m_worldTransform.h;
	m_border				= other.m_border;
	m_movable				= other.m_movable;
	m_scalable				= other.m_scalable;
	m_closable				= other.m_closable;
	m_minimizable			= other.m_minimizable;
	m_no_scrollbar			= other.m_no_scrollbar;
	m_title					= other.m_title;
	m_scroll_auto_hide		= other.m_scroll_auto_hide;
	m_background			= other.m_background;
	m_scale_left			= other.m_scale_left;
	m_no_input				= other.m_no_input;	
}

NKWindow::~NKWindow()
{
}

void NKWindow::Layout(nk_context* ctx)
{
	m_bHovering = false;
	if (nk_begin(ctx, m_primaryName, m_worldTransform, m_flags))
	{
		for (std::list<NKBase*>::iterator it = m_pChildList.begin(); it != m_pChildList.end(); ++it)
		{
			(*it)->Update(ctx);
		}

		m_worldTransform = nk_window_get_bounds(ctx);
		if (nk_input_is_mouse_hovering_rect(&ctx->input, m_worldTransform))
		{
			m_bHovering = true;
		}
		GetPosition();
	}
	nk_end(ctx);
}

void NKWindow::SafeRenderStart()
{
	//if (m_bbgCustom)
	//{
	//	if (m_titlebgOption == 1) {
	//		struct nk_image img;
	//		m_manager->GetSprite(m_bgImagePath.c_str(), m_bgSprIndex, img);
	//		m_style.window.fixed_background = nk_style_item_image(img);
	//	}
	//	else if (m_titlebgOption == 2) {
	//		struct nk_image img;
	//		m_manager->GetSprite(m_titlebgImagePath.c_str(), m_titlebgSprIndex, img);
	//		struct nk_nine_slice nineslice;
	//		nineslice.img = img;
	//		nineslice.l = (nk_ushort)m_bgNineslice[0];
	//		nineslice.t = (nk_ushort)m_bgNineslice[1];
	//		nineslice.r = (nk_ushort)m_bgNineslice[2];
	//		nineslice.b = (nk_ushort)m_bgNineslice[3];
	//		m_style.window.fixed_background = nk_style_item_nine_slice(nineslice);
	//	}
	//}
}

void NKWindow::SafeRenderEnd()
{
}

void NKWindow::EditInfo()
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

void NKWindow::EditStyle()
{
	HeaderEditor();
	WindowEditor();
	ComponentEditor();
}
