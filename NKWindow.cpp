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

NKWindow::NKWindow(const NKWindow& other) : NKBase(other), NKBaseWindow(other), NKStyleHeader(other), NKStyleWindow(other)
{
	m_type = other.m_type;
	m_flags = other.m_flags;
}

NKWindow::~NKWindow()
{
}

void NKWindow::Update(nk_context* ctx)
{
	if (m_bActive)
	{
		nk_style original = ctx->style;

		ctx->style = m_pParent != nullptr && m_followParentStyle ? *m_pParentStyle : m_style;

		Layout(ctx);

		ctx->style = original;
	}
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

void NKWindow::EditInfo()
{
	EditInfoWindowProperty(m_ctx, m_flags);

	if (nk_tree_push(m_ctx, NK_TREE_NODE, "Create UI", NK_MINIMIZED)) {
		if (nk_button_label(m_ctx, "Space"))
		{
			CreateUI("NKSpace");
		}
		if (nk_button_label(m_ctx, "SuperStyle"))
		{
			CreateUI("NKSuperStyleObject");
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
