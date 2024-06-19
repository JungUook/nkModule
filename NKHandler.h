#pragma once
#ifndef NKHandler_h__
#define NKHandler_h__

class NuklearUI;

class NKHandler
{
public:
	NKHandler();
	NKHandler(const NKHandler& other);
	virtual ~NKHandler();

	virtual void RegistFunction(const char* functionName, const char* argsName = nullptr);
	virtual void CallEvent(NuklearUI* pManager);
	virtual void CallEvent(NuklearUI* pManager, nk_edit_events edit_event, char* inputText, int* inputTextLength);

protected:
	void EditInfoData(nk_context* ctx);

public:
	char m_functionName[64];
	char m_argsName[64];

public:
	template <class Archive>
	void serialize(Archive& ar) {
		ar(m_functionName
			, m_argsName
		);
	}
};
#endif //NKHandler_h__