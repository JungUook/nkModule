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
	~NKProperty();
};
#endif //NKProperty_h__