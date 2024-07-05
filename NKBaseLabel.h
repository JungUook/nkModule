#pragma once
#ifndef NKBaseLabel_h__
#define NKBaseLabel_h__
#include "NKLuaInterface.h"

class NuklearUI;

class NKBaseLabel
{
public:
	NKBaseLabel();
	NKBaseLabel(const NKBaseLabel& other);
	virtual ~NKBaseLabel();

	virtual nk_flags EditLabel(nk_context* ctx, NuklearUI* pManager);
	virtual void SetLabel(const char* text);
	void LSetLabel(luabridge::LuaRef ref);
	bool CSetLabel(void* param);

	virtual void CustomFontSizeBegin(nk_context* ctx, nk_font* font);
	virtual void CustomFontSizeEnd(nk_context* ctx, NuklearUI* pManager, nk_font* font);
protected:
	float m_fScale;
	char m_cEditLabel[256];
	int m_iEditLabelLen;
	char m_cContent[256];

public:
	template <class Archive>
	void serialize(Archive& ar) {
		ar(m_fScale
			, m_cEditLabel
			, m_iEditLabelLen
			, m_cContent
		);
	}
};

#endif //NKBaseLabel_h__