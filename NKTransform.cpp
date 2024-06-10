#include "pch.h"
#include "NKTransform.h"
#include "NuklearUI.h"

NKTransform::NKTransform()
{
	m_pivot.x = 0.f;
	m_pivot.y = 0.f;
	m_worldTransform.x = 0.f;
	m_worldTransform.y = 0.f;
	m_worldTransform.w = 0.f;
	m_worldTransform.h = 0.f;
	m_position.x = 0.f;
	m_position.y = 0.f;
}

NKTransform::NKTransform(const NKTransform& other)
{
	m_pivot.x = other.m_pivot.x;
	m_pivot.y = other.m_pivot.y;
	m_worldTransform.x = other.m_worldTransform.x;
	m_worldTransform.y = other.m_worldTransform.y;
	m_worldTransform.w = other.m_worldTransform.w;
	m_worldTransform.h = other.m_worldTransform.h;
	m_position.x = other.m_position.x;
	m_position.y = other.m_position.y;
}

NKTransform::~NKTransform()
{
}

void NKTransform::SetPivot(NKTransform* parent, NuklearUI* pManager, float x, float y)
{
	struct nk_vec2 beforePivot = m_pivot;

	if (x < 0.f) {
		x = 0.f;
	}
	else if (x > 1) {
		x = 1.f;
	}

	if (y < 0.f) {
		y = 0.f;
	}
	else if (y > 1.f) {
		y = 1.f;
	}

	m_pivot.x = x;
	m_pivot.y = y;

	if (parent) {
		m_position.x = m_worldTransform.x + (m_pivot.x * m_worldTransform.w) - (parent->GetPivot().x * parent->GetWidth());
		m_position.y = m_worldTransform.y + (m_pivot.y * m_worldTransform.h) - (parent->GetPivot().y * parent->GetHeight());
	}
	else {
		m_position.x = m_worldTransform.x + (m_pivot.x * m_worldTransform.w) - (pManager->GetPivot()->x * pManager->GetViewport()->w);
		m_position.y = m_worldTransform.y + (m_pivot.y * m_worldTransform.h) - (pManager->GetPivot()->y * pManager->GetViewport()->h);
	}
}

void NKTransform::SetPosition(NKTransform* parent, NuklearUI* pManager, float x, float y)
{
	m_position.x = x;
	m_position.y = y;

	if (parent) {
		m_worldTransform.x = x - (m_pivot.x * m_worldTransform.w) + (parent->GetPivot().x * parent->GetWidth());
		m_worldTransform.y = y - (m_pivot.y * m_worldTransform.h) + (parent->GetPivot().y * parent->GetHeight());
	}
	else {
		m_worldTransform.x = x - (m_pivot.x * m_worldTransform.w) + (pManager->GetPivot()->x * pManager->GetViewport()->w);
		m_worldTransform.y = y - (m_pivot.y * m_worldTransform.h) + (pManager->GetPivot()->y * pManager->GetViewport()->h);
	}
}

void NKTransform::SetSize(float width, float heigth)
{
	m_worldTransform.w = width;
	m_worldTransform.h = heigth;
}

struct nk_vec2 NKTransform::GetPivot()
{
	return m_pivot;
}

struct nk_vec2 NKTransform::GetPosition(NKTransform* parent, NuklearUI* pManager)
{
	if (parent) {
		m_position.x = m_worldTransform.x + (m_pivot.x * m_worldTransform.w) - (parent->GetPivot().x * parent->GetWidth());
		m_position.y = m_worldTransform.y + (m_pivot.y * m_worldTransform.h) - (parent->GetPivot().y * parent->GetHeight());
	}
	else {
		m_position.x = m_worldTransform.x + (m_pivot.x * m_worldTransform.w) - (pManager->GetPivot()->x * pManager->GetViewport()->w);
		m_position.y = m_worldTransform.y + (m_pivot.y * m_worldTransform.h) - (pManager->GetPivot()->y * pManager->GetViewport()->h);
	}
	return m_position;
}

struct nk_rect NKTransform::GetTransform()
{
	return m_worldTransform;
}

float NKTransform::GetWidth()
{
	return m_worldTransform.w;
}

float NKTransform::GetHeight()
{
	return m_worldTransform.h;
}

void NKTransform::PropertyTransform(nk_context* ctx, NKTransform* parent, NuklearUI* pManager)
{
	if (nk_tree_push(ctx, NK_TREE_TAB, "Transform", NK_MINIMIZED)) {

		PropertyVector2(ctx, "pivot", m_pivot, .0f, 1.f, 0.01f, 0.01f);
		PropertyVector2(ctx, "Position", m_position, -1920.f, 1920.f, 1.f, 1.f);

		SetPosition(parent, pManager, m_position.x, m_position.y);

		nk_label(ctx, "Rect", NK_TEXT_LEFT);
		nk_layout_row_dynamic(ctx, 22, 2);
		nk_property_float(ctx, "#W:", .0f, &m_worldTransform.w, 1920.f, 1.f, 1.f);
		nk_property_float(ctx, "#H:", .0f, &m_worldTransform.h, 1920.f, 1.f, 1.f);

		nk_tree_pop(ctx);
	}
}

struct nk_vec2* NKTransform::EditPivot()
{
	return &m_pivot;
}

struct nk_rect* NKTransform::EditTransform()
{
	return &m_worldTransform;
}