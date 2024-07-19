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
	virtual const char* GetWindowName();
	virtual const char* GetBaseName();
	virtual unsigned int GetParentPrimaryID();
	virtual eTypeUI GetType();

	virtual void SetPrimaryID(unsigned int id);
	virtual void SetPrimaryName(const char* name);
	virtual void SetWindowName(const char* name);
	virtual void SetBaseName(const char* name);
protected:
	unsigned int m_iPrimaryID;

	std::string m_sPrimaryName;
	char m_cPrimaryEditName[64];
	int m_iPrimaryEditName_len;

	std::string m_sWindowName;
	char m_cWindowEditName[64];
	int m_iWindowEditName_len;

	std::string m_sBaseName;
	char m_cBaseEditName[64];
	int m_iBaseEditName_len;

	unsigned int m_iWindowPrimaryID;
	unsigned int m_iParentPrimaryID;
	eTypeUI m_type;
public:
	template <class Archive>
	void serialize(Archive& ar, const unsigned int version) {
		ar(cereal::base_class<NKBaseStyle>(this)
			, cereal::base_class<NKTransform>(this)
			, CEREAL_NVP(m_iPrimaryID)
			, CEREAL_NVP(m_sPrimaryName)
			, CEREAL_NVP(m_cPrimaryEditName)
			, CEREAL_NVP(m_iPrimaryEditName_len)
			, CEREAL_NVP(m_sWindowName)
			, CEREAL_NVP(m_cWindowEditName)
			, CEREAL_NVP(m_iWindowEditName_len)
			, CEREAL_NVP(m_sBaseName)
			, CEREAL_NVP(m_cBaseEditName)
			, CEREAL_NVP(m_iBaseEditName_len)
			, CEREAL_NVP(m_iWindowPrimaryID)
			, CEREAL_NVP(m_iParentPrimaryID)
			, CEREAL_NVP(m_type)
		);
	}
};
#endif //NKProperty_h__