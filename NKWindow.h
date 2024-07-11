#pragma once
#ifndef NKWindow_h__
#define NKWindow_h__
#include "NKBase.h"
#include "NKBaseWindow.h"
#include "NKStyleHeader.h"
#include "NKStyleWindow.h"
class NKWindow : public NKBase, public NKBaseWindow, public NKStyleHeader, public NKStyleWindow
{
public:
	NKWindow();
	NKWindow(nk_context* ctx, NuklearUI* pManager);
	NKWindow(const NKWindow& other);
	virtual ~NKWindow() override;
	

public:
	virtual void LayoutBegin(nk_context* ctx) override;
	virtual void Layout(nk_context* ctx) override;
	virtual void LayoutEnd(nk_context* ctx) override;
	virtual void SafeRenderStart(nk_context* ctx) override;
	virtual void SafeRenderEnd(nk_context* ctx) override;
	virtual void EditInfo(nk_context* ctx) override;
	virtual void EditStyle(nk_context* ctx) override;
	virtual nk_bool CheckMouseHover(nk_context* ctx) override;

	virtual void UpdateComponent(nk_context* ctx, NuklearUI* pManager) override;
	virtual void EditComponentStyle(nk_context* ctx, NuklearUI* pManager) override;

	virtual void RegistCommand(const char* classname) override;
public:
	template <class Archive>
	void serialize(Archive& ar, const unsigned int version) {
		ar(cereal::base_class<NKBase>(this)
			, cereal::base_class<NKBaseWindow>(this)
			, cereal::base_class<NKStyleHeader>(this)
			, cereal::base_class<NKStyleWindow>(this)
		);
	}
};

#endif //NKWindow_h__