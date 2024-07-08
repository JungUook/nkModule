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
	NKStyleItem& operator=(const NKStyleItem& other);
	virtual ~NKStyleItem();

	virtual void UpdateComponent(nk_context* ctx, NuklearUI* pManager);
	virtual void ItemEditor(nk_context* ctx, NuklearUI* pManager);

	void DisableButton(bool bDisabled);
	void EditDisablePath(const char* disablePath);
protected:
	std::string m_sImagePath;
	int m_iOption;
	int m_iSprIndex;
	int m_iSprSize;
	int m_iNineslice[4];
	bool m_bApply;
	struct nk_style_item* m_pTarget;
	struct nk_style_item* m_pRestore;

	bool m_bDisabled;
	std::string m_sDisablePath;

public:
	template <class Archive>
	void serialize(Archive& ar) {
		ar(m_sImagePath
			, m_iOption
			, m_iSprIndex
			, m_iSprSize
			, m_iNineslice
			, m_bApply
			, *m_pTarget
			, m_bDisabled
			, m_sDisablePath
		);
	}
};

class NKComponent 
{
public:
	NKComponent() {};
	virtual ~NKComponent() {};
	virtual void UpdateComponent(nk_context* ctx, NuklearUI* pManager) = 0;
	virtual void CustomComponentsEditor(nk_context* ctx, NuklearUI* pManager) = 0;
public:
	template <class Archive>
	void serialize(Archive& ar) {
	}
};
#endif //NKStyleItem_h__