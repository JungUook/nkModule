#pragma once
#ifndef ComponentProperty_h__
#define ComponentProperty_h__
#include "NKStyleItem.h"
#include "ComponentEdit.h"
#include "ComponentButton.h"
class ComponentProperty : public NKComponent
{
public:
	ComponentProperty();
	ComponentProperty(struct nk_style_property* pTarget, struct nk_style_property* pRestore);
	ComponentProperty& operator=(const ComponentProperty& other);
	virtual ~ComponentProperty();

	virtual void UpdateComponent(nk_context* ctx, NuklearUI* pManager) override;
	virtual void CustomComponentsEditor(nk_context* ctx, NuklearUI* pManager) override;

protected:
	struct nk_style_property* m_pTarget;
	struct nk_style_property* m_pRestore;

	NKStyleItem* m_pNormal;
	NKStyleItem* m_pHover;
	NKStyleItem* m_pActive;

	ComponentEdit* m_pEdit;
	ComponentButton* m_pIncButton;
	ComponentButton* m_pDecButton;

public:
	template <class Archive>
	void serialize(Archive& ar, const unsigned int version) {
		ar(cereal::base_class<NKComponent>(this)
			, CEREAL_NVP(*m_pTarget)
			, CEREAL_NVP(*m_pRestore)
			, CEREAL_NVP(*m_pNormal)
			, CEREAL_NVP(*m_pHover)
			, CEREAL_NVP(*m_pActive)
			, CEREAL_NVP(*m_pEdit)
			, CEREAL_NVP(*m_pIncButton)
			, CEREAL_NVP(*m_pDecButton)
		);
	}
};
#endif //ComponentProperty_h__