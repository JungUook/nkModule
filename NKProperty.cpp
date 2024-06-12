#include "pch.h"
#include "NKProperty.h"

NKProperty::NKProperty() : NKStyle(), NKTransform()
{
}

NKProperty::NKProperty(const NKProperty& other) : NKStyle(other), NKTransform(other)
{
}

NKProperty::~NKProperty()
{
}