#include "pch.h"
#include "NKObjectFinder.h"
#include "NuklearUI.h"

NKObjectFinder::NKObjectFinder()
{
	m_pResultObject = nullptr;

	memset(m_cSearchObject, 0, sizeof(m_cSearchObject));
	m_iSearchObjectLen = 0;
}

NKObjectFinder::NKObjectFinder(NuklearUI* pManager)
{
	m_pResultObject = nullptr;

	memset(m_cSearchObject, 0, sizeof(m_cSearchObject));
	m_iSearchObjectLen = 0;
}

NKObjectFinder::NKObjectFinder(const NKObjectFinder& other)
{
	m_pResultObject = other.m_pResultObject;
	m_iSearchObjectLen = other.m_iSearchObjectLen;
	strcpy_s(m_cSearchObject, other.m_cSearchObject);
}

NKObjectFinder::~NKObjectFinder()
{
}

void NKObjectFinder::FoundObject(nk_context* ctx, NuklearUI* pManager)
{
	float ratio[2] = { 0.7f, 0.3f };
	nk_layout_row(ctx, NK_DYNAMIC, 40, 2, ratio);
	nk_flags searchResult = pManager->IMEInputSystem(ctx, m_cSearchObject, sizeof(m_cSearchObject), &m_iSearchObjectLen);
	if (nk_button_label(ctx, "Search") | searchResult & NK_EDIT_COMMITED) {

	}
	ratio[0] = 0.3f;
	ratio[1] = 0.7f;
	nk_layout_row(ctx, NK_DYNAMIC, 44, 2, ratio);
	nk_label(ctx, "Found: ", NK_TEXT_LEFT);
	if (m_pResultObject) {
		nk_label(ctx, m_pResultObject->GetBaseName(), NK_TEXT_RIGHT);
	}
	else {
		nk_label(ctx, "None", NK_TEXT_RIGHT);
	}
}

NKBase* NKObjectFinder::SearchObject(nk_context* ctx, NuklearUI* pManager)
{
	nk_layout_row_dynamic(ctx, 500, 1);
	if (nk_group_begin(ctx, "Node List", NK_WINDOW_TITLE)) {
		float ratio[2] = { 0.8f, 0.2f };
		nk_layout_row(ctx, NK_DYNAMIC, 22, 2, ratio);

		auto vNodes = pManager->GetNodes();

		for (auto it = vNodes->begin(); it != vNodes->end(); ++it) {
			NKBase* pBase = *it;

			bool bSearch = false;
			bool bSearchResult = true;
			if (strlen(m_cSearchObject) > 0) {
				bSearch = true;
			}

			if (bSearch) {
				std::wstring word = NKLuaInterface::utf8ToWstring(pBase->GetBaseName());
				std::wstring filter = NKLuaInterface::utf8ToWstring(m_cSearchObject);

				// word를 소문자로 변환
				std::transform(word.begin(), word.end(), word.begin(), towlower);
				// filter를 소문자로 변환
				std::transform(filter.begin(), filter.end(), filter.begin(), towlower);

				bSearchResult = word.find(filter) != std::wstring::npos;
			}

			if (!bSearchResult) {
				continue;
			}

			nk_label(ctx, pBase->GetBaseName(), NK_TEXT_LEFT);

			if (nk_button_label(ctx, "Push")) {
				m_pResultObject = pBase;
			}
		}

		nk_group_end(ctx);
	}

	return m_pResultObject;
}

void NKObjectFinder::LostObjectEvent(unsigned int id)
{
	if (m_pResultObject == nullptr) {
		return;
	}
	if (m_pResultObject->GetPrimaryID() == id) {
		m_pResultObject = nullptr;
		memset(m_cSearchObject, 0, sizeof(m_cSearchObject));
		m_iSearchObjectLen = 0;
	}
}
