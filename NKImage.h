#pragma once
#ifndef NKImage_h__
#define NKImage_h__
#include "NKBase.h"
class NKImage : public NKBase
{
public:
	NKImage();
	~NKImage();

public:
	virtual void Layout(nk_context* ctx) override;
	virtual void EditInfo() override;

public:
	std::string m_imagePath;
	int m_sprIndex;
	int m_sprSize;
};


#endif //NKImage_h__