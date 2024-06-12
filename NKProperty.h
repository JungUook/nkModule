#pragma once
#ifndef NKProperty_h__
#define NKProperty_h__
#include "NKStyle.h"
#include "NKTransform.h"

class NKStyle;
class NKTransform;

class NKProperty : public NKStyle, public NKTransform
{
public:
	NKProperty();
	NKProperty(const NKProperty& other);
	~NKProperty();
};
#endif //NKProperty_h__