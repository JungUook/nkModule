#pragma once
#ifndef NKStyleItem_h__
#define NKStyleItem_h__
#include "Constants.h"

class NuklearUI;

class NKStyleItem
{
public:
	NKStyleItem(struct nk_style_item* pTarget, struct nk_style_item* pRestore);
	NKStyleItem(const NKStyleItem& other);
	~NKStyleItem();

	void ItemEditor(nk_context* ctx, NuklearUI* pManager);
protected:
	std::string m_sImagePath;
	int m_iOption;
	int m_iSprIndex;
	int m_iSprSize;
	int m_iNineslice[4];
	struct nk_style_item* m_pTarget;
	struct nk_style_item* m_pRestore;
};

class NKComponent 
{
public:
	NKComponent() {};
	~NKComponent() {};
	virtual void CustomComponentsEditor(nk_context* ctx, NuklearUI* pManager) = 0;
};
#endif //NKStyleItem_h__