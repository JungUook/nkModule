#pragma once
#ifndef ComponentButton_h__
#define ComponentButton_h__
#include "NKStyleItem.h"
class ComponentButton : public NKComponent
{
public:
	ComponentButton();
	ComponentButton(struct nk_style_button* pTarget, struct nk_style_button* pRestore);
	ComponentButton& operator=(const ComponentButton& other);
	virtual ~ComponentButton();

	virtual void UpdateComponent(nk_context* ctx, NuklearUI* pManager) override;
	virtual void CustomComponentsEditor(nk_context* ctx, NuklearUI* pManager) override;

	void DisableButton(bool bDisabled);
	void EditDisablePath(nk_context* ctx, NuklearUI* pManager);
protected:
	struct nk_style_button* m_pTarget;
	struct nk_style_button* m_pRestore;

	NKStyleItem* m_pNormal;
	NKStyleItem* m_pHover;
	NKStyleItem* m_pActive;

	bool m_bDisabled;
	std::string m_sDisablePath;

public:
	template <class Archive>
	void serialize(Archive& ar, const unsigned int version) {
		ar(cereal::base_class<NKComponent>(this)
			, CEREAL_NVP(*m_pTarget)
			, CEREAL_NVP(*m_pRestore)
			, CEREAL_NVP(*m_pNormal)
			, CEREAL_NVP(*m_pHover)
			, CEREAL_NVP(*m_pActive)
			, CEREAL_NVP(m_bDisabled)
		);
	}
};
#endif //ComponentButton_h__