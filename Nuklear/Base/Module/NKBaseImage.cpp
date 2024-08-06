#include "pch.h"
#include "NKBaseImage.h"
#include "NuklearUI.h"

NKBaseImage::NKBaseImage()
{
	m_imagePath = "None";
	m_sprIndex = 0;
	m_sprSize = 0;
	m_mapSpr = nullptr;
}

NKBaseImage::NKBaseImage(NuklearUI* pManager)
{
	m_imagePath = "None";
	m_sprIndex = 0;
	m_sprSize = 0;
	m_mapSpr = pManager->GetSprMap();
}

NKBaseImage::NKBaseImage(const NKBaseImage& other)
{
	m_imagePath = other.m_imagePath;
	m_sprIndex = other.m_sprIndex;
	m_sprSize = other.m_sprSize;
	m_mapSpr = other.m_mapSpr;
}

NKBaseImage::~NKBaseImage()
{
}

void NKBaseImage::SetImagePath(const char* imgPath)
{
	m_imagePath = imgPath;
}

void NKBaseImage::LSetImagePath(luabridge::LuaRef ref)
{
	CHECK_LUA_REF(ref);
	std::string imgPath = ref.cast<std::string>();
	SetImagePath(imgPath.c_str());
}

bool NKBaseImage::CSetImagePath(void* param)
{
	const char** imgPath = static_cast<const char**>(param);

	if (imgPath) {
		SetImagePath(*imgPath);
		return true;
	}
	return false;
}

void NKBaseImage::SetSpritePath(const char* imgPath)
{
	auto found = m_mapSpr->find(imgPath);
	if (found != m_mapSpr->end()) {
		m_imagePath = found->first;
		m_sprSize = found->second->GetSpr()->GetXCount() * found->second->GetSpr()->GetYCount();
		if (m_sprSize <= m_sprIndex) {
			m_sprIndex = 0;
		}
	}
}

void NKBaseImage::LSetSpritePath(luabridge::LuaRef ref)
{
	CHECK_LUA_REF(ref);
	std::string imgPath = ref.cast<std::string>();
	SetSpritePath(imgPath.c_str());
}

void NKBaseImage::SetSpriteIndex(int index)
{
	m_sprIndex = index;
}

void NKBaseImage::LSetSpriteIndex(luabridge::LuaRef ref)
{
	CHECK_LUA_REF(ref);
	int index = ref.cast<int>();
	SetSpriteIndex(index);
}

bool NKBaseImage::CSetSpriteIndex(void* param)
{
	int* index = static_cast<int*>(param);

	if (index) {
		SetSpriteIndex(*index);
		return true;
	}
	return false;
}
