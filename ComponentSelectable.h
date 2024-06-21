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
	void serialize(Archive& ar) {
		ar(cereal::base_class<NKComponent>(this)
			, *m_pTarget
			, *m_pRestore
			, *m_pNormal
			, *m_pHover
			, *m_pPressed
			, *m_pNormalActive
			, *m_pHoverActive
			, *m_pPressedActive
		);
	}
};
#endif //ComponentSelectable_h__