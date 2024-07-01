#include "pch.h"
#include "NKCereal.h"
#include "NuklearUI.h"
#include "UiLibrary.h"

#include <cereal/types/vector.hpp>
#include <cereal/types/array.hpp>
#include <cereal/types/string.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/types/polymorphic.hpp>

NKCereal::NKCereal()
{
}

NKCereal::~NKCereal()
{
}

void NKCereal::SaveFile(std::vector<CustomData>& vVar, std::vector<CustomData>& vFunc, const std::string& filename)
{
	std::vector<std::string> vSprData;
	size_t size = m_vecModule.size();
	std::vector<std::string> vStr;

	for (auto it = m_mapSpr.begin(); it != m_mapSpr.end(); ++it) {
		std::string str = it->first;
		vSprData.push_back(str.c_str());
	}


	for (size_t i = 0; i < size; ++i) {
		std::string str = m_vecModule.at(i)->getClassName();
		vStr.push_back(str);
	}

	std::ofstream os(filename);
	cereal::JSONOutputArchive archive(os);


	archive(CEREAL_NVP(vSprData));

	archive(CEREAL_NVP(vVar));
	archive(CEREAL_NVP(vFunc));
	archive(CEREAL_NVP(m_vecPrefab));

	archive(CEREAL_NVP(size));
	archive(CEREAL_NVP(vStr));


	for (size_t i = 0; i < size; ++i) {
		NKBase* ptr = m_vecModule.at(i);
		SaveSwitch(ptr, archive);
	}
}

void NKCereal::LoadFile(std::vector<CustomData>& vVar, std::vector<CustomData>& vFunc, const std::string& filename)
{
	std::vector<std::string> vSprData;
	size_t size;
	std::vector<std::string> vStr;

	std::ifstream is(filename);
	cereal::JSONInputArchive archive(is);

	archive(CEREAL_NVP(vSprData));


	wchar_t originalDir[MAX_PATH] = { 0, };
	GetCurrentDirectoryW(MAX_PATH, originalDir);
	for (size_t i = 0; i < vSprData.size(); ++i) {
		std::string str = vSprData.at(i);
		m_pManager->LoadSprFile(str.c_str());
	}
	SetCurrentDirectoryW(originalDir);

	archive(CEREAL_NVP(vVar));
	archive(CEREAL_NVP(vFunc));
	archive(CEREAL_NVP(m_vecPrefab));

	archive(CEREAL_NVP(size));
	archive(CEREAL_NVP(vStr));

	for (size_t i = 0; i < size; ++i) {
		NKBase* ptr = m_pManager->SimpleCreateUI(vStr.at(i).c_str());
		ptr->Initialize(m_pManager);
		LoadSwitch(ptr, archive, i);
		m_pManager->RegistUI(vStr.at(i).c_str(), ptr);
	}

	for (auto it = m_vecModule.begin(); it != m_vecModule.end(); ++it) {
		m_pManager->LoadNode(*it);
	}

	for (auto it = m_vecModule.begin(); it != m_vecModule.end(); ++it) {
		NKBase* pBase = *it;
		m_pManager->ResetPrimaryID(pBase);
		m_mapModuleID.insert(std::make_pair(pBase->GetPrimaryID(), pBase));
	}

	for (auto it = m_vecObject.begin(); it != m_vecObject.end(); ++it) {
		NKBase* pBase = *it;
		pBase->ResetWindowID(pBase);
	}

	for (auto it = m_vecObject.begin(); it != m_vecObject.end(); ++it) {
		NKBase* pBase = *it;
		auto list = pBase->GetChildList();
		for (auto child = list->begin(); child != list->end(); ++child) {
			NKBase* pChild = *child;
			pChild->ResetParentID(pBase);
		}
	}
}

void NKCereal::OpenPrefabDialog()
{
	wchar_t originalDir[MAX_PATH] = { 0, };
	GetCurrentDirectoryW(MAX_PATH, originalDir);

	OPENFILENAMEW ofn;
	const size_t buffer_size = 65536; // 충분히 큰 버퍼 크기
	wchar_t* szFile = new wchar_t[buffer_size];
	ZeroMemory(szFile, buffer_size * sizeof(wchar_t));
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = buffer_size;
	ofn.lpstrFilter = L"All Files\0*.*\0Prefab Files\0*.json\0";
	ofn.nFilterIndex = 2; // 기본 선택을 SPR Files로 설정
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT | OFN_EXPLORER;

	if (GetOpenFileNameW(&ofn) == TRUE) {
		wchar_t* p = szFile;
		std::wstring directory = p;
		p += directory.length() + 1;

		while (*p) {
			std::wstring filePath = directory + L"\\" + p;
			std::filesystem::path path(filePath);
			std::wstring extension = path.extension().wstring();
			std::transform(extension.begin(), extension.end(), extension.begin(), towlower);
			if (extension == L".json") {
				int size_needed = WideCharToMultiByte(CP_UTF8, 0, filePath.c_str(), -1, NULL, 0, NULL, NULL);
				char* result = new char[size_needed];
				WideCharToMultiByte(CP_UTF8, 0, filePath.c_str(), -1, result, size_needed, NULL, NULL);

				if (!Contains(m_vecPrefab, result)) {
					m_vecPrefab.push_back(result);
				}
				else {
				}

				delete[] result;
			}
			p += wcslen(p) + 1;
		}

		// If only one file is selected, GetOpenFileNameW does not add the directory separately
		if (directory.length() > 0 && *p == '\0') {
			std::filesystem::path path(directory);
			std::wstring extension = path.extension().wstring();
			std::transform(extension.begin(), extension.end(), extension.begin(), towlower);
			if (extension == L".json") {
				int size_needed = WideCharToMultiByte(CP_UTF8, 0, directory.c_str(), -1, NULL, 0, NULL, NULL);
				char* result = new char[size_needed];
				WideCharToMultiByte(CP_UTF8, 0, directory.c_str(), -1, result, size_needed, NULL, NULL);

				if (!Contains(m_vecPrefab, result)) {
					m_vecPrefab.push_back(result);
				}
				else {
				}

				delete[] result;
			}
		}
	}

	delete[] szFile; // 동적으로 할당한 메모리 해제

	SetCurrentDirectoryW(originalDir);
}

void NKCereal::SavePrefab(const std::string& filename, NKBase* prefab)
{
	std::vector<NKBase*> vPrefab;
	std::vector<std::string> vStr;

	prefab->GetPrefab(vPrefab);
	size_t size = vPrefab.size();

	for (size_t i = 0; i < size; ++i) {
		std::string str = vPrefab.at(i)->getClassName();
		vStr.push_back(str);
	}

	std::ofstream os(filename + ".json");
	cereal::JSONOutputArchive archive(os);

	archive(CEREAL_NVP(size));
	archive(CEREAL_NVP(vStr));


	for (size_t i = 0; i < size; ++i) {
		NKBase* ptr = vPrefab.at(i);
		SaveSwitch(ptr, archive);
	}

	if (!Contains(m_vecPrefab, filename + ".json")) {
		m_vecPrefab.push_back(filename + ".json");
	}
	else {
	}
}

void NKCereal::LoadPrefab(const std::string& filename, NKBase* parent)
{
	size_t size;
	std::vector<NKBase*> vPrefab;
	std::vector<std::string> vStr;

	std::ifstream is(filename);
	cereal::JSONInputArchive archive(is);

	archive(CEREAL_NVP(size));
	archive(CEREAL_NVP(vStr));

	for (size_t i = 0; i < size; ++i) {
		NKBase* ptr = m_pManager->SimpleCreateUI(vStr.at(i).c_str());
		ptr->Initialize(m_pManager);
		LoadSwitch(ptr, archive, i);
		m_pManager->RegistUI(vStr.at(i).c_str(), ptr);
		vPrefab.push_back(ptr);
	}

	for (auto it = vPrefab.begin(); it != vPrefab.end(); ++it) {

		if (it != vPrefab.begin()) {
			m_pManager->LoadNode(*it);
		}
		else {
			m_pManager->LoadNode(*it, true);
		}
	}

	for (auto it = vPrefab.begin(); it != vPrefab.end(); ++it) {
		NKBase* pBase = *it;
		m_pManager->ResetPrimaryID(pBase);
		m_mapModuleID.insert(std::make_pair(pBase->GetPrimaryID(), pBase));
	}

	{
		NKBase* pBase  = nullptr;
		pBase = vPrefab.at(0);

		if (parent == nullptr) {

			if (pBase->GetType() != eWINDOW) {
				
				NKWindow* pWin = new NKWindow(m_pManager->GetContext(), m_pManager);
				m_pManager->Add(pWin);
				pBase->ResetWindowID(pWin);
				pWin->RegistChild(pBase);
			}
		}
		else {
			pBase->ResetWindowID(parent);
			if (pBase->GetType() != eWINDOW) {
				parent->RegistChild(pBase);
			}
		}

		auto list = pBase->GetChildList();
		for (auto child = list->begin(); child != list->end(); ++child) {
			NKBase* pChild = *child;
			pChild->ResetParentID(pBase);
		}
	}
}

bool NKCereal::Contains(const std::vector<std::string>& vec, const std::string& str)
{
	return std::find(vec.begin(), vec.end(), str) != vec.end();
}
