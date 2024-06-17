#pragma once
#ifndef ComponentButton_h__
#define ComponentButton_h__
#include "NKStyleItem.h"
class ComponentButton : public NKComponent
{
public:
	ComponentButton(struct nk_style_button* pTarget, struct nk_style_button* pRestore);
	ComponentButton(const ComponentButton& other);
	~ComponentButton();

	virtual void CustomComponentsEditor(nk_context* ctx, NuklearUI* pManager) override;

protected:
	struct nk_style_button* m_pTarget;
	struct nk_style_button* m_pRestore;

	NKStyleItem* m_pNormal;
	NKStyleItem* m_pHover;
	NKStyleItem* m_pActive;
};
#endif //ComponentButton_h__