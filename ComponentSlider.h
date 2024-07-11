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
	ComponentSlider& operator=(const ComponentSlider& other);
	virtual ~ComponentSlider();

	virtual void UpdateComponent(nk_context* ctx, NuklearUI* pManager) override;
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
	void serialize(Archive& ar, const unsigned int version) {
		ar(cereal::base_class<NKComponent>(this)
			, CEREAL_NVP(*m_pTarget)
			, CEREAL_NVP(*m_pRestore)
			, CEREAL_NVP(*m_pNormal)
			, CEREAL_NVP(*m_pHover)
			, CEREAL_NVP(*m_pActive)
			, CEREAL_NVP(*m_CursorNormal)
			, CEREAL_NVP(*m_CursorHover)
			, CEREAL_NVP(*m_CursorActive)
			, CEREAL_NVP(*m_pIncButton)
			, CEREAL_NVP(*m_pDecButton)
		);
	}
};
#endif //ComponentSlider_h__