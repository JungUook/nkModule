#pragma once
#ifndef NKCereal_h__
#define NKCereal_h__

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
	void SaveFile(std::vector<CustomData>& vVar, std::vector<CustomData>& vFunc, const std::string& filename);
	void LoadFile(std::vector<CustomData>& vVar, std::vector<CustomData>& vFunc, const std::string& filename);

	//Prefab
public:
	void OpenPrefabDialog();
	void SavePrefab(const std::string& filename, NKBase* prefab);
	void LoadPrefab(const std::string& filename, NKBase* parent = nullptr);

private:
	bool Contains(const std::vector<std::string>& vec, const std::string& str);

public:
	std::vector<std::string> m_vecPrefab;
	NuklearUI* m_pManager;

public:
	std::vector<NKBase*> m_vecObject;
	std::vector<NKBase*> m_vecModule;
	std::map<unsigned int, NKBase*> m_mapModuleID;
	std::map<std::string, NKBase*> m_mapModuleName;
	std::map<std::string, sprData*> m_mapSpr;
};


#endif // NKCereal_h__