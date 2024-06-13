#pragma once
#ifndef NKHandler_h__
#define NKHandler_h__
class NuklearUI;

class NKHandler
{
public:
	NKHandler();
	NKHandler(const NKHandler& other);
	~NKHandler();

	virtual void RegistFunction(const char* functionName, const char* argsName = nullptr);
	virtual void CallEvent(NuklearUI* pManager);
	virtual void CallEvent(NuklearUI* pManager, nk_edit_events edit_event, char* inputText, int* inputTextLength);

protected:
	void EditInfoData(nk_context* ctx);

public:
	char m_functionName[64];
	char m_argsName[64];
};
#endif //NKHandler_h__