#include "pch.h"
#include "NKProperty.h"
NKProperty::NKProperty() : NKBaseStyle(), NKTransform()
, m_iPrimaryID(0)
, m_iPrimaryEditName_len(0)
, m_iWindowEditName_len(0)
, m_iBaseEditName_len(0)
, m_iParentPrimaryID(0)
, m_type(eNONE)
, m_iWindowPrimaryID(0)
{
	m_sPrimaryName = "None";
	m_sWindowName = "None";
	m_sBaseName = "None";
	memset(m_cPrimaryEditName, 0, sizeof(m_cPrimaryEditName));
	memset(m_cWindowEditName, 0, sizeof(m_cWindowEditName));
	memset(m_cBaseEditName, 0, sizeof(m_cBaseEditName));
}

NKProperty::NKProperty(nk_context* ctx, NuklearUI* pManager) : NKBaseStyle(ctx, pManager), NKTransform()
, m_iPrimaryID(0)
, m_iPrimaryEditName_len(0)
, m_iWindowEditName_len(0)
, m_iBaseEditName_len(0)
, m_iParentPrimaryID(0)
, m_type(eNONE)
, m_iWindowPrimaryID(0)
{
	m_sPrimaryName = "None";
	m_sWindowName = "None";
	m_sBaseName = "None";
	memset(m_cPrimaryEditName, 0, sizeof(m_cPrimaryEditName));
	memset(m_cWindowEditName, 0, sizeof(m_cWindowEditName));
	memset(m_cBaseEditName, 0, sizeof(m_cBaseEditName));
}

NKProperty::NKProperty(const NKProperty& other) : NKBaseStyle(other), NKTransform(other)
{
	m_iPrimaryID = reinterpret_cast<intptr_t>(this);
	m_iParentPrimaryID = other.m_iParentPrimaryID;
	m_iWindowPrimaryID = other.m_iWindowPrimaryID;
	m_iPrimaryEditName_len = other.m_iPrimaryEditName_len;
	m_iWindowEditName_len = other.m_iWindowEditName_len;
	m_iBaseEditName_len = other.m_iBaseEditName_len;
	m_type = other.m_type;
	m_sPrimaryName = other.m_sPrimaryName;
	m_sWindowName = other.m_sWindowName;
	m_sBaseName = other.m_sBaseName;
	strcpy_s(m_cPrimaryEditName, other.m_cPrimaryEditName);
	strcpy_s(m_cWindowEditName, other.m_cWindowEditName);
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
	m_sPrimaryName = className.c_str();
	m_sWindowName = className.c_str();
	m_sBaseName = className.c_str();
}

unsigned int NKProperty::GetPrimaryID()
{
	return m_iPrimaryID;
}

const char* NKProperty::GetPrimaryName()
{
	return m_sPrimaryName.c_str();
}

const char* NKProperty::GetWindowName()
{
	return m_sWindowName.c_str();
}

const char* NKProperty::GetBaseName()
{
	return m_sBaseName.c_str();
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
	m_sPrimaryName = name;
}
void NKProperty::SetWindowName(const char* name)
{
	m_sWindowName = name;
}

void NKProperty::SetBaseName(const char* name)
{
	m_sBaseName = name;
}