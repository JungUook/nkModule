#include "pch.h"
#include "NKTransform.h"
#include "NuklearUI.h"

NKTransform::NKTransform()
{
	m_cPivot.x = 0.f;
	m_cPivot.y = 0.f;
	m_cTransform.x = 0.f;
	m_cTransform.y = 0.f;
	m_cTransform.w = 0.f;
	m_cTransform.h = 0.f;
	m_cPosition.x = 0.f;
	m_cPosition.y = 0.f;
	m_cSyncTransform = nullptr;
	m_bMouseHover = false;
}

NKTransform::NKTransform(const NKTransform& other)
{
	m_cPivot.x = other.m_cPivot.x;
	m_cPivot.y = other.m_cPivot.y;
	m_cTransform.x = other.m_cTransform.x;
	m_cTransform.y = other.m_cTransform.y;
	m_cTransform.w = other.m_cTransform.w;
	m_cTransform.h = other.m_cTransform.h;
	m_cPosition.x = other.m_cPosition.x;
	m_cPosition.y = other.m_cPosition.y;
	m_cSyncTransform = nullptr;
	m_bMouseHover = false;
}

NKTransform::~NKTransform()
{
}

void NKTransform::SetPivot(NKTransform* parent, NuklearUI* pManager, float x, float y)
{
	struct nk_vec2 beforePivot = m_cPivot;

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

	m_cPivot.x = x;
	m_cPivot.y = y;

	if (parent) {
		m_cPosition.x = m_cTransform.x + (m_cPivot.x * m_cTransform.w) - (parent->GetPivot().x * parent->GetWidth());
		m_cPosition.y = m_cTransform.y + (m_cPivot.y * m_cTransform.h) - (parent->GetPivot().y * parent->GetHeight());
	}
	else {
		m_cPosition.x = m_cTransform.x + (m_cPivot.x * m_cTransform.w) - (pManager->GetPivot()->x * pManager->GetViewport()->w);
		m_cPosition.y = m_cTransform.y + (m_cPivot.y * m_cTransform.h) - (pManager->GetPivot()->y * pManager->GetViewport()->h);
	}
}

void NKTransform::SetPosition(NKTransform* parent, NuklearUI* pManager, float x, float y)
{
	m_cPosition.x = x;
	m_cPosition.y = y;

	if (parent) {
		m_cTransform.x = x - (m_cPivot.x * m_cTransform.w) + (parent->GetPivot().x * parent->GetWidth());
		m_cTransform.y = y - (m_cPivot.y * m_cTransform.h) + (parent->GetPivot().y * parent->GetHeight());
	}
	else {
		m_cTransform.x = x - (m_cPivot.x * m_cTransform.w) + (pManager->GetPivot()->x * pManager->GetViewport()->w);
		m_cTransform.y = y - (m_cPivot.y * m_cTransform.h) + (pManager->GetPivot()->y * pManager->GetViewport()->h);
	}
}

void NKTransform::SetSize(float width, float heigth)
{
	m_cTransform.w = width;
	m_cTransform.h = heigth;
}

struct nk_vec2 NKTransform::GetPivot()
{
	return m_cPivot;
}

struct nk_vec2 NKTransform::GetPosition(NKTransform* parent, NuklearUI* pManager)
{
	if (parent) {
		m_cPosition.x = m_cTransform.x + (m_cPivot.x * m_cTransform.w) - (parent->GetPivot().x * parent->GetWidth());
		m_cPosition.y = m_cTransform.y + (m_cPivot.y * m_cTransform.h) - (parent->GetPivot().y * parent->GetHeight());
	}
	else {
		m_cPosition.x = m_cTransform.x + (m_cPivot.x * m_cTransform.w) - (pManager->GetPivot()->x * pManager->GetViewport()->w);
		m_cPosition.y = m_cTransform.y + (m_cPivot.y * m_cTransform.h) - (pManager->GetPivot()->y * pManager->GetViewport()->h);
	}
	return m_cPosition;
}

struct nk_rect NKTransform::GetTransform()
{
	return m_cTransform;
}

float NKTransform::GetWidth()
{
	return m_cTransform.w;
}

float NKTransform::GetHeight()
{
	return m_cTransform.h;
}

void NKTransform::PropertyTransform(nk_context* ctx, NKTransform* parent, NuklearUI* pManager)
{
	if (nk_tree_push(ctx, NK_TREE_TAB, "Transform", NK_MINIMIZED)) {

		PropertyVector2(ctx, "pivot", m_cPivot, .0f, 1.f, 0.01f, 0.01f);
		PropertyVector2(ctx, "Position", m_cPosition, -1920.f, 1920.f, 1.f, 1.f);
		SetPosition(parent, pManager, m_cPosition.x, m_cPosition.y);
		PropertyTransform2(ctx, "Transform", m_cTransform, -1920.f, 1920.f, 1.f, 1.f);
		nk_tree_pop(ctx);
	}
}

struct nk_vec2* NKTransform::RefPivot()
{
	return &m_cPivot;
}

struct nk_rect* NKTransform::RefTransform()
{
	return &m_cTransform;
}