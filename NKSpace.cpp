#include "pch.h"
#include "NKSpace.h"

NKSpace::NKSpace()
	: m_layoutFormat(NK_STATIC)
	, m_height(60)
	, m_widgetCount(0)
{
	m_type = eSPACE;
}

NKSpace::~NKSpace()
{
}

void NKSpace::Layout(nk_context* ctx)
{
	m_widgetCount = m_pChildList.size();

	nk_layout_space_begin(ctx, m_layoutFormat, m_height, m_widgetCount);
	for (std::list<NKBase*>::iterator it = m_pChildList.begin(); it != m_pChildList.end(); ++it)
	{
		nk_layout_space_push(ctx, (*it)->GetTransform());
		(*it)->Update(ctx);
	}
	nk_layout_space_end(ctx);
}
