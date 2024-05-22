#include "pch.h"
#include "NKImage.h"

NKImage::NKImage()
{
	m_type = eIMAGE;
	memset(&m_image, 0, sizeof(m_image));

	m_pivot.x = 0.f;
	m_pivot.y = 0.f;
	m_worldTransform.x = 50.f;
	m_worldTransform.y = 50.f;
	m_worldTransform.w = 100.f;
	m_worldTransform.h = 100.f;

	std::string className = getClassName().c_str();
	m_cName = className.c_str();
	strcpy_s(m_primaryName, m_cName);
}

NKImage::~NKImage()
{
}

void NKImage::Layout(nk_context* ctx)
{
	nk_image(ctx, m_image);
}
