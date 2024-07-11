#pragma once
#ifndef NKCombo_h__
#define NKCombo_h__
#include "NKBase.h"
#include "NKStyleCombo.h"
class NKCombo : public NKBase, public NKStyleCombo
{
public:
	NKCombo();
	NKCombo(nk_context* ctx, NuklearUI* pManager);
	NKCombo(const NKCombo& other);
	virtual ~NKCombo();
	

public:
	virtual void Layout(nk_context* ctx) override;
	virtual void SafeRenderStart(nk_context* ctx) override;
	virtual void SafeRenderEnd(nk_context* ctx) override;
	virtual void EditInfo(nk_context* ctx) override;
	virtual void EditStyle(nk_context* ctx) override;

	void SetComboName(const char* name);
	void LSetComboName(luabridge::LuaRef ref);
	bool CSetComboName(void* param);
	void SetLabelSize(float x, float y);
	void LSetLabelSize(luabridge::LuaRef ref);
	bool CSetLabelSize(void* param);
	void SetCurrentLabel(int number);
	virtual void RegistCommand(const char* classname) override;
public:
	int m_currentLabel;
	nk_text_alignment m_labelAlignment;
	struct nk_vec2 m_labelSize;
	std::string m_cComboLabel;

public:
	template <class Archive>
	void serialize(Archive& ar, const unsigned int version) {
		ar(cereal::base_class<NKBase>(this)
			, cereal::base_class<NKStyleCombo>(this)
			, CEREAL_NVP(m_currentLabel)
			, CEREAL_NVP(m_labelAlignment)
			, CEREAL_NVP(m_labelSize)
			, CEREAL_NVP(m_cComboLabel)
		);
	}
};


#endif //NKCombo_h__