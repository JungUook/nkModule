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
	void serialize(Archive& ar) {
		ar(cereal::base_class<NKComponent>(this)
			, *m_pTarget
			, *m_pRestore
			, *m_pNormal
			, *m_pHover
			, *m_pActive
			, *m_pCursorNormal
			, *m_pCursorHover
		);
	}
};
#endif //ComponentToggle_h__