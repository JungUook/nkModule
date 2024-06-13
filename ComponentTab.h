#pragma once
#ifndef ComponentTab_h__
#define ComponentTab_h__
#include "NKStyleItem.h"
#include "ComponentButton.h"
class ComponentTab : public NKComponent
{
public:
	ComponentTab(struct nk_style_tab* pTarget, struct nk_style_tab* pRestore);
	~ComponentTab();

	virtual void CustomComponentsEditor(nk_context* ctx, NuklearUI* pManager) override;

protected:
	struct nk_style_tab* m_pTarget;
	struct nk_style_tab* m_pRestore;

	NKStyleItem* m_pBackground;

	ComponentButton* m_pTabMaximizeButton;
	ComponentButton* m_pTabMinimizeButton;
	ComponentButton* m_pNodeMaximizeButton;
	ComponentButton* m_pNodeMinimizeButton;
};
#endif //ComponentTab_h__