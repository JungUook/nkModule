#pragma once
#ifndef ComponentToggle_h__
#define ComponentToggle_h__
#include "NKStyleItem.h"
class ComponentToggle : public NKComponent
{
public:
	ComponentToggle();
	ComponentToggle(struct nk_style_toggle* pTarget, struct nk_style_toggle* pRestore);
	ComponentToggle& operator=(const ComponentToggle& other);
	virtual ~ComponentToggle();

	virtual void UpdateComponent(nk_context* ctx, NuklearUI* pManager) override;
	virtual void CustomComponentsEditor(nk_context* ctx, NuklearUI* pManager) override;

protected:
	struct nk_style_toggle* m_pTarget;
	struct nk_style_toggle* m_pRestore;

	NKStyleItem* m_pNormal;
	NKStyleItem* m_pHover;
	NKStyleItem* m_pActive;

	NKStyleItem* m_pCursorNormal;
	NKStyleItem* m_pCursorHover;

public:
	template <class Archive>
	void serialize(Archive& ar, const unsigned int version) {
		ar(cereal::base_class<NKComponent>(this)
			, CEREAL_NVP(*m_pTarget)
			, CEREAL_NVP(*m_pRestore)
			, CEREAL_NVP(*m_pNormal)
			, CEREAL_NVP(*m_pHover)
			, CEREAL_NVP(*m_pActive)
			, CEREAL_NVP(*m_pCursorNormal)
			, CEREAL_NVP(*m_pCursorHover)
		);
	}
};
#endif //ComponentToggle_h__