#pragma once
#ifndef NKBaseLabel_h__
#define NKBaseLabel_h__

class NuklearUI;

class NKBaseLabel
{
public:
	NKBaseLabel();
	NKBaseLabel(const NKBaseLabel& other);
	~NKBaseLabel();

	virtual nk_flags EditLabel(nk_context* ctx, NuklearUI* pManager);
	virtual void SetLabel(const char* text);
protected:
	char m_cEditLabel[256];
	int m_iEditLabelLen;
	char m_cContent[256];
};

#endif //NKBaseLabel_h__