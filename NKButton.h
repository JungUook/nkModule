#pragma once
#ifndef NKButton_h__
#define NKButton_h__
#include "NKBase.h"
#include "NKHandler.h"
#include "NKBaseLabel.h"
#include "NKStyleButton.h"
class NKButton : public NKBase, public NKHandler, public NKBaseLabel, public NKStyleButton
{
public:
	NKButton();
	NKButton(nk_context* ctx, NuklearUI* pManager);
	NKButton(const NKButton& other);
	virtual ~NKButton();
	

public:
	virtual void LayoutBegin(nk_context* ctx) override;
	virtual void Layout(nk_context* ctx) override;
	virtual void LayoutEnd(nk_context* ctx) override;
	virtual void SafeRenderStart(nk_context* ctx) override;
	virtual void SafeRenderEnd(nk_context* ctx) override;
	virtual void EditInfo(nk_context* ctx) override;
	virtual void EditStyle(nk_context* ctx) override;

	virtual void RegistCommand(const char* classname) override;

	void DisableButton(bool bDisabled);
	void LDisableButton(luabridge::LuaRef ref);
	bool CDisableButton(void* param);
public:
	nk_bool m_bDisabled;
public:
	template <class Archive>
	void serialize(Archive& ar) {
		ar(cereal::base_class<NKBase>(this)
			, cereal::base_class<NKHandler>(this)
			, cereal::base_class<NKBaseLabel>(this)
			, cereal::base_class<NKStyleButton>(this)
			, m_bDisabled
		);
	}
};
#endif //NKButton_h__