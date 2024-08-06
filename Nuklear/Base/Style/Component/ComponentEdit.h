#pragma once
#ifndef ComponentEdit_h__
#define ComponentEdit_h__
#include "NKStyleItem.h"
#include "ComponentScrollbar.h"
class ComponentEdit : public NKComponent
{
public:
	ComponentEdit();
	ComponentEdit(struct nk_style_edit* pTarget, struct nk_style_edit* pRestore);
	ComponentEdit& operator=(const ComponentEdit& other);
	virtual ~ComponentEdit();

	virtual void UpdateComponent(nk_context* ctx, NuklearUI* pManager) override;
	virtual void CustomComponentsEditor(nk_context* ctx, NuklearUI* pManager) override;

protected:
	struct nk_style_edit* m_pTarget;
	struct nk_style_edit* m_pRestore;

	NKStyleItem* m_pNormal;
	NKStyleItem* m_pHover;
	NKStyleItem* m_pActive;

	ComponentScrollbar* m_pScrollbar;

public:
	template <class Archive>
	void serialize(Archive& ar, const unsigned int version) {
		ar(cereal::base_class<NKComponent>(this)
			, CEREAL_NVP(*m_pTarget)
			, CEREAL_NVP(*m_pRestore)
			, CEREAL_NVP(*m_pNormal)
			, CEREAL_NVP(*m_pHover)
			, CEREAL_NVP(*m_pActive)
			, CEREAL_NVP(*m_pScrollbar)
		);
	}
};
#endif //ComponentEdit_h__