#pragma once
#ifndef ComponentTab_h__
#define ComponentTab_h__
#include "NKStyleItem.h"
#include "ComponentButton.h"
class ComponentTab : public NKComponent
{
public:
	ComponentTab();
	ComponentTab(struct nk_style_tab* pTarget, struct nk_style_tab* pRestore);
	ComponentTab& operator=(const ComponentTab& other);
	virtual ~ComponentTab();

	virtual void UpdateComponent(nk_context* ctx, NuklearUI* pManager) override;
	virtual void CustomComponentsEditor(nk_context* ctx, NuklearUI* pManager) override;

protected:
	struct nk_style_tab* m_pTarget;
	struct nk_style_tab* m_pRestore;

	NKStyleItem* m_pBackground;

	ComponentButton* m_pTabMaximizeButton;
	ComponentButton* m_pTabMinimizeButton;
	ComponentButton* m_pNodeMaximizeButton;
	ComponentButton* m_pNodeMinimizeButton;

public:
	template <class Archive>
	void serialize(Archive& ar, const unsigned int version) {
		ar(cereal::base_class<NKComponent>(this)
			, CEREAL_NVP(*m_pTarget)
			, CEREAL_NVP(*m_pRestore)
			, CEREAL_NVP(*m_pBackground)
			, CEREAL_NVP(*m_pTabMaximizeButton)
			, CEREAL_NVP(*m_pTabMinimizeButton)
			, CEREAL_NVP(*m_pNodeMaximizeButton)
			, CEREAL_NVP(*m_pNodeMinimizeButton)
		);
	}
};
#endif //ComponentTab_h__