#include "pch.h"
#include "NKCereal.h"
#include "NuklearUI.h"

#include <cereal/types/vector.hpp>
#include <cereal/types/array.hpp>
#include <cereal/types/string.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/types/polymorphic.hpp>
#include "UiLibrary.h"

void SaveSwitch(NKBase* ptr, cereal::BinaryOutputArchive& archive);
void LoadSwitch(NKBase* ptr, cereal::BinaryInputArchive& archive, size_t i);

void NKCereal::SaveFileBinary(const std::string& filename)
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

	std::ofstream os(filename, std::ios::binary);
	cereal::BinaryOutputArchive archive(os);


	archive(CEREAL_NVP(vSprData));
	archive(CEREAL_NVP(m_vecPrefab));
	archive(CEREAL_NVP(m_vecLuaCode));

	archive(CEREAL_NVP(size));
	archive(CEREAL_NVP(vStr));


	for (size_t i = 0; i < size; ++i) {
		NKBase* ptr = m_vecModule.at(i);
		SaveSwitch(ptr, archive);
	}
}

void NKCereal::LoadFileBinary(std::map<std::string, CustomData>& vVar, std::map<std::string, CustomData>& vFunc, const std::string& filename)
{
	std::vector<std::string> vSprData;
	size_t size;
	std::vector<std::string> vStr;

	std::ifstream is(filename, std::ios::binary);
	cereal::BinaryInputArchive archive(is);

	archive(CEREAL_NVP(vSprData));

	wchar_t originalDir[MAX_PATH] = { 0, };
	GetCurrentDirectoryW(MAX_PATH, originalDir);
	for (size_t i = 0; i < vSprData.size(); ++i) {
		std::string str = GetExecutablePath() + "\\" + vSprData.at(i);
		m_pManager->LoadSprFile(str.c_str());
	}
	SetCurrentDirectoryW(originalDir);

	archive(CEREAL_NVP(m_vecPrefab));
	archive(CEREAL_NVP(m_vecLuaCode));

	archive(CEREAL_NVP(size));
	archive(CEREAL_NVP(vStr));

	for (auto it = m_vecLuaCode.begin(); it != m_vecLuaCode.end(); ++it) {
		std::string str = GetExecutablePath() + "\\" + *it;
		m_pManager->m_luaInterface.LoadLuaFile(str.c_str());
	}

	for (size_t i = 0; i < size; ++i) {
		NKBase* ptr = m_pManager->SimpleCreateUI(vStr.at(i).c_str());
		ptr->Initialize(m_pManager);
		LoadSwitch(ptr, archive, i);
		m_pManager->RegistUI(ptr);
	}

	for (auto it = m_vecModule.begin(); it != m_vecModule.end(); ++it) {
		m_pManager->LoadNode(*it);
	}

	for (auto it = m_vecModule.begin(); it != m_vecModule.end(); ++it) {
		m_pManager->LoadLinkNode(*it);
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


void NKCereal::SavePrefabBinary(const std::string& filename, NKBase* prefab)
{
	std::vector<NKBase*> vPrefab;
	std::vector<std::string> vStr;

	prefab->GetPrefab(vPrefab);
	size_t size = vPrefab.size();

	for (size_t i = 0; i < size; ++i) {
		std::string str = vPrefab.at(i)->getClassName();
		vStr.push_back(str);
	}

	std::ofstream os(filename + ".bin", std::ios::binary);
	cereal::BinaryOutputArchive archive(os);

	archive(CEREAL_NVP(size));
	archive(CEREAL_NVP(vStr));


	for (size_t i = 0; i < size; ++i) {
		NKBase* ptr = vPrefab.at(i);
		SaveSwitch(ptr, archive);
	}

	std::string filePath = filename + ".bin";
	std::string relativePath = GetRelativePath(filePath.c_str());

	if (!Contains(m_vecPrefab, relativePath)) {
		m_vecPrefab.push_back(relativePath);
	}
	else {
	}
}

void NKCereal::LoadPrefabBinary(const std::string& filename, NKBase* parent)
{
	size_t size;
	std::vector<NKBase*> vPrefab;
	std::map<unsigned int, NKBase*> mPrefab;
	std::vector<std::string> vStr;

	std::ifstream is(GetExecutablePath() + "\\" + filename, std::ios::binary);
	cereal::BinaryInputArchive archive(is);

	archive(CEREAL_NVP(size));
	archive(CEREAL_NVP(vStr));

	for (size_t i = 0; i < size; ++i) {
		NKBase* ptr = m_pManager->SimpleCreateUI(vStr.at(i).c_str());
		ptr->Initialize(m_pManager);
		LoadSwitch(ptr, archive, i);
		vPrefab.push_back(ptr);
		mPrefab.insert(std::make_pair(ptr->GetPrimaryID(), ptr));
	}

	for (auto it = vPrefab.begin(); it != vPrefab.end(); ++it) {
		NKBase* ptr = *it;
		unsigned int pID = ptr->GetParentPrimaryID();
		if (pID != 0) {
			auto found = mPrefab.find(pID);
			if (found != mPrefab.end()) {
				NKBase* pParent = found->second;
				auto pList = pParent->GetChildList();
				pList->push_back(ptr);
			}
		}
	}

	for (auto it = vPrefab.begin(); it != vPrefab.end(); ++it) {
		NKBase* ptr = *it;
		ptr->SetPrimaryID(reinterpret_cast<intptr_t>(ptr));
	}

	for (auto it = vPrefab.begin(); it != vPrefab.end(); ++it) {
		NKBase* pParent = *it;
		auto pList = pParent->GetChildList();

		for (auto cit = pList->begin(); cit != pList->end(); ++cit) {
			NKBase* pChild = *cit;
			pChild->ResetParentID(pParent);
		}
	}

	NKBase* pBase = nullptr;
	NKWindow* pWin = nullptr;
	pBase = vPrefab.at(0);
	if (pBase->GetType() == eWINDOW) {
		pBase->ResetWindowID(pBase);
	}
	else {
		pWin = new NKWindow(m_pManager->GetContext(), m_pManager);
		m_pManager->Add(pWin);
		pBase->ResetWindowID(pWin);
	}

	for (auto it = vPrefab.begin(); it != vPrefab.end(); ++it) {
		NKBase* ptr = *it;
		auto pList = ptr->GetChildList();
		pList->clear();
	}


	for (auto it = vPrefab.begin(); it != vPrefab.end(); ++it) {
		NKBase* ptr = *it;
		m_pManager->RegistUI(ptr);
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
		m_pManager->LoadLinkNode(*it);
	}

	for (auto it = vPrefab.begin(); it != vPrefab.end(); ++it) {
		NKBase* pBase = *it;
		m_pManager->ResetPrimaryID(pBase);
		m_mapModuleID.insert(std::make_pair(pBase->GetPrimaryID(), pBase));
	}

	if (parent == nullptr) {
		if (pBase->GetType() != eWINDOW) {
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

void SaveSwitch(NKBase* ptr, cereal::BinaryOutputArchive& archive) {
	eTypeUI eType = ptr->GetType();
	switch (eType)
	{
	case eWINDOW: {
		NKWindow* nkWindow = static_cast<NKWindow*>(ptr);
		NKWindow& cWindow = *nkWindow;
		archive(CEREAL_NVP(cWindow));
		break;
	}
	case eSPACE: {
		NKSpace* nkSpace = static_cast<NKSpace*>(ptr);
		NKSpace& cSpace = *nkSpace;
		archive(CEREAL_NVP(cSpace));
		break;
	}
	case eGROUP: {
		NKGroup* nkGroup = static_cast<NKGroup*>(ptr);
		NKGroup& cGroup = *nkGroup;
		archive(CEREAL_NVP(cGroup));
		break;
	}
	case ePOPUP: {
		NKPopup* nkPopup = static_cast<NKPopup*>(ptr);
		NKPopup& cPopup = *nkPopup;
		archive(CEREAL_NVP(cPopup));
		break;
	}
	case eCOMBO: {
		NKCombo* nkCombo = static_cast<NKCombo*>(ptr);
		NKCombo& cCombo = *nkCombo;
		archive(CEREAL_NVP(cCombo));
		break;
	}
	case eBUTTON: {
		NKButton* nkButton = static_cast<NKButton*>(ptr);
		NKButton& cButton = *nkButton;
		archive(CEREAL_NVP(cButton));
		break;
	}
	case eEDIT: {
		NKEdit* nkEdit = static_cast<NKEdit*>(ptr);
		NKEdit& cEdit = *nkEdit;
		archive(CEREAL_NVP(cEdit));
		break;
	}
	case eIMAGE: {
		NKImage* nkImage = static_cast<NKImage*>(ptr);
		NKImage& cImage = *nkImage;
		archive(CEREAL_NVP(cImage));
		break;
	}
	case eLABEL: {
		NKLabel* nkLabel = static_cast<NKLabel*>(ptr);
		NKLabel& cLabel = *nkLabel;
		archive(CEREAL_NVP(cLabel));
		break;
	}
	case eCOMBO_ITEM: {
		NKComboItem* nkComboItem = static_cast<NKComboItem*>(ptr);
		NKComboItem& cComboItem = *nkComboItem;
		archive(CEREAL_NVP(cComboItem));
		break;
	}
	case eCHECKBOX: {
		NKCheckbox* nkCheckbox = static_cast<NKCheckbox*>(ptr);
		NKCheckbox& cCheckbox = *nkCheckbox;
		archive(CEREAL_NVP(cCheckbox));
		break;
	}
	case eSLIDER: {
		NKSlider* nkSlider = static_cast<NKSlider*>(ptr);
		NKSlider& cSlider = *nkSlider;
		archive(CEREAL_NVP(cSlider));
		break;
	}
	case ePROGRESS: {
		NKProgress* nkProgress = static_cast<NKProgress*>(ptr);
		NKProgress& cProgress = *nkProgress;
		archive(CEREAL_NVP(cProgress));
		break;
	}
	case eSELECTABLE: {
		NKSelectable* nkSelectable = static_cast<NKSelectable*>(ptr);
		NKSelectable& cSelectable = *nkSelectable;
		archive(CEREAL_NVP(cSelectable));
		break;
	}
	case eTREE: {
		NKTree* nkTree = static_cast<NKTree*>(ptr);
		NKTree& cTree = *nkTree;
		archive(CEREAL_NVP(cTree));
		break;
	}
	case eCHART: {
		NKChart* nkChart = static_cast<NKChart*>(ptr);
		NKChart& cChart = *nkChart;
		archive(CEREAL_NVP(cChart));
		break;
	}
	case eCOLOR_PICKER: {
		NKColorPicker* nkColorPicker = static_cast<NKColorPicker*>(ptr);
		NKColorPicker& cColorPicker = *nkColorPicker;
		archive(CEREAL_NVP(cColorPicker));
		break;
	}
	case eTOOLTIP: {
		NKTooltip* nkTooltip = static_cast<NKTooltip*>(ptr);
		NKTooltip& cTooltip = *nkTooltip;
		archive(CEREAL_NVP(cTooltip));
		break;
	}
	case eMENU: {
		NKMenu* nkMenu = static_cast<NKMenu*>(ptr);
		NKMenu& cMenu = *nkMenu;
		archive(CEREAL_NVP(cMenu));
		break;
	}
	case eSCROLLBAR: {
		NKScrollbar* nkScrollbar = static_cast<NKScrollbar*>(ptr);
		NKScrollbar& cScrollbar = *nkScrollbar;
		archive(CEREAL_NVP(cScrollbar));
		break;
	}
	case eSUPERSTYLE: {
		NKSuperStyleObject* nkSuperStyleObject = static_cast<NKSuperStyleObject*>(ptr);
		NKSuperStyleObject& cSuperStyleObject = *nkSuperStyleObject;
		archive(CEREAL_NVP(cSuperStyleObject));
		break;
	}
	default:
		break;
	}
}
void LoadSwitch(NKBase* ptr, cereal::BinaryInputArchive& archive, size_t i) {
	eTypeUI type = ptr->GetType();

	switch (type)
	{
	case eWINDOW: {
		NKWindow* nkWindow = static_cast<NKWindow*>(ptr);
		NKWindow& cWindow = *nkWindow;
		archive(CEREAL_NVP(cWindow));
		break;
	}
	case eSPACE: {
		NKSpace* nkSpace = static_cast<NKSpace*>(ptr);
		NKSpace& cSpace = *nkSpace;
		archive(CEREAL_NVP(cSpace));
		break;
	}
	case eGROUP: {
		NKGroup* nkGroup = static_cast<NKGroup*>(ptr);
		NKGroup& cGroup = *nkGroup;
		archive(CEREAL_NVP(cGroup));
		break;
	}
	case ePOPUP: {
		NKPopup* nkPopup = static_cast<NKPopup*>(ptr);
		NKPopup& cPopup = *nkPopup;
		archive(CEREAL_NVP(cPopup));
		break;
	}
	case eCOMBO: {
		NKCombo* nkCombo = static_cast<NKCombo*>(ptr);
		NKCombo& cCombo = *nkCombo;
		archive(CEREAL_NVP(cCombo));
		break;
	}
	case eBUTTON: {
		NKButton* nkButton = static_cast<NKButton*>(ptr);
		NKButton& cButton = *nkButton;
		archive(CEREAL_NVP(cButton));
		break;
	}
	case eEDIT: {
		NKEdit* nkEdit = static_cast<NKEdit*>(ptr);
		NKEdit& cEdit = *nkEdit;
		archive(CEREAL_NVP(cEdit));
		break;
	}
	case eIMAGE: {
		NKImage* nkImage = static_cast<NKImage*>(ptr);
		NKImage& cImage = *nkImage;
		archive(CEREAL_NVP(cImage));
		break;
	}
	case eLABEL: {
		NKLabel* nkLabel = static_cast<NKLabel*>(ptr);
		NKLabel& cLabel = *nkLabel;
		archive(CEREAL_NVP(cLabel));
		break;
	}
	case eCOMBO_ITEM: {
		NKComboItem* nkComboItem = static_cast<NKComboItem*>(ptr);
		NKComboItem& cComboItem = *nkComboItem;
		archive(CEREAL_NVP(cComboItem));
		break;
	}
	case eCHECKBOX: {
		NKCheckbox* nkCheckbox = static_cast<NKCheckbox*>(ptr);
		NKCheckbox& cCheckbox = *nkCheckbox;
		archive(CEREAL_NVP(cCheckbox));
		break;
	}
	case eSLIDER: {
		NKSlider* nkSlider = static_cast<NKSlider*>(ptr);
		NKSlider& cSlider = *nkSlider;
		archive(CEREAL_NVP(cSlider));
		break;
	}
	case ePROGRESS: {
		NKProgress* nkProgress = static_cast<NKProgress*>(ptr);
		NKProgress& cProgress = *nkProgress;
		archive(CEREAL_NVP(cProgress));
		break;
	}
	case eSELECTABLE: {
		NKSelectable* nkSelectable = static_cast<NKSelectable*>(ptr);
		NKSelectable& cSelectable = *nkSelectable;
		archive(CEREAL_NVP(cSelectable));
		break;
	}
	case eTREE: {
		NKTree* nkTree = static_cast<NKTree*>(ptr);
		NKTree& cTree = *nkTree;
		archive(CEREAL_NVP(cTree));
		break;
	}
	case eCHART: {
		NKChart* nkChart = static_cast<NKChart*>(ptr);
		NKChart& cChart = *nkChart;
		archive(CEREAL_NVP(cChart));
		break;
	}
	case eCOLOR_PICKER: {
		NKColorPicker* nkColorPicker = static_cast<NKColorPicker*>(ptr);
		NKColorPicker& cColorPicker = *nkColorPicker;
		archive(CEREAL_NVP(cColorPicker));
		break;
	}
	case eTOOLTIP: {
		NKTooltip* nkTooltip = static_cast<NKTooltip*>(ptr);
		NKTooltip& cTooltip = *nkTooltip;
		archive(CEREAL_NVP(cTooltip));
		break;
	}
	case eMENU: {
		NKMenu* nkMenu = static_cast<NKMenu*>(ptr);
		NKMenu& cMenu = *nkMenu;
		archive(CEREAL_NVP(cMenu));
		break;
	}
	case eSCROLLBAR: {
		NKScrollbar* nkScrollbar = static_cast<NKScrollbar*>(ptr);
		NKScrollbar& cScrollbar = *nkScrollbar;
		archive(CEREAL_NVP(cScrollbar));
		break;
	}
	case eSUPERSTYLE: {
		NKSuperStyleObject* nkSuperStyleObject = static_cast<NKSuperStyleObject*>(ptr);
		NKSuperStyleObject& cSuperStyleObject = *nkSuperStyleObject;
		archive(CEREAL_NVP(cSuperStyleObject));
		break;
	}
	default:
		break;
	}
}