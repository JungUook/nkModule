#pragma once
#ifndef NKStyleItem_h__
#define NKStyleItem_h__
#include "Constants.h"

class NuklearUI;

class NKStyleItem
{
public:
	NKStyleItem();
	NKStyleItem(struct nk_style_item* pTarget, struct nk_style_item* pRestore);
	NKStyleItem(const NKStyleItem& other);
	virtual ~NKStyleItem();

	void ItemEditor(nk_context* ctx, NuklearUI* pManager);
protected:
	std::string m_sImagePath;
	int m_iOption;
	int m_iSprIndex;
	int m_iSprSize;
	int m_iNineslice[4];
	struct nk_style_item* m_pTarget;
	struct nk_style_item* m_pRestore;

public:
	template <class Archive>
	void serialize(Archive& ar) {
		ar(m_sImagePath
			, m_iOption
			, m_iSprIndex
			, m_iSprSize
			, m_iNineslice
			, *m_pTarget
		);
	}
};

class NKComponent 
{
public:
	NKComponent() {};
	virtual ~NKComponent() {};
	virtual void CustomComponentsEditor(nk_context* ctx, NuklearUI* pManager) = 0;
public:
	template <class Archive>
	void serialize(Archive& ar) {
	}
};
#endif //NKStyleItem_h__