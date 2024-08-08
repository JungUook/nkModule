#pragma once
#ifndef NKTransform_h__
#define NKTransform_h__
#include "Constants.h"

class NuklearUI;

enum POSTYPE {
	eSTATIC = 0,
	eDYNAMIC = 1
};

class NKTransform
{
public:
	NKTransform();
	NKTransform(const NKTransform& other);
	virtual ~NKTransform();

	//속성관련 함수
public:
	virtual void SetPivot(NKTransform* parent, NuklearUI* pManager, float x, float y);
	virtual void SetPosition(NKTransform* parent, NuklearUI* pManager, float x, float y);
	virtual void SetSize(float width, float heigth);
	virtual struct nk_vec2 GetPivot();
	virtual struct nk_vec2 GetPosition(NKTransform* parent, NuklearUI* pManager);
	virtual struct nk_rect GetTransform();
	virtual float GetWidth();
	virtual float GetHeight();

	virtual struct nk_vec2* RefPivot();
	virtual struct nk_rect* RefTransform();

	//ui 편집용 함수
protected:
	virtual void PropertyTransform(nk_context* ctx, NKTransform* parent, NuklearUI* pManager);

protected:
	struct nk_vec2 m_sPivot;
	struct nk_vec2 m_sPosition;
	struct nk_rect m_sTransform;
	struct nk_rect* m_sSyncTransform;
	nk_bool m_bMouseHover;

public:
	template <class Archive>
	void serialize(Archive& ar, const unsigned int version) {
		ar(CEREAL_NVP(m_sPivot)
			, CEREAL_NVP(m_sPosition)
			, CEREAL_NVP(m_sTransform)
			, CEREAL_NVP(m_bMouseHover)
		);
	}
};
#endif //NKTransform_h__
