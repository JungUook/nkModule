#pragma once
#ifndef NKEdit_h__
#define NKEdit_h__
#include "NKBase.h"
#include "NKHandler.h"
#include "NKStyleEdit.h"
class NKEdit : public NKBase, public NKHandler, public NKStyleEdit
{
public:
	NKEdit();
	NKEdit(nk_context* ctx, NuklearUI* pManager);
	NKEdit(const NKEdit& other);
	virtual ~NKEdit();
	

public:
	virtual void Layout(nk_context* ctx) override;
	virtual void SafeRenderStart(nk_context* ctx) override;
	virtual void SafeRenderEnd(nk_context* ctx) override;
	virtual void EditInfo(nk_context* ctx) override;
	virtual void EditStyle(nk_context* ctx) override;
	void Clear();
	bool CClear(void* param);

	void SetText(const char* text);
	void LSetText(luabridge::LuaRef ref);
	bool CSetText(void* param);

	virtual void RegistCommand(const char* classname) override;
public:
	char m_inputText[256];
	int m_inputTextLength;
	nk_plugin_filter m_filter;

public:
	template <class Archive>
	void serialize(Archive& ar, const unsigned int version) {
		ar(cereal::base_class<NKBase>(this)
			, cereal::base_class<NKHandler>(this)
			, cereal::base_class<NKStyleEdit>(this)
			, CEREAL_NVP(m_inputText)
			, CEREAL_NVP(m_inputTextLength)
		);
	}
};


#endif //NKEdit_h__