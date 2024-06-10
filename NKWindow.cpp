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
		GetPosition(m_pParent, m_manager);
	}
	nk_end(ctx);
}

void NKWindow::EditInfo()
{
	EditInfoWindow();
}

void NKWindow::EditStyle()
{
	NKBase::EditStyle();
}
