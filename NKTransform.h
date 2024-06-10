#pragma once
#ifndef NKTransform_h__
#define NKTransform_h__
#include "Constants.h"

class NuklearUI;

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

	virtual struct nk_vec2* EditPivot();
	virtual struct nk_rect* EditTransform();

	//ui 편집용 함수
protected:
	virtual void PropertyTransform(nk_context* ctx, NKTransform* parent, NuklearUI* pManager);

protected:
	struct nk_vec2 m_pivot;
	struct nk_vec2 m_position;
	struct nk_rect m_worldTransform;
};
#endif //NKTransform_h__
