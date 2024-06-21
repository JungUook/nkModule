#pragma once
#ifndef ComponentScrollbar_h__
#define ComponentScrollbar_h__
#include "NKStyleItem.h"
#include "ComponentButton.h"
class ComponentScrollbar : public NKComponent
{
public:
	ComponentScrollbar();
	ComponentScrollbar(struct nk_style_scrollbar* pTarget, struct nk_style_scrollbar* pRestore);
	ComponentScrollbar& operator=(const ComponentScrollbar& other);
	virtual ~ComponentScrollbar();

	virtual void UpdateComponent(nk_context* ctx, NuklearUI* pManager) override;
	virtual void CustomComponentsEditor(nk_context* ctx, NuklearUI* pManager) override;

protected:
	struct nk_style_scrollbar* m_pTarget;
	struct nk_style_scrollbar* m_pRestore;

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
#endif //ComponentScrollbar_h__