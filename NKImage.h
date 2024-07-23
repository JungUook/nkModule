#pragma once
#ifndef NKImage_h__
#define NKImage_h__
#include "NKBase.h"
#include "NKBaseImage.h"
class NKImage : public NKBase, public NKBaseImage
{
public:
	NKImage();
	NKImage(nk_context* ctx, NuklearUI* pManager);
	NKImage(const NKImage& other);
	virtual ~NKImage();
	

public:
	virtual void Layout(nk_context* ctx) override;
	virtual void SafeRenderStart(nk_context* ctx) override;
	virtual void SafeRenderEnd(nk_context* ctx) override;
	virtual void EditInfo(nk_context* ctx) override;

	virtual void RegistCommand(const char* classname) override;

public:
	template <class Archive>
	void serialize(Archive& ar, const unsigned int version) {
		if (version >= 4) {
			ar(cereal::base_class<NKBase>(this),
				cereal::base_class<NKBaseImage>(this)
				);
		}
		else {
			ar(cereal::base_class<NKBase>(this)
				, CEREAL_NVP(m_imagePath)
				, CEREAL_NVP(m_sprIndex)
				, CEREAL_NVP(m_sprSize)
			);
		}
	}
};


#endif //NKImage_h__