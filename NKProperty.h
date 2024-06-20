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
	NKProperty(nk_context* ctx, NuklearUI* pManager);
	NKProperty(const NKProperty& other);
	virtual ~NKProperty();

	//각 객체의 기본값
public:
	virtual void Initialize();

	virtual std::string getClassName() const;
	virtual unsigned int GetPrimaryID();
	virtual const char* GetPrimaryName();
	virtual const char* GetBaseName();
	virtual unsigned int GetParentPrimaryID();
	virtual eTypeUI GetType();

	virtual void SetPrimaryID(unsigned int id);
	virtual void SetPrimaryName(const char* name);
	virtual void SetBaseName(const char* name);
protected:
	unsigned int m_iPrimaryID;
	char m_cprimaryName[64];
	char m_cprimaryEditName[64];
	int m_cprimaryEditName_len;

	char m_cBaseName[64];
	char m_cBaseEditName[64];
	int m_cBaseEditName_len;

	unsigned int m_iParentPrimaryID;
	eTypeUI m_type;
public:
	template <class Archive>
	void serialize(Archive& ar) {
		ar(cereal::base_class<NKBaseStyle>(this)
			, cereal::base_class<NKTransform>(this)
			, m_iPrimaryID
			, m_cprimaryName
			, m_cprimaryEditName
			, m_cprimaryEditName_len
			, m_cBaseName
			, m_cBaseEditName
			, m_cBaseEditName_len
			, m_iParentPrimaryID
			, m_type
		);
	}
};
#endif //NKProperty_h__