#pragma once
#ifndef ComponentScrollbar_h__
#define ComponentScrollbar_h__
#include "NKStyleItem.h"
#include "ComponentButton.h"
class ComponentScrollbar : public NKComponent
{
public:
	ComponentScrollbar(struct nk_style_scrollbar* pTarget, struct nk_style_scrollbar* pRestore);
	ComponentScrollbar(const ComponentScrollbar& other);
	~ComponentScrollbar();

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
};
#endif //ComponentScrollbar_h__