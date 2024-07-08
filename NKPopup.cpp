#include "pch.h"
#include "NKPopup.h"

NKPopup::NKPopup() : NKBase(), NKBaseWindow(), NKStyleHeader(), NKStyleWindow()
{
	m_type = ePOPUP;
	m_popupType = NK_POPUP_STATIC;
	m_flags = NK_WINDOW_TITLE;
}

NKPopup::NKPopup(nk_context* ctx, NuklearUI* pManager) : NKBase(ctx, pManager), NKBaseWindow(), NKStyleHeader(ctx, &m_style), NKStyleWindow(ctx, &m_style)
{
	m_type = ePOPUP;
	m_popupType = NK_POPUP_STATIC;
	m_flags = NK_WINDOW_TITLE;

	m_cTransform.x = 50.f;
	m_cTransform.y = 50.f;
	m_cTransform.w = 150.f;
	m_cTransform.h = 100.f;
}

NKPopup::NKPopup(const NKPopup& other) : NKBase(other), NKBaseWindow(other), NKStyleHeader(other, m_ctx, &m_style), NKStyleWindow(other, m_ctx, &m_style)
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
	if (nk_popup_begin(ctx, m_popupType, m_cBaseName, m_flags, GetTransform()))
	{
		for (std::list<NKBase*>::iterator it = m_pChildList.begin(); it != m_pChildList.end(); ++it)
		{
			(*it)->Update(ctx);
		}
		nk_popup_end(ctx);
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
}

void NKPopup::EditComponentStyle(nk_context* ctx, NuklearUI* pManager)
{
	NKStyleHeader::EditComponentStyle(ctx, pManager);
	NKStyleWindow::EditComponentStyle(ctx, pManager);
}

void NKPopup::RegistCommand(const char* classname)
{
	NKBase::RegistCommand(classname);
}
