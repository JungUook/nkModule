#pragma once
#ifndef ComponentProgress_h__
#define ComponentProgress_h__
#include "NKStyleItem.h"
class ComponentProgress : public NKComponent
{
public:
	ComponentProgress(struct nk_style_progress* pTarget, struct nk_style_progress* pRestore);
	~ComponentProgress();

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
};
#endif //ComponentProgress_h__