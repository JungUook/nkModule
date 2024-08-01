#pragma once
#ifndef NKBaseImage_h__
#define NKBaseImage_h__
#include "NKLuaInterface.h"

class NuklearUI;
class sprData;

class NKBaseImage
{
public:
	NKBaseImage();
	NKBaseImage(NuklearUI* pManager);
	NKBaseImage(const NKBaseImage& other);
	virtual ~NKBaseImage();

	virtual void SetImagePath(const char* imgPath);
	virtual void LSetImagePath(luabridge::LuaRef ref);
	virtual bool CSetImagePath(void* param);
	virtual void SetSpritePath(const char* imgPath);
	virtual void LSetSpritePath(luabridge::LuaRef ref);
	virtual void SetSpriteIndex(int index);
	virtual void LSetSpriteIndex(luabridge::LuaRef ref);
	virtual bool CSetSpriteIndex(void* param);

public:
	std::string m_imagePath;
	int m_sprIndex;
	int m_sprSize;

	std::map<std::string, sprData*>* m_mapSpr;
public:
	template <class Archive>
	void serialize(Archive& ar, const unsigned int version) {
		ar(
			CEREAL_NVP(m_imagePath)
			, CEREAL_NVP(m_sprIndex)
			, CEREAL_NVP(m_sprSize)
		);
	}
};
#endif //NKBaseImage_h__