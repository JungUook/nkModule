#pragma once
#ifndef NKLabel_h__
#define NKLabel_h__
#include "NKBase.h"
#include "NKBaseLabel.h"
#include "NKStyleText.h"
class NKLabel : public NKBase, public NKBaseLabel, public NKStyleText
{
public:
	NKLabel();
	NKLabel(nk_context* ctx, NuklearUI* pManager);
	NKLabel(const NKLabel& other);
	virtual ~NKLabel();
	

public:
	virtual void LayoutBegin(nk_context* ctx) override;
	virtual void Layout(nk_context* ctx) override;
	virtual void LayoutEnd(nk_context* ctx) override;
	virtual void SafeRenderStart(nk_context* ctx) override;
	virtual void SafeRenderEnd(nk_context* ctx) override;
	virtual void EditInfo(nk_context* ctx) override;
	virtual void EditStyle(nk_context* ctx) override;

	virtual void RegistCommand(const char* classname) override;
public:
	nk_bool m_bWrap;
	nk_bool m_bBold;
	nk_bool m_bOutline;

public:
    template <class Archive>
    void serialize(Archive& ar, const unsigned int version) {
		if (version >= 10) {
			ar(cereal::base_class<NKBase>(this)
				, cereal::base_class<NKBaseLabel>(this)
				, cereal::base_class<NKStyleText>(this)
				, CEREAL_NVP(m_bWrap)
				, CEREAL_NVP(m_bBold)
				, CEREAL_NVP(m_bOutline)
			);
		}
		else {
			ar(cereal::base_class<NKBase>(this)
				, cereal::base_class<NKBaseLabel>(this)
				, cereal::base_class<NKStyleText>(this)
				, CEREAL_NVP(m_bWrap)
			);
		}
    }
};


#endif //NKLabel_h__