#pragma once
#ifndef NKGroup_h__
#define NKGroup_h__
#include "NKBase.h"
#include "NKBaseWindow.h"
#include "NKStyleHeader.h"
#include "NKStyleWindow.h"
class NKGroup : public NKBase, public NKBaseWindow, public NKStyleHeader, public NKStyleWindow
{
public:
	NKGroup();
	NKGroup(nk_context* ctx, NuklearUI* pManager);
	NKGroup(const NKGroup& other);
	virtual ~NKGroup();

public:
	virtual void Layout(nk_context* ctx) override;
	virtual void SafeRenderStart() override;
	virtual void SafeRenderEnd() override;
	virtual void EditInfo() override;
	virtual void EditStyle() override;
	virtual void EditComponentStyle(nk_context* ctx, NuklearUI* pManager) override;

public:
	template <class Archive>
	void serialize(Archive& ar) {
		ar(cereal::base_class<NKBase>(this)
			, cereal::base_class<NKBaseWindow>(this)
			, cereal::base_class<NKStyleHeader>(this)
			, cereal::base_class<NKStyleWindow>(this)
		);
	}
};

#endif //NKGroup_h__
