#pragma once
#ifndef ComponentSelectable_h__
#define ComponentSelectable_h__
#include "NKStyleItem.h"
class ComponentSelectable : public NKComponent
{
public:
	ComponentSelectable();
	ComponentSelectable(struct nk_style_selectable* pTarget, struct nk_style_selectable* pRestore);
	ComponentSelectable& operator=(const ComponentSelectable& other);
	virtual ~ComponentSelectable();

	virtual void UpdateComponent(nk_context* ctx, NuklearUI* pManager) override;
	virtual void CustomComponentsEditor(nk_context* ctx, NuklearUI* pManager) override;

protected:
	struct nk_style_selectable* m_pTarget;
	struct nk_style_selectable* m_pRestore;

	NKStyleItem* m_pNormal;
	NKStyleItem* m_pHover;
	NKStyleItem* m_pPressed;

	NKStyleItem* m_pNormalActive;
	NKStyleItem* m_pHoverActive;
	NKStyleItem* m_pPressedActive;

public:
	template <class Archive>
	void serialize(Archive& ar, const unsigned int version) {
		ar(cereal::base_class<NKComponent>(this)
			, CEREAL_NVP(*m_pTarget)
			, CEREAL_NVP(*m_pRestore)
			, CEREAL_NVP(*m_pNormal)
			, CEREAL_NVP(*m_pHover)
			, CEREAL_NVP(*m_pPressed)
			, CEREAL_NVP(*m_pNormalActive)
			, CEREAL_NVP(*m_pHoverActive)
			, CEREAL_NVP(*m_pPressedActive)
		);
	}
};
#endif //ComponentSelectable_h__