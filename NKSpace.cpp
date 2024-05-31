#include "pch.h"
#include "NKSpace.h"

NKSpace::NKSpace() : NKBase()
{
	m_layoutFormat	= NK_STATIC;
	m_widgetCount	= 0;
	m_type			= eSPACE;
	m_dynamicCount	= 1;
}

NKSpace::NKSpace(const NKSpace& other) : NKBase()
{
	m_layoutFormat	= other.m_layoutFormat;
	m_widgetCount	= other.m_widgetCount;	
	m_type			= other.m_type;	
	m_dynamicCount	= other.m_dynamicCount;	
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
		nk_layout_row_dynamic(ctx, m_worldTransform.h, m_dynamicCount);
		for (std::list<NKBase*>::iterator it = m_pChildList.begin(); it != m_pChildList.end(); ++it)
		{
			nk_layout_space_push(ctx, (*it)->GetTransform());
			(*it)->Update(ctx);
		}
	}
}

void NKSpace::EditInfo()
{
	nk_layout_row_dynamic(m_ctx, 22, 1);

	nk_label(m_ctx, "Space_Type", NK_TEXT_LEFT);
	if (nk_option_label(m_ctx, "STATIC", m_layoutFormat == NK_STATIC)) m_layoutFormat = NK_STATIC;
	if (nk_option_label(m_ctx, "DYNAMIC", m_layoutFormat == NK_DYNAMIC)) m_layoutFormat = NK_DYNAMIC;

	if (m_layoutFormat == NK_DYNAMIC)
	{
		nk_label(m_ctx, "Widget_Count", NK_TEXT_LEFT);
		nk_property_int(m_ctx, "#Count:", 1, &m_dynamicCount, 16, 1, 1);
	}


	nk_layout_row_dynamic(m_ctx, 22, 1);
	nk_label(m_ctx, "Create_UI", NK_TEXT_LEFT);
	if (nk_button_label(m_ctx, "Group"))
	{
		CreateUI("NKGroup");
	}
	if (nk_button_label(m_ctx, "Popup"))
	{
		CreateUI("NKPopup");
	}
	if (nk_button_label(m_ctx, "Combo"))
	{
		CreateUI("NKCombo");
	}
	if (nk_button_label(m_ctx, "Button"))
	{
		CreateUI("NKButton");
	}
	if (nk_button_label(m_ctx, "InputBox"))
	{
		CreateUI("NKEdit");
	}
	if (nk_button_label(m_ctx, "Image"))
	{
		CreateUI("NKImage");
	}
	if (nk_button_label(m_ctx, "Label"))
	{
		CreateUI("NKLabel");
	}
	if (nk_button_label(m_ctx, "CheckBox"))
	{
		CreateUI("NKCheckbox");
	}
}

void NKSpace::EditStyle()
{
	nk_label(m_ctx, "None", NK_TEXT_LEFT);
}

void NKSpace::SetLayout(int type)
{
	m_layoutFormat = (nk_layout_format)type;
}

void NKSpace::SetCols(int cols)
{
	m_dynamicCount = cols;
}
