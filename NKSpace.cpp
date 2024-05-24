#include "pch.h"
#include "NKSpace.h"

NKSpace::NKSpace()
{
	m_layoutFormat	= NK_STATIC;
	m_widgetCount	= 0;
	m_type			= eSPACE;

	std::string className = getClassName().c_str();
	m_cName = className.c_str();
	strcpy_s(m_primaryName, m_cName);
}

NKSpace::~NKSpace()
{
}

void NKSpace::Layout(nk_context* ctx)
{
	m_widgetCount = m_pChildList.size();

	if (m_layoutFormat == NK_STATIC)
	{
		nk_layout_space_begin(ctx, m_layoutFormat, m_worldTransform.h, m_widgetCount);
		for (std::list<NKBase*>::iterator it = m_pChildList.begin(); it != m_pChildList.end(); ++it)
		{
			nk_layout_space_push(ctx, (*it)->GetTransform());
			(*it)->Update(ctx);
		}
		nk_layout_space_end(ctx);
	}
	else
	{
		nk_layout_row_dynamic(ctx, m_worldTransform.h, m_widgetCount);
		for (std::list<NKBase*>::iterator it = m_pChildList.begin(); it != m_pChildList.end(); ++it)
		{
			nk_layout_space_push(ctx, (*it)->GetTransform());
			(*it)->Update(ctx);
		}
	}
}

void NKSpace::SetLayout(int type)
{
	m_layoutFormat = (nk_layout_format)type;
}
