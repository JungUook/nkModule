#include "pch.h"
#include "NKStyleHeader.h"
#include "NuklearUI.h"

NKStyleHeader::NKStyleHeader(nk_context* ctx, nk_style* style)
{
	m_pComponent = new ComponentHeader(&style->window.header, &ctx->style.window.header);
}

NKStyleHeader::~NKStyleHeader()
{
	delete m_pComponent;
	m_pComponent = nullptr;
}

void NKStyleHeader::EditComponentStyle(nk_context* ctx, NuklearUI* pManager)
{
	m_pComponent->CustomComponentsEditor(ctx, pManager);
}
