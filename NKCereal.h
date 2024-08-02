#pragma once
#ifndef NKCereal_h__
#define NKCereal_h__

#include "NKLuaInterface.h"

#include <vector>
#include <map>
#include <list>
#include <fstream>
#include <unordered_map>
#include <commdlg.h>
#include <string>
#include <filesystem>

class NuklearUI;
class NKBase;
struct CustomData;
class sprData;

class NKCereal
{
public:
	NKCereal();
	~NKCereal();
	//project
public:
	void SaveFile(const std::string& filename);
	void SaveFileBinary(const std::string& filename);
	void LoadFile(std::map<std::string, CustomData>& vVar, std::map<std::string, CustomData>& vFunc, const std::string& filename);
	void LoadFileBinary(std::map<std::string, CustomData>& vVar, std::map<std::string, CustomData>& vFunc, const std::string& filename);

	//Prefab
public:
	void OpenPrefabDialog();
	void SavePrefab(const std::string& filename, NKBase* prefab);
	void SavePrefabBinary(const std::string& filename, NKBase* prefab);
	NKBase* LoadPrefab(const std::string& filename, NKBase* parent = nullptr);
	void LoadPrefabBinary(const std::string& filename, NKBase* parent = nullptr);
	NKBase* LLoadPrefab(luabridge::LuaRef ref);

	void OpenLuaCodeDialog();

	void OpenDialog(LPCWSTR strFilter, const wchar_t* strExtension, std::vector<std::string>& vec);
	std::string GetRelativePath(const char* absolutePath);
	std::string GetExecutablePath();

private:
	bool Contains(const std::vector<std::string>& vec, const std::string& str);
public:
	std::vector<std::string> m_vecPrefab;
	std::vector<std::string> m_vecLuaCode;
	NuklearUI* m_pManager;

public:
	std::vector<NKBase*> m_vecObject;
	std::vector<NKBase*> m_vecModule;
	std::map<unsigned int, NKBase*> m_mapModuleID;
	std::map<std::string, NKBase*> m_mapModuleName;
	std::map<std::string, sprData*> m_mapSpr;
};


#endif // NKCereal_h__