#include "pch.h"
#include "NKGroup.h"

NKGroup::NKGroup(nk_context* ctx, NuklearUI* pManager) : NKBase(ctx, pManager), NKBaseWindow(), NKStyleHeader(ctx, &m_style), NKStyleWindow(ctx, &m_style)
{
	m_type			= eGROUP;
	m_flags			= 0;

	m_cTransform.x = 50.f;
	m_cTransform.y = 50.f;
	m_cTransform.w = 150.f;
	m_cTransform.h = 300.f;
}

NKGroup::NKGroup(const NKGroup& other) : NKBase(other), NKBaseWindow(other), NKStyleHeader(other), NKStyleWindow(other)
{
	m_type			= other.m_type;
	m_flags			= other.m_flags;
}

NKGroup::~NKGroup()
{
}

void NKGroup::Layout(nk_context* ctx)
{
	if (nk_group_begin(ctx, m_cBaseName, m_flags))
	{
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
	EditInfoWindowProperty(m_ctx, m_flags);

	if (nk_tree_push(m_ctx, NK_TREE_NODE, "Create UI", NK_MINIMIZED)) {
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
	EditComponentStyle(m_ctx, m_pManager);
}

void NKGroup::EditComponentStyle(nk_context* ctx, NuklearUI* pManager)
{
	NKStyleHeader::EditComponentStyle(ctx, pManager);
	NKStyleWindow::EditComponentStyle(ctx, pManager);
}
