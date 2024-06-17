#include "pch.h"
#include "NKSuperStyleObject.h"

NKSuperStyleObject::NKSuperStyleObject(nk_context* ctx, NuklearUI* pManager)
	: NKBase(ctx, pManager)
	, NKStyleButton(ctx, &m_style)
	, NKStyleChart(ctx, &m_style)
	, NKStyleCheckbox(ctx, &m_style)
	, NKStyleCombo(ctx, &m_style)
	, NKStyleContextualButton(ctx, &m_style)
	, NKStyleEdit(ctx, &m_style)
	, NKStyleHeader(ctx, &m_style)
	, NKStyleMenuButton(ctx, &m_style)
	, NKStyleOption(ctx, &m_style)
	, NKStyleProgress(ctx, &m_style)
	, NKStyleProperty(ctx, &m_style)
	, NKStyleScrollbarH(ctx, &m_style)
	, NKStyleScrollbarV(ctx, &m_style)
	, NKStyleSelectedable(ctx, &m_style)
	, NKStyleSlider(ctx, &m_style)
	, NKStyleTab(ctx, &m_style)
	, NKStyleText(ctx, &m_style)
	, NKStyleWindow(ctx, &m_style)
{
}

NKSuperStyleObject::NKSuperStyleObject(const NKSuperStyleObject& other)
	: NKBase(other)
	, NKStyleButton(other)
	, NKStyleChart(other)
	, NKStyleCheckbox(other)
	, NKStyleCombo(other)
	, NKStyleContextualButton(other)
	, NKStyleEdit(other)
	, NKStyleHeader(other)
	, NKStyleMenuButton(other)
	, NKStyleOption(other)
	, NKStyleProgress(other)
	, NKStyleProperty(other)
	, NKStyleScrollbarH(other)
	, NKStyleScrollbarV(other)
	, NKStyleSelectedable(other)
	, NKStyleSlider(other)
	, NKStyleTab(other)
	, NKStyleText(other)
	, NKStyleWindow(other)
{
}

NKSuperStyleObject::~NKSuperStyleObject()
{
}

void NKSuperStyleObject::Layout(nk_context* ctx)
{
	for (std::list<NKBase*>::iterator it = m_pChildList.begin(); it != m_pChildList.end(); ++it)
	{
		(*it)->Update(ctx);
	}
}

void NKSuperStyleObject::EditInfo()
{
	nk_layout_row_dynamic(m_ctx, 22, 1);
	nk_label(m_ctx, "Create_UI", NK_TEXT_LEFT);
	if (nk_button_label(m_ctx, "Space"))
	{
		CreateUI("NKSpace");
	}
}

void NKSuperStyleObject::EditStyle()
{
	NKBase::EditStyle();
	EditComponentStyle(m_ctx, m_pManager);
}

void NKSuperStyleObject::EditComponentStyle(nk_context* ctx, NuklearUI* pManager)
{
	NKStyleButton::EditComponentStyle(ctx, pManager);
	NKStyleChart::EditComponentStyle(ctx, pManager);
	NKStyleCheckbox::EditComponentStyle(ctx, pManager);
	NKStyleCombo::EditComponentStyle(ctx, pManager);
	NKStyleContextualButton::EditComponentStyle(ctx, pManager);
	NKStyleEdit::EditComponentStyle(ctx, pManager);
	NKStyleHeader::EditComponentStyle(ctx, pManager);
	NKStyleMenuButton::EditComponentStyle(ctx, pManager);
	NKStyleOption::EditComponentStyle(ctx, pManager);
	NKStyleProgress::EditComponentStyle(ctx, pManager);
	NKStyleProperty::EditComponentStyle(ctx, pManager);
	NKStyleScrollbarH::EditComponentStyle(ctx, pManager);
	NKStyleScrollbarV::EditComponentStyle(ctx, pManager);
	NKStyleSelectedable::EditComponentStyle(ctx, pManager);
	NKStyleSlider::EditComponentStyle(ctx, pManager);
	NKStyleTab::EditComponentStyle(ctx, pManager);
	NKStyleText::EditComponentStyle(ctx, pManager);
	NKStyleWindow::EditComponentStyle(ctx, pManager);
}
