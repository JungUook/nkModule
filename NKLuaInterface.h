#pragma once
#ifndef NKLuaInterface_h__
#define NKLuaInterface_h__

#include "LuaLibrary.h"
#include "LuaBridge/LuaBridge.h"

#include <vector>
#include <map>
#include <list>
#include <fstream>
#include <unordered_map>
#include <commdlg.h>
#include <string>
#include <filesystem>

class NKBase;
class NKHandler;
class NuklearUI;
class NKObjectFinder;

#define CEREAL_NVP(T) ::cereal::make_nvp(#T, T)

struct CustomData {
	char name[256];
	bool bFunction;
	int iVarType;
	std::vector<NKHandler*> vUseObj;
	CustomData() : bFunction(false) {
		memset(name, 0, sizeof(name));
	}

	template <class Archive>
	void serialize(Archive& ar, const unsigned int version) {
		if (version >= 5) {
			ar(CEREAL_NVP(name)
				, CEREAL_NVP(bFunction)
				, CEREAL_NVP(iVarType)
			);
		}
		else {
			ar(CEREAL_NVP(name)
				, CEREAL_NVP(bFunction)
			);
		}
	}
};

struct BindingFunc {
	void* binding;
	void(*func)(void*, void*);
};

class NKLuaInterface
{
public:
	NKLuaInterface();
	~NKLuaInterface();

public:
	void Init();
	void Release();

	lua_State* GetLua();
	//lua
public:
	void LoadLuaFile(const char* filePath);
	luabridge::LuaRef GetLuaTable(const char* tableName);
	luabridge::LuaRef DeepCopy(const char* tableName);
	luabridge::LuaRef DeepCopy(const luabridge::LuaRef& source, lua_State* L);
	bool RunFunction(const char* functionName);
	bool RunFunctionArgs(const char* functionName, const luabridge::LuaRef& args);

	void ResponseFunction(const char* functionName);
	void ResponseFunctionArgs(const char* functionName, const luabridge::LuaRef& args);

	void SubscribeVariable(std::string key, NKHandler* handler);
	void SubscribeFunction(std::string key, NKHandler* handler);

	void UnsubscribeVariable(std::string key, NKHandler* handler);
	void UnsubscribeFunction(std::string key, NKHandler* handler);

	bool IsActiveFunction(std::string functionname);
	bool IsActiveVariable(std::string variablename);

	luabridge::LuaRef GetLuaTable(std::string variablename);

	static std::wstring utf8ToWstring(const char* str);
	static bool customCompare(const CustomData aData, const CustomData bData);
#ifdef _NKDEBUG
	void DebugLoadLuaFile(const char* filePath);
#endif // _NKDEBUG

	void BindingTriggerEvent(luabridge::LuaRef args);

	void* ConvertData(luabridge::LuaRef params);

	void* NKGetData(const char* key);
	int NKGetDataInt(const char* key);
	float NKGetDataFloat(const char* key);
	std::string NKGetDataString(const char* key);
	bool NKGetDataBool(const char* key);

	void RegisterBase();

public:
	std::map<std::string, CustomData> m_mapVariable;
	std::map<std::string, CustomData> m_mapFunction;
	NuklearUI* m_pManager;

	std::map<std::string, BindingFunc> m_mapBindingEventHandlers;

	std::list<double> m_lRef_d;
	std::list<std::string> m_lRef_s;
	std::list<int> m_lRef_b;

	std::map<std::string, void*> m_mTableRef;
	double m_dRef;
	std::string m_sRef;
	bool m_bRef;
private:
	lua_State* m_lua;



#ifdef _NKDEBUG
	std::ofstream logFile;
#endif // _NKDEBUG
	void logToFile(const std::string& message);
};


#endif // !NKLuaInterface_h__