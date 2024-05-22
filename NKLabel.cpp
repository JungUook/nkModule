#include "pch.h"
#include "NKLabel.h"

NKLabel::NKLabel()
{
	m_type = eLABEL;
	m_flags = NK_TEXT_CENTERED;
	memset(m_content, 0, sizeof(m_content));

	m_pivot.x = 0.f;
	m_pivot.y = 0.f;
	m_worldTransform.x = 50.f;
	m_worldTransform.y = 50.f;
	m_worldTransform.w = 150.f;
	m_worldTransform.h = 60.f;

	std::string className = getClassName().c_str();
	m_cName = className.c_str();
	strcpy_s(m_primaryName, m_cName);
}

NKLabel::~NKLabel()
{
}

void NKLabel::Layout(nk_context* ctx)
{
	nk_label(ctx, m_content, m_flags);
}
