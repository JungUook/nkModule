#include "pch.h"
#include "NKGroup.h"

NKGroup::NKGroup() : NKBase()
{
	m_layoutFormat	= NK_DYNAMIC;
	m_width			= 0;
	m_height		= 0;	
	m_cols			= 0;
	m_ratio			= nullptr;
	m_type			= eGROUP;
	m_flags			= 0;
}

NKGroup::NKGroup(const NKGroup& other) : NKBase()
{
	m_layoutFormat	= other.m_layoutFormat;
	m_width			= other.m_width;
	m_height		= other.m_height;
	m_cols			= other.m_cols;
	m_ratio			= other.m_ratio;
	m_type			= other.m_type;
	m_flags			= other.m_flags;
}

NKGroup::~NKGroup()
{
}

void NKGroup::Layout(nk_context* ctx)
{
	if (nk_group_begin(ctx, m_baseName, m_flags))
	{
		if (m_layoutFormat == NK_DYNAMIC)
		{
			nk_layout_row_dynamic(ctx, (float)m_height, m_cols);
		}
		else
		{
			nk_layout_row_static(ctx, (float)m_height, m_width, m_cols);
		}
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
	EditInfoWindow();
}

void NKGroup::EditStyle()
{
	NKBase::EditStyle();
}
