#pragma once
#ifndef ComponentCombo_h__
#define ComponentCombo_h__
#include "NKStyleItem.h"
#include "ComponentButton.h"

class ComponentCombo : public NKComponent
{
public:
	ComponentCombo();
	ComponentCombo(struct nk_style_combo* pTarget, struct nk_style_combo* pRestore);
	ComponentCombo& operator=(const ComponentCombo& other);
	virtual ~ComponentCombo();

	virtual void UpdateComponent(nk_context* ctx, NuklearUI* pManager) override;
	virtual void CustomComponentsEditor(nk_context* ctx, NuklearUI* pManager) override;

protected:
	struct nk_style_combo* m_pTarget;
	struct nk_style_combo* m_pRestore;

	NKStyleItem* m_pNormal;
	NKStyleItem* m_pHover;
	NKStyleItem* m_pActive;

	ComponentButton* m_pButton;

public:
	template <class Archive>
	void serialize(Archive& ar, const unsigned int version) {
		ar(cereal::base_class<NKComponent>(this)
			, CEREAL_NVP(*m_pTarget)
			, CEREAL_NVP(*m_pRestore)
			, CEREAL_NVP(*m_pNormal)
			, CEREAL_NVP(*m_pHover)
			, CEREAL_NVP(*m_pActive)
			, CEREAL_NVP(*m_pButton)
		);
	}
};
#endif //ComponentCombo_h__