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

protected:
	struct nk_style_button* m_pTarget;
	struct nk_style_button* m_pRestore;

	NKStyleItem* m_pNormal;
	NKStyleItem* m_pHover;
	NKStyleItem* m_pActive;

public:
	template <class Archive>
	void serialize(Archive& ar) {
		ar(cereal::base_class<NKComponent>(this)
			, *m_pTarget
			, *m_pRestore
			, *m_pNormal
			, *m_pHover
			, *m_pActive
		);
	}
};
#endif //ComponentButton_h__