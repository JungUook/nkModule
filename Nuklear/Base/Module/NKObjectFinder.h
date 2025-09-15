#pragma once
#ifndef NKObjectFinder_h__
#define NKObjectFinder_h__

class NuklearUI;
class NKBase;

#define CEREAL_NVP(T) ::cereal::make_nvp(#T, T)
class NKObjectFinder
{
public:
	NKObjectFinder();
	NKObjectFinder(NuklearUI* pManager);
	NKObjectFinder(const NKObjectFinder& other);
	virtual ~NKObjectFinder();

public:
	void FoundObject(nk_context* ctx, NuklearUI* pManager);
	NKBase* SearchObject(nk_context* ctx, NuklearUI* pManager);

	void LostObjectEvent(uintptr_t id);
	void FailRegist();
	void RegistObjectEvent(NKBase* pBase);
	uintptr_t GetLinkObjPrimaryID();
		
protected:
	char m_cSearchObject[256];
	int m_iSearchObjectLen;

	uintptr_t m_iResultObjPrimaryID;
	NKBase* m_pResultObject;

public:
	template <class Archive>
	void serialize(Archive& ar, const unsigned int version) {
		ar(
			CEREAL_NVP(m_iResultObjPrimaryID)
		);
	}
};
#endif NKObjectFinder_h__