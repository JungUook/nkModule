#pragma once
#ifndef NKHandler_h__
#define NKHandler_h__

#define CEREAL_NVP(T) ::cereal::make_nvp(#T, T)

class NuklearUI;
class NKLuaInterface;

class NKHandler
{
public:
	NKHandler();
	NKHandler(const NKHandler& other);
	virtual ~NKHandler();

	virtual void RegistFunction(const char* functionName, NKLuaInterface* pInterface);
	virtual void RegistVariable(const char* argsName, NKLuaInterface* pInterface);
	virtual void CallEvent(NKLuaInterface* pManager);
	virtual void CallEvent(NKLuaInterface* pManager, nk_edit_events edit_event, char* inputText, int* inputTextLength);
	virtual void CallbackEvent(NKLuaInterface* pManager);

	const char* GetFunctionName();
	const char* GetArgsName();

protected:
	virtual void EditInfoData(nk_context* ctx, NuklearUI* pManager, NKLuaInterface* pInterface);

public:
	std::string m_functionName;
	std::string m_argsName;

	char m_functionNameEdit[64];
	int m_functionNameEditLen;

	char m_argsNameEdit[64];
	int m_argsNameEditLen;

public:
	template <class Archive>
	void serialize(Archive& ar, const unsigned int version) {
		ar(CEREAL_NVP(m_functionName)
			, CEREAL_NVP(m_argsName)
		);
	}
};
#endif //NKHandler_h__