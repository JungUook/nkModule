#include "pch.h"
#include "NKImage.h"

NKImage::NKImage()
{
	m_type = eIMAGE;

	m_pivot.x = 0.f;
	m_pivot.y = 0.f;
	m_worldTransform.x = 50.f;
	m_worldTransform.y = 50.f;
	m_worldTransform.w = 100.f;
	m_worldTransform.h = 100.f;

	m_imagePath = "None";
	m_sprIndex = 0;
	m_sprSize = 0;
}

NKImage::~NKImage()
{
}

void NKImage::Layout(nk_context* ctx)
{
	if (m_imagePath != "None") {
		if (m_sprSize > 0) {
			struct nk_image img;
			m_manager->GetSprite(m_imagePath.c_str(), m_sprIndex, img);
			nk_image(ctx, img);
		}
		else {
			struct nk_image img;
			m_manager->GetImage(m_imagePath.c_str(), img);
			nk_image(ctx, img);
		}
	}
}

void NKImage::EditInfo()
{
	auto mapSpr = m_manager->GetSprMap();
	int size = mapSpr->size();

	if (m_sprSize > 0)
	{
		nk_layout_row_dynamic(m_ctx, 22, 1);
		nk_property_int(m_ctx, "#Index:", 0, &m_sprIndex, m_sprSize - 1, 1, 1);
	}

	nk_layout_row_dynamic(m_ctx, 22, 2);
	nk_label(m_ctx, "Selected:", NK_TEXT_LEFT);
	std::filesystem::path filePath(m_imagePath.c_str());
	nk_label(m_ctx, filePath.filename().string().c_str(), NK_TEXT_RIGHT);
	if (nk_button_label(m_ctx, "apply"))
	{
		struct nk_image img;
		m_manager->GetSprite(m_imagePath.c_str(), m_sprIndex, img, true);
	}
	if (nk_button_label(m_ctx, "clear"))
	{
		m_imagePath = "None";
		m_sprIndex = 0;
		m_sprSize = 0;
	}

	if (size > 0)
	{
		nk_layout_row_dynamic(m_ctx, 300, 1);
		if (nk_group_begin(m_ctx, "SPR List", NK_WINDOW_TITLE)) {

			float ratio[2] = { 0.8f, 0.2f };
			nk_layout_row(m_ctx, NK_DYNAMIC, 22, 2, ratio);
			int selected = 0;

			for (auto it = mapSpr->begin(); it != mapSpr->end(); ++it) {
				std::filesystem::path filePath((*it).first.c_str());
				nk_label(m_ctx, filePath.filename().string().c_str(), NK_TEXT_LEFT);

				if (nk_button_label(m_ctx, "Load")) {
					m_imagePath = (*it).first;
					m_sprSize = ((*it).second)->GetSpr()->GetXCount() * ((*it).second)->GetSpr()->GetYCount();
					if (m_sprSize <= m_sprIndex) {
						m_sprIndex = 0;
					}
				}
			}
			nk_group_end(m_ctx);
		}
	}
}