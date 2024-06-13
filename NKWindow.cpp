#include "pch.h"
#include "NKWindow.h"

NKWindow::NKWindow(nk_context* ctx, NuklearUI* pManager) : NKBase(ctx, pManager), NKBaseWindow(), NKStyleHeader(ctx, &m_style), NKStyleWindow(ctx, &m_style)
{
	m_type = eWINDOW;
	m_flags = NK_WINDOW_TITLE;

	m_worldTransform.x = 50.f;
	m_worldTransform.y = 50.f;
	m_worldTransform.w = 300.f;
	m_worldTransform.h = 600.f;
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
		GetPosition(m_pParent, m_pManager);
	}
	nk_end(ctx);
}

void NKWindow::EditInfo()
{
	EditInfoWindowProperty(m_ctx, m_flags);

	if (nk_tree_push(m_ctx, NK_TREE_NODE, "Create UI", NK_MINIMIZED)) {
		if (nk_button_label(m_ctx, "Space"))
		{
			CreateUI("NKSpace");
		}
		nk_tree_pop(m_ctx);
	}
}

void NKWindow::EditStyle()
{
	NKBase::EditStyle();
	EditComponentStyle(m_ctx, m_pManager);
}

void NKWindow::EditComponentStyle(nk_context* ctx, NuklearUI* pManager)
{
	NKStyleHeader::EditComponentStyle(ctx, pManager);
	NKStyleWindow::EditComponentStyle(ctx, pManager);
}
