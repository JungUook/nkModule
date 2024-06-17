#include "pch.h"
#include "NKStyleItem.h"
#include "NuklearUI.h"

NKStyleItem::NKStyleItem(nk_style_item* pTarget, nk_style_item* pRestore) : m_sImagePath("None"), m_iOption(0), m_iSprIndex(0), m_iSprSize(0), m_pTarget(nullptr), m_pRestore(nullptr) {
	for (int i = 0; i < 4; ++i) {
		m_iNineslice[i] = 0;
	}
	m_pTarget = pTarget;
	m_pRestore = pRestore;
}

NKStyleItem::NKStyleItem(const NKStyleItem& other)
{
	for (int i = 0; i < 4; ++i) {
		m_iNineslice[i] = other.m_iNineslice[i];
	}
	m_sImagePath = other.m_sImagePath;
	m_iOption = other.m_iOption;
	m_iSprIndex = other.m_iSprIndex;
	m_iSprSize = other.m_iSprSize;
	m_pTarget = other.m_pTarget;
	m_pRestore = other.m_pRestore;

}

NKStyleItem::~NKStyleItem()
{
}

void NKStyleItem::ItemEditor(nk_context* ctx, NuklearUI* pManager)
{
	nk_layout_row_dynamic(ctx, 30, 1);
	if (nk_option_label(ctx, "color", m_iOption == 0)) m_iOption = 0;
	if (nk_option_label(ctx, "image", m_iOption == 1)) m_iOption = 1;
	if (nk_option_label(ctx, "nine_slice", m_iOption == 2)) m_iOption = 2;

	if (m_iOption == 0) {
		m_pTarget->type = NK_STYLE_ITEM_COLOR;
		ColorPicker(ctx, m_pTarget->data.color);
	}
	else if (m_iOption == 1 || m_iOption == 2) {
		auto mapSpr = pManager->GetSprMap();
		int size = mapSpr->size();

		nk_layout_row_dynamic(ctx, 22, 1);
		nk_property_int(ctx, "#Index:", 0, &m_iSprIndex, m_iSprSize - 1, 1, 1);

		nk_layout_row_dynamic(ctx, 22, 2);
		nk_label(ctx, "Selected:", NK_TEXT_LEFT);
		std::filesystem::path filePath(m_sImagePath.c_str());
		nk_label(ctx, filePath.filename().string().c_str(), NK_TEXT_RIGHT);
		if (nk_button_label(ctx, "apply"))
		{
			if (m_iOption == 1) {
				struct nk_image img;
				pManager->GetSprite(m_sImagePath.c_str(), m_iSprIndex, img, true);
				(*m_pTarget) = nk_style_item_image(img);
			}
			else if (m_iOption == 2) {
				struct nk_image img;
				pManager->GetSprite(m_sImagePath.c_str(), m_iSprIndex, img, true);
				struct nk_nine_slice nineslice {};
				nineslice.img = img;
				nineslice.l = (nk_ushort)m_iNineslice[0];
				nineslice.t = (nk_ushort)m_iNineslice[1];
				nineslice.r = (nk_ushort)m_iNineslice[2];
				nineslice.b = (nk_ushort)m_iNineslice[3];
				(*m_pTarget) = nk_style_item_nine_slice(nineslice);
			}
		}
		if (nk_button_label(ctx, "clear"))
		{
			(*m_pTarget) = (*m_pRestore);
		}
		if (m_iOption == 2) {
			nk_label(ctx, "nine_slice", NK_TEXT_LEFT);
			nk_layout_row_dynamic(ctx, 22, 1);
			nk_property_int(ctx, "#Left:", 0, &m_iNineslice[0], 255, 1, 1);
			nk_property_int(ctx, "#Top:", 0, &m_iNineslice[1], 255, 1, 1);
			nk_property_int(ctx, "#Right:", 0, &m_iNineslice[2], 255, 1, 1);
			nk_property_int(ctx, "#Bottom:", 0, &m_iNineslice[3], 255, 1, 1);
		}

		if (size > 0)
		{

			static char selectedFilename[260] = { 0, };
			float ratio[2] = { 0.7f, 0.3f };
			static char SearchFunction[256] = { 0, };
			static int SearchFunction_Len = 0;
			nk_layout_row(ctx, NK_DYNAMIC, 40, 2, ratio);
			nk_flags searchResult = pManager->IMEInputSystem(SearchFunction, sizeof(SearchFunction), &SearchFunction_Len);

			if (nk_button_label(ctx, "Search") | searchResult & NK_EDIT_COMMITED) {

			}

			nk_layout_row_dynamic(ctx, 300, 1);
			if (nk_group_begin(ctx, "SPR List", NK_WINDOW_TITLE)) {

				float ratio[2] = { 0.8f, 0.2f };
				nk_layout_row(ctx, NK_DYNAMIC, 22, 2, ratio);

				for (auto it = mapSpr->begin(); it != mapSpr->end(); ++it) {
					std::filesystem::path filePath((*it).first.c_str());

					bool bSearch = false;
					bool bSearchResult = true;
					if (strlen(SearchFunction) > 0) {
						bSearch = true;
					}

					if (bSearch) {
						std::wstring word = NuklearUI::utf8ToWstring(filePath.filename().string().c_str());
						std::wstring filter = NuklearUI::utf8ToWstring(SearchFunction);

						// word를 소문자로 변환
						std::transform(word.begin(), word.end(), word.begin(), towlower);
						// filter를 소문자로 변환
						std::transform(filter.begin(), filter.end(), filter.begin(), towlower);

						bSearchResult = word.find(filter) != std::wstring::npos;
					}

					if (!bSearchResult) {
						continue;
					}


					nk_label(ctx, filePath.filename().string().c_str(), NK_TEXT_LEFT);

					if (nk_button_label(ctx, "Load")) {
						m_sImagePath = (*it).first;
						m_iSprSize = ((*it).second)->GetSpr()->GetXCount() * ((*it).second)->GetSpr()->GetYCount();
						if (m_iSprSize <= m_iSprIndex) {
							m_iSprIndex = 0;
						}
					}
				}
				nk_group_end(ctx);
			}
		}
	}
}