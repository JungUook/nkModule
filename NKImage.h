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
	void Layout(nk_context* ctx) override;

	void SetImage(int SID);
public:
	struct nk_image m_image;
};


#endif //NKImage_h__