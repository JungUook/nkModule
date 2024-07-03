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

struct CustomData {
	char name[256];
	bool bFunction;

	char desc[512];
	int descLen;
	std::vector<NKHandler*> vUseObj;
	CustomData() : bFunction(false), descLen(0) {
		memset(name, 0, sizeof(name));
		memset(desc, 0, sizeof(desc));
	}

	template <class Archive>
	void serialize(Archive& ar) {
		ar(name
			, bFunction
			, desc
			, descLen
		);
	}
};

class NKLuaInterface
{
public:
	NKLuaInterface();
	~NKLuaInterface();

public:
	void Init();
	void Release();

	//lua
public:
	void LoadLuaFile(const char* filePath);
	luabridge::LuaRef GetLuaTable(const char* tableName);
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

	void RegisterBase();

public:
	std::map<std::string, CustomData> m_mapVariable;
	std::map<std::string, CustomData> m_mapFunction;
	NuklearUI* m_pManager;

private:
	lua_State* m_lua;
};


#endif // !NKLuaInterface_h__