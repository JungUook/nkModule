#pragma once
#ifndef ComponentHeader_h__
#define ComponentHeader_h__
#include "NKStyleItem.h"
#include "ComponentButton.h"
class ComponentHeader : public NKComponent
{
public:
	ComponentHeader();
	ComponentHeader(struct nk_style_window_header* pTarget, struct nk_style_window_header* pRestore);
	ComponentHeader& operator=(const ComponentHeader& other);
	virtual ~ComponentHeader();

	virtual void UpdateComponent(nk_context* ctx, NuklearUI* pManager) override;
	virtual void CustomComponentsEditor(nk_context* ctx, NuklearUI* pManager) override;

protected:
	struct nk_style_window_header* m_pTarget;
	struct nk_style_window_header* m_pRestore;

	NKStyleItem* m_pNormal;
	NKStyleItem* m_pHover;
	NKStyleItem* m_pActive;

	ComponentButton* m_pCloseButton;
	ComponentButton* m_pMinimizeButton;

public:
	template <class Archive>
	void serialize(Archive& ar) {
		ar(cereal::base_class<NKComponent>(this)
			, *m_pTarget
			, *m_pRestore
			, *m_pNormal
			, *m_pHover
			, *m_pActive
			, *m_pCloseButton
			, *m_pMinimizeButton
		);
	}
};
#endif //ComponentHeader_h__