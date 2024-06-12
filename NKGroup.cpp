#include "pch.h"
#include "NKGroup.h"

NKGroup::NKGroup() : NKBase()
{
	m_type			= eGROUP;
	m_flags			= 0;
}

NKGroup::NKGroup(const NKGroup& other) : NKBase()
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
	m_worldTransform = ctx->current->layout->row.item;
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
}
