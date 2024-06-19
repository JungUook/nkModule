#pragma once
#ifndef NKProperty_h__
#define NKProperty_h__
#include "NKBaseStyle.h"
#include "NKTransform.h"

class NKBaseStyle;
class NKTransform;

class NKProperty : public NKBaseStyle, public NKTransform
{
public:
	NKProperty();
	NKProperty(const NKProperty& other);
	virtual ~NKProperty();

public:
	template <class Archive>
	void serialize(Archive& ar) {
		ar(cereal::base_class<NKBaseStyle>(this)
			, cereal::base_class<NKTransform>(this));
	}
};
#endif //NKProperty_h__