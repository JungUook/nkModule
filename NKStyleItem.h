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
	void serialize(Archive& ar, const unsigned int version) {
		ar(CEREAL_NVP(m_sImagePath)
			, CEREAL_NVP(m_iOption)
			, CEREAL_NVP(m_iSprIndex)
			, CEREAL_NVP(m_iSprSize)
			, CEREAL_NVP(m_iNineslice)
			, CEREAL_NVP(m_bApply)
			, CEREAL_NVP(*m_pTarget)
			, CEREAL_NVP(m_bDisabled)
			, CEREAL_NVP(m_sDisablePath)
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
	void serialize(Archive& ar, const unsigned int version) {
	}
};
#endif //NKStyleItem_h__