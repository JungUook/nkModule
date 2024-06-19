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
	ComponentEdit(const ComponentEdit& other);
	virtual ~ComponentEdit();

	virtual void CustomComponentsEditor(nk_context* ctx, NuklearUI* pManager) override;

protected:
	struct nk_style_edit* m_pTarget;
	struct nk_style_edit* m_pRestore;

	NKStyleItem* m_pNormal;
	NKStyleItem* m_pHover;
	NKStyleItem* m_pActive;

	ComponentScrollbar* m_pScrollbar;
};
#endif //ComponentEdit_h__