#include "pch.h"
#include "NKPopup.h"

NKPopup::NKPopup() : NKBase(), NKBaseWindow(), NKHandler(), NKStyleHeader(), NKStyleWindow(), NKStyleScrollbarH(), NKStyleScrollbarV()
{
	m_type = ePOPUP;
	m_popupType = NK_POPUP_STATIC;
	m_flags = NK_WINDOW_TITLE;
}

NKPopup::NKPopup(nk_context* ctx, NuklearUI* pManager) : NKBase(ctx, pManager), NKBaseWindow(), NKHandler(), NKStyleHeader(ctx, &m_style), NKStyleWindow(ctx, &m_style), NKStyleScrollbarH(ctx, &m_style), NKStyleScrollbarV(ctx, &m_style)
{
	m_type = ePOPUP;
	m_popupType = NK_POPUP_STATIC;
	m_flags = NK_WINDOW_TITLE;

	m_sTransform.w = 150.f;
	m_sTransform.h = 100.f;
}

NKPopup::NKPopup(const NKPopup& other) : NKBase(other), NKBaseWindow(other), NKHandler(other), NKStyleHeader(other, m_ctx, &m_style), NKStyleWindow(other, m_ctx, &m_style), NKStyleScrollbarH(other, m_ctx, &m_style), NKStyleScrollbarV(other, m_ctx, &m_style)
{
	m_type = other.m_type;
	m_popupType = other.m_popupType;
	m_flags = other.m_flags;
}

NKPopup::~NKPopup()
{
}

void NKPopup::Layout(nk_context* ctx)
{
	char primary_name[256] = { 0, };
	strncpy_s(primary_name, m_sPrimaryName.c_str(), 256);
	primary_name[255] = '\0';

	if (nk_popup_begin_titled(ctx, m_popupType, primary_name, m_sWindowName.c_str(), m_flags, GetTransform()))
	{
		for (std::list<NKBase*>::iterator it = m_pChildList.begin(); it != m_pChildList.end(); ++it)
		{
			(*it)->Update(ctx);
		}
		nk_popup_end(ctx);
	}
	else {
		m_bActive = false;
		CallEvent(m_pLuaManager);
	}
}

void NKPopup::SafeRenderStart(nk_context* ctx)
{
	UpdateComponent(ctx, m_pManager);
}

void NKPopup::SafeRenderEnd(nk_context* ctx)
{
}

void NKPopup::EditInfo(nk_context* ctx)
{
	EditInfoWindowProperty(ctx, m_flags);

	if (nk_tree_push(ctx, NK_TREE_NODE, "Create UI", NK_MINIMIZED)) {
		if (nk_button_label(ctx, "Space"))
		{
			CreateUI("NKSpace");
		}
		nk_tree_pop(ctx);
	}
	EditInfoData(ctx, m_pManager, m_pLuaManager);
}

void NKPopup::EditStyle(nk_context* ctx)
{
	NKBase::EditStyle(ctx);
	EditComponentStyle(ctx, m_pManager);
}

void NKPopup::UpdateComponent(nk_context* ctx, NuklearUI* pManager)
{
	NKStyleHeader::UpdateComponent(ctx, pManager);
	NKStyleWindow::UpdateComponent(ctx, pManager);
	NKStyleScrollbarH::UpdateComponent(ctx, pManager);
	NKStyleScrollbarV::UpdateComponent(ctx, pManager);
}

void NKPopup::EditComponentStyle(nk_context* ctx, NuklearUI* pManager)
{
	NKStyleHeader::EditComponentStyle(ctx, pManager);
	NKStyleWindow::EditComponentStyle(ctx, pManager);
	NKStyleScrollbarH::EditComponentStyle(ctx, pManager);
	NKStyleScrollbarV::EditComponentStyle(ctx, pManager);
}

void NKPopup::RegistCommand(const char* classname)
{
	NKBase::RegistCommand(classname);
}
