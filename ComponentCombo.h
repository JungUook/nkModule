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
	void serialize(Archive& ar) {
		ar(cereal::base_class<NKComponent>(this)
			, *m_pTarget
			, *m_pRestore
			, *m_pNormal
			, *m_pHover
			, *m_pActive
			, *m_pButton
		);
	}
};
#endif //ComponentCombo_h__