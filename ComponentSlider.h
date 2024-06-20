#pragma once
#ifndef ComponentSlider_h__
#define ComponentSlider_h__
#include "NKStyleItem.h"
#include "ComponentButton.h"

class ComponentSlider : public NKComponent
{
public:
	ComponentSlider();
	ComponentSlider(struct nk_style_slider* pTarget, struct nk_style_slider* pRestore);
	ComponentSlider(const ComponentSlider& other);
	virtual ~ComponentSlider();

	virtual void CustomComponentsEditor(nk_context* ctx, NuklearUI* pManager) override;

protected:
	struct nk_style_slider* m_pTarget;
	struct nk_style_slider* m_pRestore;

	NKStyleItem* m_pNormal;
	NKStyleItem* m_pHover;
	NKStyleItem* m_pActive;

	NKStyleItem* m_CursorNormal;
	NKStyleItem* m_CursorHover;
	NKStyleItem* m_CursorActive;

	ComponentButton* m_pIncButton;
	ComponentButton* m_pDecButton;

public:
	template <class Archive>
	void serialize(Archive& ar) {
		ar(cereal::base_class<NKComponent>(this)
			, *m_pTarget
			, *m_pRestore
			, *m_pNormal
			, *m_pHover
			, *m_pActive
			, *m_CursorNormal
			, *m_CursorHover
			, *m_CursorActive
			, *m_pIncButton
			, *m_pDecButton
		);
	}
};
#endif //ComponentSlider_h__