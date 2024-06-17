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
	~NKTransform();

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
	struct nk_vec2 m_cPivot;
	struct nk_vec2 m_cPosition;
	struct nk_rect m_cTransform;
	struct nk_rect* m_cSyncTransform;
	nk_bool m_bMouseHover;
};
#endif //NKTransform_h__
