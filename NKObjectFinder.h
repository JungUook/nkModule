#pragma once
#ifndef NKObjectFinder_h__
#define NKObjectFinder_h__

class NuklearUI;
class NKBase;

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

	void LostObjectEvent(unsigned int id);
	void FailRegist();
	void RegistObjectEvent(NKBase* pBase);
	unsigned int GetLinkObjPrimaryID();
		
protected:
	char m_cSearchObject[256];
	int m_iSearchObjectLen;

	unsigned int m_iResultObjPrimaryID;
	NKBase* m_pResultObject;

public:
	template <class Archive>
	void serialize(Archive& ar) {
		ar(
			m_iResultObjPrimaryID
		);
	}
};
#endif NKObjectFinder_h__