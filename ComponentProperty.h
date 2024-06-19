#pragma once
#ifndef ComponentProperty_h__
#define ComponentProperty_h__
#include "NKStyleItem.h"
#include "ComponentEdit.h"
#include "ComponentButton.h"
class ComponentProperty : public NKComponent
{
public:
	ComponentProperty();
	ComponentProperty(struct nk_style_property* pTarget, struct nk_style_property* pRestore);
	ComponentProperty(const ComponentProperty& other);
	virtual ~ComponentProperty();

	virtual void CustomComponentsEditor(nk_context* ctx, NuklearUI* pManager) override;

protected:
	struct nk_style_property* m_pTarget;
	struct nk_style_property* m_pRestore;

	NKStyleItem* m_pNormal;
	NKStyleItem* m_pHover;
	NKStyleItem* m_pActive;

	ComponentEdit* m_pEdit;
	ComponentButton* m_pIncButton;
	ComponentButton* m_pDecButton;
};
#endif //ComponentProperty_h__