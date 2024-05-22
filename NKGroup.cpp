#include "pch.h"
#include "NKGroup.h"

NKGroup::NKGroup()
{
	m_layoutFormat	= NK_DYNAMIC;
	m_width			= 0;
	m_height		= 0;	
	m_cols			= 0;
	m_ratio			= nullptr;
	m_type			= eGROUP;
	m_flags			= 0;

	std::string className = getClassName().c_str();
	m_cName = className.c_str();
	strcpy_s(m_primaryName, m_cName);
}

NKGroup::~NKGroup()
{
}

void NKGroup::Layout(nk_context* ctx)
{
	if (nk_group_begin(ctx, m_primaryName, m_flags))
	{
		if (m_layoutFormat == NK_DYNAMIC)
		{
			nk_layout_row_dynamic(ctx, m_height, m_cols);
		}
		else
		{
			nk_layout_row_static(ctx, m_height, m_width, m_cols);
		}
		for (std::list<NKBase*>::iterator it = m_pChildList.begin(); it != m_pChildList.end(); ++it)
		{
			(*it)->Update(ctx);
		}

		nk_group_end(ctx);
	}
}
