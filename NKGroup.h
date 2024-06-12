#pragma once
#ifndef NKGroup_h__
#define NKGroup_h__
#include "NKBase.h"
#include "NKBaseWindow.h"
class NKGroup : public NKBase, public NKBaseWindow
{
public:
	NKGroup();
	NKGroup(const NKGroup& other);
	~NKGroup();

public:
	virtual void Layout(nk_context* ctx) override;
	virtual void SafeRenderStart() override;
	virtual void SafeRenderEnd() override;
	virtual void EditInfo() override;
	virtual void EditStyle() override;
};

#endif //NKGroup_h__
