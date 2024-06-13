#include "pch.h"
#include "NKProperty.h"

NKProperty::NKProperty() : NKBaseStyle(), NKTransform()
{
}

NKProperty::NKProperty(const NKProperty& other) : NKBaseStyle(other), NKTransform(other)
{
}

NKProperty::~NKProperty()
{
}