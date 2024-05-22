#include "pch.h"
#include "NKPopup.h"

NKPopup::NKPopup()
{
	m_type = ePOPUP;
	m_popupType = NK_POPUP_STATIC;
	m_flags = NK_WINDOW_TITLE | NK_WINDOW_MOVABLE;

	m_pivot.x = 0.f;
	m_pivot.y = 0.f;
	m_worldTransform.x = 50.f;
	m_worldTransform.y = 50.f;
	m_worldTransform.w = 150.f;
	m_worldTransform.h = 100.f;
}

NKPopup::~NKPopup()
{
}

void NKPopup::Layout(nk_context* ctx)
{
	if (nk_popup_begin(ctx, m_popupType, m_primaryName, m_flags, GetTransform()))
	{
		for (std::list<NKBase*>::iterator it = m_pChildList.begin(); it != m_pChildList.end(); ++it)
		{
			(*it)->Update(ctx);
		}
		nk_popup_end(ctx);
	}
}
