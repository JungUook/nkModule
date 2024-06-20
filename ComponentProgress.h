#pragma once
#ifndef ComponentProgress_h__
#define ComponentProgress_h__
#include "NKStyleItem.h"
class ComponentProgress : public NKComponent
{
public:
	ComponentProgress();
	ComponentProgress(struct nk_style_progress* pTarget, struct nk_style_progress* pRestore);
	ComponentProgress(const ComponentProgress& other);
	virtual ~ComponentProgress();

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
		);
	}
};
#endif //ComponentProgress_h__