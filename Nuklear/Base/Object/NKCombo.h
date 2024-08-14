#pragma once
#ifndef NKCombo_h__
#define NKCombo_h__
#include "NKBase.h"
#include "NKStyleCombo.h"
#include "NKStyleContextualButton.h"
#include "NKStyleWindow.h"
#include "NKStyleScrollbarH.h"
#include "NKStyleScrollbarV.h"

class NKComboItem;

class NKCombo : public NKBase, public NKStyleCombo, public NKStyleContextualButton, public NKStyleWindow, public NKStyleScrollbarH, public NKStyleScrollbarV
{
public:
	enum {
		eCOMBO_DYNAMIC,
		eCOMBO_STATIC
	};

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

	NKComboItem* AddItem(const char* name);
	NKComboItem* LAddItem(luabridge::LuaRef ref);
	bool CAddItem(void* param);

	virtual void UpdateComponent(nk_context* ctx, NuklearUI* pManager) override;
	virtual void EditComponentStyle(nk_context* ctx, NuklearUI* pManager) override;
	virtual void RegistCommand(const char* classname) override;
public:
	int m_currentLabel;
	nk_text_alignment m_labelAlignment;
	struct nk_vec2 m_labelSize;
	struct nk_vec2 m_itemSize;
	std::string m_cComboLabel;

	int m_iComboFlag;

	struct nk_vec2 m_dynamicLabelSpace; // No need to save it.
public:
	template <class Archive>
	void serialize(Archive& ar, const unsigned int version) {
		if (version >= 12) {
			ar(cereal::base_class<NKBase>(this)
				, cereal::base_class<NKStyleCombo>(this)
				, cereal::base_class<NKStyleContextualButton>(this)
				, cereal::base_class<NKStyleWindow>(this)
				, cereal::base_class<NKStyleScrollbarH>(this)
				, cereal::base_class<NKStyleScrollbarV>(this)
				, CEREAL_NVP(m_currentLabel)
				, CEREAL_NVP(m_labelAlignment)
				, CEREAL_NVP(m_labelSize)
				, CEREAL_NVP(m_itemSize)
				, CEREAL_NVP(m_cComboLabel)
				, CEREAL_NVP(m_iComboFlag)
			);
		}
		else {
			ar(cereal::base_class<NKBase>(this)
				, cereal::base_class<NKStyleCombo>(this)
				, cereal::base_class<NKStyleContextualButton>(this)
				, cereal::base_class<NKStyleWindow>(this)
				, cereal::base_class<NKStyleScrollbarH>(this)
				, cereal::base_class<NKStyleScrollbarV>(this)
				, CEREAL_NVP(m_currentLabel)
				, CEREAL_NVP(m_labelAlignment)
				, CEREAL_NVP(m_labelSize)
				, CEREAL_NVP(m_cComboLabel)
				, CEREAL_NVP(m_iComboFlag)
			);
		}
	}
};


#endif //NKCombo_h__