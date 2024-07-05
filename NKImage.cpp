#include "pch.h"
#include "NKImage.h"

NKImage::NKImage() : NKBase()
{
	m_type = eIMAGE;
	m_imagePath = "None";
	m_sprIndex = 0;
	m_sprSize = 0;
}

NKImage::NKImage(nk_context* ctx, NuklearUI* pManager) : NKBase(ctx, pManager)
{
	m_type = eIMAGE;

	m_cTransform.x = 50.f;
	m_cTransform.y = 50.f;
	m_cTransform.w = 100.f;
	m_cTransform.h = 100.f;

	m_imagePath = "None";
	m_sprIndex = 0;
	m_sprSize = 0;
}

NKImage::NKImage(const NKImage& other) : NKBase(other)
{
	m_type = other.m_type;

	m_imagePath = other.m_imagePath;
	m_sprIndex = other.m_sprIndex;
	m_sprSize = other.m_sprSize;
}

NKImage::~NKImage()
{
}

void NKImage::Layout(nk_context* ctx)
{
	if (m_imagePath != "None") {
		if (m_sprSize > 0) {
			struct nk_image img;
			bool bResult = m_pManager->GetSprite(m_imagePath.c_str(), m_sprIndex, img);
			if (!bResult) {
				m_pManager->ErrorPopup("Image URL not linked to the editor.");
				m_imagePath = "None";
				return;
			}
			else {
				nk_image(ctx, img);
			}
		}
		else {
			struct nk_image img;
			bool bResult = m_pManager->GetImage(m_imagePath.c_str(), img);
			if (!bResult) {
				m_pManager->ErrorPopup("Image URL not linked to the editor.");
				m_imagePath = "None";
				return;
			}
			else {
				nk_image(ctx, img);
			}
		}
	}
}

void NKImage::SafeRenderStart(nk_context* ctx)
{
	//UpdateComponent(ctx, m_pManager);
}

void NKImage::SafeRenderEnd(nk_context* ctx)
{
}

void NKImage::EditInfo(nk_context* ctx)
{
	auto mapSpr = m_pManager->GetSprMap();
	int size = mapSpr->size();

	if (m_sprSize > 0)
	{
		nk_layout_row_dynamic(ctx, 22, 1);
		nk_property_int(ctx, "#Index:", 0, &m_sprIndex, m_sprSize - 1, 1, 1);
	}

	nk_layout_row_dynamic(ctx, 22, 2);
	nk_label(ctx, "Selected:", NK_TEXT_LEFT);
	std::filesystem::path filePath(m_imagePath.c_str());
	nk_label(ctx, filePath.filename().string().c_str(), NK_TEXT_RIGHT);
	if (nk_button_label(ctx, "apply"))
	{
		struct nk_image img;
		m_pManager->GetSprite(m_imagePath.c_str(), m_sprIndex, img, true);
	}
	if (nk_button_label(ctx, "clear"))
	{
		m_imagePath = "None";
		m_sprIndex = 0;
		m_sprSize = 0;
	}

	if (size > 0)
	{
		nk_layout_row_dynamic(ctx, 300, 1);
		if (nk_group_begin(ctx, "SPR List", NK_WINDOW_TITLE)) {

			float ratio[2] = { 0.8f, 0.2f };
			nk_layout_row(ctx, NK_DYNAMIC, 22, 2, ratio);
			int selected = 0;

			for (auto it = mapSpr->begin(); it != mapSpr->end(); ++it) {
				std::filesystem::path filePath((*it).first.c_str());
				nk_label(ctx, filePath.filename().string().c_str(), NK_TEXT_LEFT);

				if (nk_button_label(ctx, "Load")) {
					m_imagePath = (*it).first;
					m_sprSize = ((*it).second)->GetSpr()->GetXCount() * ((*it).second)->GetSpr()->GetYCount();
					if (m_sprSize <= m_sprIndex) {
						m_sprIndex = 0;
					}
				}
			}
			nk_group_end(ctx);
		}
	}
}

void NKImage::SetImagePath(const char* imgPath)
{
	m_imagePath = imgPath;
}

void NKImage::LSetImagePath(luabridge::LuaRef ref)
{
	CHECK_LUA_REF(ref);
	std::string imgPath = ref.cast<std::string>();
	SetImagePath(imgPath.c_str());
}

bool NKImage::CSetImagePath(void* param)
{
	const char** imgPath = static_cast<const char**>(param);

	if (imgPath) {
		SetImagePath(*imgPath);
		return true;
	}
	return false;
}

void NKImage::SetSpritePath(const char* imgPath)
{
	auto mapSpr = m_pManager->GetSprMap();
	auto found = mapSpr->find(imgPath);
	if (found != mapSpr->end()) {
		m_imagePath = found->first;
		m_sprSize = found->second->GetSpr()->GetXCount() * found->second->GetSpr()->GetYCount();
		if (m_sprSize <= m_sprIndex) {
			m_sprIndex = 0;
		}
	}
}

void NKImage::LSetSpritePath(luabridge::LuaRef ref)
{
	CHECK_LUA_REF(ref);
	std::string imgPath = ref.cast<std::string>();
	SetSpritePath(imgPath.c_str());
}

void NKImage::SetIndex(int index)
{
	m_sprIndex = index;
}

void NKImage::LSetIndex(luabridge::LuaRef ref)
{
	CHECK_LUA_REF(ref);
	int index = ref.cast<int>();
	SetIndex(index);
}

bool NKImage::CSetIndex(void* param)
{
	int* index = static_cast<int*>(param);

	if (index) {
		SetIndex(*index);
		return true;
	}
	return false;
}

void NKImage::RegistCommand()
{
	NKBase::RegistCommand();
	MAKE_INTERFACE(m_mapFunc, this, NKImage::CSetImagePath, "NKImage");
	MAKE_INTERFACE(m_mapFunc, this, NKImage::CSetIndex, "NKImage");
}
