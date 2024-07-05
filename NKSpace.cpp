#include "pch.h"
#include "NKSpace.h"

NKSpace::NKSpace() : NKBase()
{
	m_layoutFormat = NK_STATIC;
	m_widgetCount = 0;
	m_type = eSPACE;
	m_dynamicCount = 1;
}

NKSpace::NKSpace(nk_context* ctx, NuklearUI* pManager) : NKBase(ctx, pManager)
{
	m_layoutFormat	= NK_STATIC;
	m_widgetCount	= 0;
	m_type			= eSPACE;
	m_dynamicCount	= 1;
}

NKSpace::NKSpace(const NKSpace& other) : NKBase(other)
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
		nk_layout_space_begin(ctx, m_layoutFormat, m_cTransform.h, m_widgetCount);
		for (std::list<NKBase*>::iterator it = m_pChildList.begin(); it != m_pChildList.end(); ++it)
		{
			
			nk_layout_space_push(ctx, (*it)->GetTransform());
			(*it)->CheckMouseHover(ctx);
			(*it)->Update(ctx);
		}
		nk_layout_space_end(ctx);
	}
	else
	{
		nk_layout_row_dynamic(ctx, m_cTransform.h, m_dynamicCount);
		for (std::list<NKBase*>::iterator it = m_pChildList.begin(); it != m_pChildList.end(); ++it)
		{
			(*it)->CheckMouseHover(ctx);
			(*it)->Update(ctx);
		}
	}
}

void NKSpace::SafeRenderStart(nk_context* ctx)
{
	//UpdateComponent(ctx, m_pManager);
}

void NKSpace::SafeRenderEnd(nk_context* ctx)
{
}

void NKSpace::EditInfo(nk_context* ctx)
{
	nk_layout_row_dynamic(ctx, 22, 1);

	nk_label(ctx, "Space_Type", NK_TEXT_LEFT);
	if (nk_option_label(ctx, "STATIC", m_layoutFormat == NK_STATIC)) m_layoutFormat = NK_STATIC;
	if (nk_option_label(ctx, "DYNAMIC", m_layoutFormat == NK_DYNAMIC)) m_layoutFormat = NK_DYNAMIC;

	if (m_layoutFormat == NK_DYNAMIC)
	{
		nk_label(ctx, "Widget_Count", NK_TEXT_LEFT);
		nk_property_int(ctx, "#Count:", 1, &m_dynamicCount, 16, 1, 1);
	}


	nk_layout_row_dynamic(ctx, 22, 1);
	nk_label(ctx, "Create_UI", NK_TEXT_LEFT);
	if (nk_button_label(ctx, "Group"))
	{
		CreateUI("NKGroup");
	}
	if (nk_button_label(ctx, "Popup"))
	{
		CreateUI("NKPopup");
	}
	if (nk_button_label(ctx, "Combo"))
	{
		CreateUI("NKCombo");
	}
	if (nk_button_label(ctx, "Button"))
	{
		CreateUI("NKButton");
	}
	if (nk_button_label(ctx, "InputBox"))
	{
		CreateUI("NKEdit");
	}
	if (nk_button_label(ctx, "Image"))
	{
		CreateUI("NKImage");
	}
	if (nk_button_label(ctx, "Label"))
	{
		CreateUI("NKLabel");
	}
	if (nk_button_label(ctx, "CheckBox"))
	{
		CreateUI("NKCheckbox");
	}
	if (nk_button_label(ctx, "Slider"))
	{
		CreateUI("NKSlider");
	}
	if (nk_button_label(ctx, "Progress"))
	{
		CreateUI("NKProgress");
	}
	if (nk_button_label(ctx, "Selectable"))
	{
		CreateUI("NKSelectable");
	}
	if (nk_button_label(ctx, "Tree"))
	{
		CreateUI("NKTree");
	}
	if (nk_button_label(ctx, "Chart"))
	{
		CreateUI("NKChart");
	}
	if (nk_button_label(ctx, "Tooltip"))
	{
		CreateUI("NKTooltip");
	}
	if (nk_button_label(ctx, "Menu"))
	{
		CreateUI("NKMenu");
	}
	if (nk_button_label(ctx, "ColorPicker"))
	{
		CreateUI("NKColorPicker");
	}
}

void NKSpace::EditStyle(nk_context* ctx)
{
	nk_label(ctx, "None", NK_TEXT_LEFT);
}

void NKSpace::SetLayout(int type)
{
	m_layoutFormat = (nk_layout_format)type;
}

void NKSpace::LSetLayout(luabridge::LuaRef ref)
{
	CHECK_LUA_REF(ref);
	int type = ref.cast<int>();
	SetLayout(type);
}

bool NKSpace::CSetLayout(void* param)
{
	int* type = static_cast<int*>(param);

	if (type) {
		SetLayout(*type);
		return true;
	}
	return false;
}

void NKSpace::SetCols(int cols)
{
	m_dynamicCount = cols;
}

void NKSpace::LSetCols(luabridge::LuaRef ref)
{
	CHECK_LUA_REF(ref);
	int cols = ref.cast<int>();
	SetCols(cols);
}

bool NKSpace::CSetCols(void* param)
{
	int* cols = static_cast<int*>(param);

	if (cols) {
		SetCols(*cols);
		return true;
	}
	return false;
}

void NKSpace::RegistCommand()
{
	NKBase::RegistCommand();

	MAKE_INTERFACE(m_mapFunc, this, NKSpace::CSetLayout, "NKSpace");
	MAKE_INTERFACE(m_mapFunc, this, NKSpace::CSetCols, "NKSpace");
}
