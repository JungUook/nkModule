#pragma once
#ifndef NKBaseStyle_h__
#define NKBaseStyle_h__
#include "Constants.h"

class NuklearUI;

class NKBaseStyle
{
public:
	NKBaseStyle();
	NKBaseStyle(nk_context* ctx, NuklearUI* pManager);
	NKBaseStyle(const NKBaseStyle& other);
	virtual ~NKBaseStyle();
	virtual void Init(nk_context* ctx, nk_style* style);

protected:
	virtual void InitializeStyle(nk_context* ctx, NuklearUI* pManager);
	virtual void InitializeStyle(nk_font* font, nk_style& parentStyle, nk_style* parent_of_parentStyle);

	virtual void StyleUpdateStart(nk_context* ctx, nk_style& original, NKBaseStyle* pParent);
	virtual void StyleUpdateEnd(nk_context* ctx, nk_style& original);

	virtual void SetStyle(nk_style* style);
	virtual void Setfont(nk_font* font);
	virtual void SetBackground(NuklearUI* pManager, int SID);

	//제어 함수
public:
	//virtual void Load(nk_context* ctx, NuklearUI* pManager);

	//ui 편집용 함수
protected:
	void FollowParentStyle(nk_context* ctx, NKBaseStyle* pParent);

protected:
	nk_style m_style;
	nk_font* m_font;

	nk_style* m_pParentStyle;
	nk_bool m_followParentStyle;

public:
	template <class Archive>
	void serialize(Archive& ar) {
		ar(m_followParentStyle);
	}
};
#endif //NKBaseStyle_h__