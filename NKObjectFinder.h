#pragma once
#ifndef NKObjectFinder_h__
#define NKObjectFinder_h__

class NuklearUI;
class NKBase;

class NKObjectFinder
{
public:
	NKObjectFinder(NuklearUI* pManager);
	NKObjectFinder(const NKObjectFinder& other);
	~NKObjectFinder();

public:
	void FoundObject(nk_context* ctx, NuklearUI* pManager);
	NKBase* SearchObject(nk_context* ctx, NuklearUI* pManager);

	void LostObjectEvent(unsigned int id);
		
protected:
	char m_cSearchObject[256];
	int m_iSearchObjectLen;

	NKBase* m_pResultObject;

};
#endif NKObjectFinder_h__