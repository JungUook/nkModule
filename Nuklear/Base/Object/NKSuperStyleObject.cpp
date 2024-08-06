#include "pch.h"
#include "NKSuperStyleObject.h"

NKSuperStyleObject::NKSuperStyleObject()
	: NKBase()
	, NKStyleButton()
	, NKStyleChart()
	, NKStyleCheckbox()
	, NKStyleCombo()
	, NKStyleContextualButton()
	, NKStyleEdit()
	, NKStyleHeader()
	, NKStyleMenuButton()
	, NKStyleOption()
	, NKStyleProgress()
	, NKStyleProperty()
	, NKStyleScrollbarH()
	, NKStyleScrollbarV()
	, NKStyleSelectedable()
	, NKStyleSlider()
	, NKStyleTab()
	, NKStyleText()
	, NKStyleWindow()
{
}

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
	, NKStyleButton(other, m_ctx, &m_style)
	, NKStyleChart(other, m_ctx, &m_style)
	, NKStyleCheckbox(other, m_ctx, &m_style)
	, NKStyleCombo(other, m_ctx, &m_style)
	, NKStyleContextualButton(other, m_ctx, &m_style)
	, NKStyleEdit(other, m_ctx, &m_style)
	, NKStyleHeader(other, m_ctx, &m_style)
	, NKStyleMenuButton(other, m_ctx, &m_style)
	, NKStyleOption(other, m_ctx, &m_style)
	, NKStyleProgress(other, m_ctx, &m_style)
	, NKStyleProperty(other, m_ctx, &m_style)
	, NKStyleScrollbarH(other, m_ctx, &m_style)
	, NKStyleScrollbarV(other, m_ctx, &m_style)
	, NKStyleSelectedable(other, m_ctx, &m_style)
	, NKStyleSlider(other, m_ctx, &m_style)
	, NKStyleTab(other, m_ctx, &m_style)
	, NKStyleText(other, m_ctx, &m_style)
	, NKStyleWindow(other, m_ctx, &m_style)
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

void NKSuperStyleObject::SafeRenderStart(nk_context* ctx)
{
	UpdateComponent(m_ctx, m_pManager);
}

void NKSuperStyleObject::SafeRenderEnd(nk_context* ctx)
{
}

void NKSuperStyleObject::EditInfo(nk_context* ctx)
{
	nk_layout_row_dynamic(ctx, 22, 1);
	nk_label(ctx, "Create_UI", NK_TEXT_LEFT);
	if (nk_button_label(ctx, "Space"))
	{
		CreateUI("NKSpace");
	}
}

void NKSuperStyleObject::EditStyle(nk_context* ctx)
{
	NKBase::EditStyle(ctx);
	EditComponentStyle(ctx, m_pManager);
}

void NKSuperStyleObject::UpdateComponent(nk_context* ctx, NuklearUI* pManager)
{
	NKStyleButton::UpdateComponent(ctx, pManager);
	NKStyleChart::UpdateComponent(ctx, pManager);
	NKStyleCheckbox::UpdateComponent(ctx, pManager);
	NKStyleCombo::UpdateComponent(ctx, pManager);
	NKStyleContextualButton::UpdateComponent(ctx, pManager);
	NKStyleEdit::UpdateComponent(ctx, pManager);
	NKStyleHeader::UpdateComponent(ctx, pManager);
	NKStyleMenuButton::UpdateComponent(ctx, pManager);
	NKStyleOption::UpdateComponent(ctx, pManager);
	NKStyleProgress::UpdateComponent(ctx, pManager);
	NKStyleProperty::UpdateComponent(ctx, pManager);
	NKStyleScrollbarH::UpdateComponent(ctx, pManager);
	NKStyleScrollbarV::UpdateComponent(ctx, pManager);
	NKStyleSelectedable::UpdateComponent(ctx, pManager);
	NKStyleSlider::UpdateComponent(ctx, pManager);
	NKStyleTab::UpdateComponent(ctx, pManager);
	NKStyleText::UpdateComponent(ctx, pManager);
	NKStyleWindow::UpdateComponent(ctx, pManager);
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

void NKSuperStyleObject::RegistCommand(const char* classname)
{
	NKBase::RegistCommand(classname);
}
