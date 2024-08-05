#include "pch.h"
#include "NKGroup.h"

NKGroup::NKGroup() : NKBase(), NKBaseWindow(), NKStyleHeader(), NKStyleWindow(), NKStyleScrollbarH(), NKStyleScrollbarV()
{
	m_type = eGROUP;
	m_flags = 0;
}

NKGroup::NKGroup(nk_context* ctx, NuklearUI* pManager) : NKBase(ctx, pManager), NKBaseWindow(), NKStyleHeader(ctx, &m_style), NKStyleWindow(ctx, &m_style), NKStyleScrollbarH(ctx, &m_style), NKStyleScrollbarV(ctx, &m_style)
{
	m_type			= eGROUP;
	m_flags			= NK_WINDOW_TITLE;

	m_cTransform.w = 150.f;
	m_cTransform.h = 300.f;
}

NKGroup::NKGroup(const NKGroup& other) : NKBase(other), NKBaseWindow(other), NKStyleHeader(other, m_ctx, &m_style), NKStyleWindow(other, m_ctx, &m_style), NKStyleScrollbarH(other, m_ctx, &m_style), NKStyleScrollbarV(other, m_ctx, &m_style)
{
	m_type			= other.m_type;
	m_flags			= other.m_flags;
}

NKGroup::~NKGroup()
{
}

void NKGroup::Layout(nk_context* ctx)
{
	char primary_name[256] = { 0, };
	strncpy_s(primary_name, m_sPrimaryName.c_str(), 256);
	primary_name[255] = '\0';
	
	if (nk_group_begin_titled(ctx, primary_name, m_sWindowName.c_str(), m_flags))
	{
		for (std::list<NKBase*>::iterator it = m_pChildList.begin(); it != m_pChildList.end(); ++it)
		{
			(*it)->Update(ctx);
		}
		nk_group_end(ctx);
	}
}

void NKGroup::SafeRenderStart(nk_context* ctx)
{
	UpdateComponent(ctx, m_pManager);
}

void NKGroup::SafeRenderEnd(nk_context* ctx)
{
}

void NKGroup::EditInfo(nk_context* ctx)
{
	EditInfoWindowProperty(ctx, m_flags);

	if (nk_tree_push(ctx, NK_TREE_NODE, "Create UI", NK_MINIMIZED)) {
		if (nk_button_label(ctx, "Space"))
		{
			CreateUI("NKSpace");
		}
		nk_tree_pop(ctx);
	}
}

void NKGroup::EditStyle(nk_context* ctx)
{
	NKBase::EditStyle(ctx);
	EditComponentStyle(ctx, m_pManager);
}

void NKGroup::UpdateComponent(nk_context* ctx, NuklearUI* pManager)
{
	NKStyleHeader::UpdateComponent(ctx, pManager);
	NKStyleWindow::UpdateComponent(ctx, pManager);
	NKStyleScrollbarH::UpdateComponent(ctx, pManager);
	NKStyleScrollbarV::UpdateComponent(ctx, pManager);
}

void NKGroup::EditComponentStyle(nk_context* ctx, NuklearUI* pManager)
{
	NKStyleHeader::EditComponentStyle(ctx, pManager);
	NKStyleWindow::EditComponentStyle(ctx, pManager);
	NKStyleScrollbarH::EditComponentStyle(ctx, pManager);
	NKStyleScrollbarV::EditComponentStyle(ctx, pManager);
}

void NKGroup::RegistCommand(const char* classname)
{
	NKBase::RegistCommand(classname);
}
