#include "pch.h"
#include "NKProperty.h"
NKProperty::NKProperty() : NKBaseStyle(), NKTransform()
, m_iPrimaryID(0)
, m_cprimaryEditName_len(0)
, m_cBaseEditName_len(0)
, m_iParentPrimaryID(0)
, m_type(eNONE)
{
	memset(m_cprimaryName, 0, sizeof(m_cprimaryName));
	memset(m_cBaseName, 0, sizeof(m_cBaseName));
	memset(m_cprimaryEditName, 0, sizeof(m_cprimaryEditName));
	memset(m_cBaseEditName, 0, sizeof(m_cBaseEditName));
}

NKProperty::NKProperty(nk_context* ctx, NuklearUI* pManager) : NKBaseStyle(ctx, pManager), NKTransform()
, m_iPrimaryID(0)
, m_cprimaryEditName_len(0)
, m_cBaseEditName_len(0)
, m_iParentPrimaryID(0)
, m_type(eNONE)
{
	memset(m_cprimaryName, 0, sizeof(m_cprimaryName));
	memset(m_cBaseName, 0, sizeof(m_cBaseName));
	memset(m_cprimaryEditName, 0, sizeof(m_cprimaryEditName));
	memset(m_cBaseEditName, 0, sizeof(m_cBaseEditName));
}

NKProperty::NKProperty(const NKProperty& other) : NKBaseStyle(other), NKTransform(other)
{
	m_iPrimaryID = other.m_iPrimaryID;
	m_iParentPrimaryID = other.m_iParentPrimaryID;
	m_cprimaryEditName_len = other.m_cprimaryEditName_len;
	m_cBaseEditName_len = other.m_cBaseEditName_len;
	m_type = other.m_type;
	strcpy_s(m_cprimaryName, other.m_cprimaryName);
	strcpy_s(m_cBaseName, other.m_cBaseName);
	strcpy_s(m_cprimaryEditName, other.m_cprimaryEditName);
	strcpy_s(m_cBaseEditName, other.m_cBaseEditName);
}

NKProperty::~NKProperty()
{
}

std::string NKProperty::getClassName() const
{
	std::string className = typeid(*this).name();
	std::string prefix = "class ";

	// Remove the prefix if it exists
	if (className.find(prefix) == 0) {
		className = className.substr(prefix.length());
	}

	return className;
}

void NKProperty::Initialize()
{
	std::string className = getClassName().c_str();
	strcpy_s(m_cprimaryName, className.c_str());
	strcpy_s(m_cBaseName, className.c_str());
}

unsigned int NKProperty::GetPrimaryID()
{
	return m_iPrimaryID;
}

const char* NKProperty::GetPrimaryName()
{
	return m_cprimaryName;
}

const char* NKProperty::GetBaseName()
{
	return m_cBaseName;
}

unsigned int NKProperty::GetParentPrimaryID()
{
	return m_iParentPrimaryID;
}

eTypeUI NKProperty::GetType()
{
	return m_type;
}

void NKProperty::SetPrimaryID(unsigned int id)
{
	m_iPrimaryID = id;
}

void NKProperty::SetPrimaryName(const char* name)
{
	memset(m_cprimaryName, 0, sizeof(m_cprimaryName));
	strcpy_s(m_cprimaryName, name);
}

void NKProperty::SetBaseName(const char* name)
{
	memset(m_cBaseName, 0, sizeof(m_cBaseName));
	strcpy_s(m_cBaseName, name);
}
