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
class NKObjectFinder;

struct CustomData {
	char name[256];
	int nameLen;
	char tableName[256];
	int tableLen;

	std::vector<NKBase*> vUseObj;
	CustomData() : nameLen(0), tableLen(0) {
		memset(name, 0, sizeof(name));
		memset(tableName, 0, sizeof(tableName));
	}

	template <class Archive>
	void serialize(Archive& ar) {
		ar(name
			, nameLen
			, tableName
			, tableLen
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
	void ResponseFunction(const char* functionName, const luabridge::LuaRef& args);

	void AddVariable(CustomData& var);
	void AddFunction(CustomData& func);

	static std::wstring utf8ToWstring(const char* str);
	static bool customCompare(const CustomData aData, const CustomData bData);
#ifdef _NKDEBUG
	void DebugLoadLuaFile(const char* filePath);
#endif // _NKDEBUG

	void RegisterBase();

public:
	std::vector<CustomData> m_vecVariable;
	std::vector<CustomData> m_vecFunction;

private:
	lua_State* m_lua;

#ifdef _NKDEBUG
	char m_filePath[256];
#endif // _NKDEBUG
};


#endif // !NKLuaInterface_h__