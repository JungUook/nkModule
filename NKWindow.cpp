#include "pch.h"
#include "NKWindow.h"

NKWindow::NKWindow() : NKBase(), NKBaseWindow(), NKStyleHeader(), NKStyleWindow()
{
	m_type = eWINDOW;
	m_flags = NK_WINDOW_TITLE;
}

NKWindow::NKWindow(nk_context* ctx, NuklearUI* pManager) : NKBase(ctx, pManager), NKBaseWindow(), NKStyleHeader(ctx, &m_style), NKStyleWindow(ctx, &m_style)
{
	m_type = eWINDOW;
	m_flags = NK_WINDOW_TITLE;

	m_cTransform.x = 50.f;
	m_cTransform.y = 50.f;
	m_cTransform.w = 300.f;
	m_cTransform.h = 600.f;
}

NKWindow::NKWindow(const NKWindow& other) : NKBase(other), NKBaseWindow(other), NKStyleHeader(other, m_ctx, &m_style), NKStyleWindow(other, m_ctx, &m_style)
{
	m_type = other.m_type;
	m_flags = other.m_flags;
}

NKWindow::~NKWindow()
{
}

void NKWindow::Layout(nk_context* ctx)
{
	if (nk_begin(ctx, m_cprimaryName, m_cTransform, m_flags))
	{
		for (std::list<NKBase*>::iterator it = m_pChildList.begin(); it != m_pChildList.end(); ++it)
		{
			(*it)->Update(ctx);
		}
		m_cTransform = nk_window_get_bounds(ctx);
		CheckMouseHover(ctx);
		GetPosition(m_pParent, m_pManager);
	}	
	nk_end(ctx);
}

void NKWindow::SafeRenderStart(nk_context* ctx)
{
	UpdateComponent(ctx, m_pManager);
}

void NKWindow::SafeRenderEnd(nk_context* ctx)
{
}

void NKWindow::EditInfo(nk_context* ctx)
{
	EditInfoWindowProperty(ctx, m_flags);

	if (nk_tree_push(ctx, NK_TREE_NODE, "Create UI", NK_MINIMIZED)) {
		if (nk_button_label(ctx, "Space"))
		{
			CreateUI("NKSpace");
		}
		if (nk_button_label(ctx, "SuperStyle"))
		{
			CreateUI("NKSuperStyleObject");
		}
		nk_tree_pop(ctx);
	}
}

void NKWindow::EditStyle(nk_context* ctx)
{
	NKBase::EditStyle(ctx);
	EditComponentStyle(ctx, m_pManager);
}

void NKWindow::UpdateComponent(nk_context* ctx, NuklearUI* pManager)
{
	NKStyleHeader::UpdateComponent(ctx, pManager);
	NKStyleWindow::UpdateComponent(ctx, pManager);
}

void NKWindow::EditComponentStyle(nk_context* ctx, NuklearUI* pManager)
{
	NKStyleHeader::EditComponentStyle(ctx, pManager);
	NKStyleWindow::EditComponentStyle(ctx, pManager);
}
