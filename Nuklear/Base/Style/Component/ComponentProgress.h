#pragma once
#ifndef ComponentProgress_h__
#define ComponentProgress_h__
#include "NKStyleItem.h"
class ComponentProgress : public NKComponent
{
public:
	ComponentProgress();
	ComponentProgress(struct nk_style_progress* pTarget, struct nk_style_progress* pRestore);
	ComponentProgress& operator=(const ComponentProgress& other);
	virtual ~ComponentProgress();

	virtual void UpdateComponent(nk_context* ctx, NuklearUI* pManager) override;
	virtual void CustomComponentsEditor(nk_context* ctx, NuklearUI* pManager) override;

protected:
	struct nk_style_progress* m_pTarget;
	struct nk_style_progress* m_pRestore;

	NKStyleItem* m_pNormal;
	NKStyleItem* m_pHover;
	NKStyleItem* m_pActive;

	NKStyleItem* m_CursorNormal;
	NKStyleItem* m_CursorHover;
	NKStyleItem* m_CursorActive;

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
		);
	}
};
#endif //ComponentProgress_h__