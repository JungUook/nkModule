#pragma once
#ifndef NKImage_h__
#define NKImage_h__
#include "NKBase.h"
class NKImage : public NKBase
{
public:
	NKImage();
	NKImage(nk_context* ctx, NuklearUI* pManager);
	NKImage(const NKImage& other);
	virtual ~NKImage();
	

public:
	virtual void Layout(nk_context* ctx) override;
	virtual void SafeRenderStart(nk_context* ctx) override;
	virtual void SafeRenderEnd(nk_context* ctx) override;
	virtual void EditInfo(nk_context* ctx) override;

	void SetImagePath(const char* imgPath);
	void LSetImagePath(luabridge::LuaRef ref);
	bool CSetImagePath(void* param);
	void SetSpritePath(const char* imgPath);
	void LSetSpritePath(luabridge::LuaRef ref);
	void SetIndex(int index);
	void LSetIndex(luabridge::LuaRef ref);
	bool CSetIndex(void* param);

	virtual void RegistCommand() override;
public:
	std::string m_imagePath;
	int m_sprIndex;
	int m_sprSize;

public:
	template <class Archive>
	void serialize(Archive& ar) {
		ar(cereal::base_class<NKBase>(this)
			, m_imagePath
			, m_sprIndex
			, m_sprSize
		);
	}
};


#endif //NKImage_h__