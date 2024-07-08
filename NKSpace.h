#pragma once
#ifndef NKSpace_h__
#define NKSpace_h__
#include "NKBase.h"
class NKSpace : public NKBase
{
public:
	NKSpace();
	NKSpace(nk_context* ctx, NuklearUI* pManager);
	NKSpace(const NKSpace& other);
	virtual ~NKSpace();
	

public:
	virtual void Layout(nk_context* ctx) override;
	virtual void SafeRenderStart(nk_context* ctx) override;
	virtual void SafeRenderEnd(nk_context* ctx) override;
	virtual void EditInfo(nk_context* ctx) override;
	virtual void EditStyle(nk_context* ctx) override;
	
	void SetLayout(int type);
	void LSetLayout(luabridge::LuaRef ref);
	bool CSetLayout(void* param);
	void SetCols(int cols);
	void LSetCols(luabridge::LuaRef ref);
	bool CSetCols(void* param);

	virtual void RegistCommand(const char* classname) override;
public:
	nk_layout_format m_layoutFormat;
	int m_widgetCount;
	int m_dynamicCount;

public:
	template <class Archive>
	void serialize(Archive& ar) {
		ar(cereal::base_class<NKBase>(this)
			, m_layoutFormat
			, m_widgetCount
			, m_dynamicCount
		);
	}
};

#endif //NKSpace_h__