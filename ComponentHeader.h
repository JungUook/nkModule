#pragma once
#ifndef ComponentHeader_h__
#define ComponentHeader_h__
#include "NKStyleItem.h"
#include "ComponentButton.h"
class ComponentHeader : public NKComponent
{
public:
	ComponentHeader();
	ComponentHeader(struct nk_style_window_header* pTarget, struct nk_style_window_header* pRestore);
	ComponentHeader& operator=(const ComponentHeader& other);
	virtual ~ComponentHeader();

	virtual void UpdateComponent(nk_context* ctx, NuklearUI* pManager) override;
	virtual void CustomComponentsEditor(nk_context* ctx, NuklearUI* pManager) override;

protected:
	struct nk_style_window_header* m_pTarget;
	struct nk_style_window_header* m_pRestore;

	NKStyleItem* m_pNormal;
	NKStyleItem* m_pHover;
	NKStyleItem* m_pActive;

	ComponentButton* m_pCloseButton;
	ComponentButton* m_pMinimizeButton;

public:
	template <class Archive>
	void serialize(Archive& ar, const unsigned int version) {
		ar(cereal::base_class<NKComponent>(this)
			, CEREAL_NVP(*m_pTarget)
			, CEREAL_NVP(*m_pRestore)
			, CEREAL_NVP(*m_pNormal)
			, CEREAL_NVP(*m_pHover)
			, CEREAL_NVP(*m_pActive)
			, CEREAL_NVP(*m_pCloseButton)
			, CEREAL_NVP(*m_pMinimizeButton)
		);
	}
};
#endif //ComponentHeader_h__