#include "pch.h"
#include "NKTransform.h"
#include "NuklearUI.h"

NKTransform::NKTransform()
{
	m_sPivot.x = 0.f;
	m_sPivot.y = 0.f;
	m_sTransform.x = 0.f;
	m_sTransform.y = 0.f;
	m_sTransform.w = 0.f;
	m_sTransform.h = 0.f;
	m_sPosition.x = 0.f;
	m_sPosition.y = 0.f;
	m_sSyncTransform = nullptr;
	m_bMouseHover = false;
}

NKTransform::NKTransform(const NKTransform& other)
{
	m_sPivot.x = other.m_sPivot.x;
	m_sPivot.y = other.m_sPivot.y;
	m_sTransform.x = other.m_sTransform.x;
	m_sTransform.y = other.m_sTransform.y;
	m_sTransform.w = other.m_sTransform.w;
	m_sTransform.h = other.m_sTransform.h;
	m_sPosition.x = other.m_sPosition.x;
	m_sPosition.y = other.m_sPosition.y;
	m_sSyncTransform = nullptr;
	m_bMouseHover = false;
}

NKTransform::~NKTransform()
{
}

void NKTransform::SetPivot(NKTransform* parent, NuklearUI* pManager, float x, float y)
{
	struct nk_vec2 beforePivot = m_sPivot;

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

	m_sPivot.x = x;
	m_sPivot.y = y;

	if (parent) {
		m_sPosition.x = m_sTransform.x + (m_sPivot.x * m_sTransform.w) - (parent->GetPivot().x * parent->GetWidth());
		m_sPosition.y = m_sTransform.y + (m_sPivot.y * m_sTransform.h) - (parent->GetPivot().y * parent->GetHeight());
	}
	else {
		m_sPosition.x = m_sTransform.x + (m_sPivot.x * m_sTransform.w) - (pManager->GetPivot()->x * pManager->GetViewport()->w);
		m_sPosition.y = m_sTransform.y + (m_sPivot.y * m_sTransform.h) - (pManager->GetPivot()->y * pManager->GetViewport()->h);
	}
}

void NKTransform::SetPosition(NKTransform* parent, NuklearUI* pManager, float x, float y)
{
	m_sPosition.x = x;
	m_sPosition.y = y;

	if (parent) {
		m_sTransform.x = x - (m_sPivot.x * m_sTransform.w) + (parent->GetPivot().x * parent->GetWidth());
		m_sTransform.y = y - (m_sPivot.y * m_sTransform.h) + (parent->GetPivot().y * parent->GetHeight());
	}
	else {
		m_sTransform.x = x - (m_sPivot.x * m_sTransform.w) + (pManager->GetPivot()->x * pManager->GetViewport()->w);
		m_sTransform.y = y - (m_sPivot.y * m_sTransform.h) + (pManager->GetPivot()->y * pManager->GetViewport()->h);
	}
}

void NKTransform::SetSize(float width, float heigth)
{
	m_sTransform.w = width;
	m_sTransform.h = heigth;
}

struct nk_vec2 NKTransform::GetPivot()
{
	return m_sPivot;
}

struct nk_vec2 NKTransform::GetPosition(NKTransform* parent, NuklearUI* pManager)
{
	if (parent) {
		m_sPosition.x = m_sTransform.x + (m_sPivot.x * m_sTransform.w) - (parent->GetPivot().x * parent->GetWidth());
		m_sPosition.y = m_sTransform.y + (m_sPivot.y * m_sTransform.h) - (parent->GetPivot().y * parent->GetHeight());
	}
	else {
		m_sPosition.x = m_sTransform.x + (m_sPivot.x * m_sTransform.w) - (pManager->GetPivot()->x * pManager->GetViewport()->w);
		m_sPosition.y = m_sTransform.y + (m_sPivot.y * m_sTransform.h) - (pManager->GetPivot()->y * pManager->GetViewport()->h);
	}
	return m_sPosition;
}

struct nk_rect NKTransform::GetTransform()
{
	return m_sTransform;
}

float NKTransform::GetWidth()
{
	return m_sTransform.w;
}

float NKTransform::GetHeight()
{
	return m_sTransform.h;
}

void NKTransform::PropertyTransform(nk_context* ctx, NKTransform* parent, NuklearUI* pManager)
{
	if (nk_tree_push(ctx, NK_TREE_TAB, "Transform", NK_MINIMIZED)) {

		PropertyVector2(ctx, "pivot", m_sPivot, .0f, 1.f, 0.01f, 0.01f);
		PropertyVector2(ctx, "Position", m_sPosition, -1920.f, 1920.f, 1.f, 1.f);
		SetPosition(parent, pManager, m_sPosition.x, m_sPosition.y);
		PropertyTransform2(ctx, "Transform", m_sTransform, -1920.f, 1920.f, 1.f, 1.f);
		nk_tree_pop(ctx);
	}
}

struct nk_vec2* NKTransform::RefPivot()
{
	return &m_sPivot;
}

struct nk_rect* NKTransform::RefTransform()
{
	return &m_sTransform;
}