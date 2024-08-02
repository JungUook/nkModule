#pragma once

#ifndef UiLibrary_h__
#define UiLibrary_h__

#include "NKWindow.h"
#include "NKSpace.h"
#include "NKGroup.h"
#include "NKPopup.h"
#include "NKCombo.h"
#include "NKButton.h"
#include "NKEdit.h"
#include "NKImage.h"
#include "NKLabel.h"
#include "NKComboItem.h"
#include "NKCheckbox.h"
#include "NKSlider.h"
#include "NKProgress.h"
#include "NKSelectable.h"
#include "NKTree.h"
#include "NKChart.h"
#include "NKTooltip.h"
#include "NKMenu.h"
#include "NKScrollbar.h"
#include "NKColorPicker.h"
#include "NKSuperStyleObject.h"

#define EditorVersion 10

static NKBase* CopyObject(NKBase* pBase)
{
	eTypeUI type = pBase->GetType();

	switch (type)
	{
	case eWINDOW: {
		NKWindow* ptr = static_cast<NKWindow*>(pBase);
		NKWindow* nkWindow = new NKWindow(*ptr);
		return nkWindow;
	}
	case eSPACE: {
		NKSpace* ptr = static_cast<NKSpace*>(pBase);
		NKSpace* nkSpace = new NKSpace(*ptr);
		return nkSpace;
	}
	case eGROUP: {
		NKGroup* ptr = static_cast<NKGroup*>(pBase);
		NKGroup* nkGroup = new NKGroup(*ptr);
		return nkGroup;
	}
	case ePOPUP: {
		NKPopup* ptr = static_cast<NKPopup*>(pBase);
		NKPopup* nkPopup = new NKPopup(*ptr);
		return nkPopup;
	}
	case eCOMBO: {
		NKCombo* ptr = static_cast<NKCombo*>(pBase);
		NKCombo* nkCombo = new NKCombo(*ptr);
		return nkCombo;
	}
	case eBUTTON: {
		NKButton* ptr = static_cast<NKButton*>(pBase);
		NKButton* nkButton = new NKButton(*ptr);
		return nkButton;
	}
	case eEDIT: {
		NKEdit* ptr = static_cast<NKEdit*>(pBase);
		NKEdit* nkEdit = new NKEdit(*ptr);
		return nkEdit;
	}
	case eIMAGE: {
		NKImage* ptr = static_cast<NKImage*>(pBase);
		NKImage* nkImage = new NKImage(*ptr);
		return nkImage;
	}
	case eLABEL: {
		NKLabel* ptr = static_cast<NKLabel*>(pBase);
		NKLabel* nkLabel = new NKLabel(*ptr);
		return nkLabel;
	}
	case eCOMBO_ITEM: {
		NKComboItem* ptr = static_cast<NKComboItem*>(pBase);
		NKComboItem* nkComboItem = new NKComboItem(*ptr);
		return nkComboItem;
	}
	case eCHECKBOX: {
		NKCheckbox* ptr = static_cast<NKCheckbox*>(pBase);
		NKCheckbox* nkCheckbox = new NKCheckbox(*ptr);
		return nkCheckbox;
	}
	case eSLIDER: {
		NKSlider* ptr = static_cast<NKSlider*>(pBase);
		NKSlider* nkSlider = new NKSlider(*ptr);
		return nkSlider;
	}
	case ePROGRESS: {
		NKProgress* ptr = static_cast<NKProgress*>(pBase);
		NKProgress* nkProgress = new NKProgress(*ptr);
		return nkProgress;
	}
	case eSELECTABLE: {
		NKSelectable* ptr = static_cast<NKSelectable*>(pBase);
		NKSelectable* nkSelectable = new NKSelectable(*ptr);
		return nkSelectable;
	}
	case eTREE: {
		NKTree* ptr = static_cast<NKTree*>(pBase);
		NKTree* nkTree = new NKTree(*ptr);
		return nkTree;
	}
	case eCHART: {
		NKChart* ptr = static_cast<NKChart*>(pBase);
		NKChart* nkChart = new NKChart(*ptr);
		return nkChart;
	}
	case eCOLOR_PICKER: {
		NKColorPicker* ptr = static_cast<NKColorPicker*>(pBase);
		NKColorPicker* nkColorPicker = new NKColorPicker(*ptr);
		return nkColorPicker;
	}
	case eTOOLTIP: {
		NKTooltip* ptr = static_cast<NKTooltip*>(pBase);
		NKTooltip* nkTooltip = new NKTooltip(*ptr);
		return nkTooltip;
	}
	case eMENU: {
		NKMenu* ptr = static_cast<NKMenu*>(pBase);
		NKMenu* nkMenu = new NKMenu(*ptr);
		return nkMenu;
	}
	case eSCROLLBAR: {
		NKScrollbar* ptr = static_cast<NKScrollbar*>(pBase);
		NKScrollbar* nkScrollbar = new NKScrollbar(*ptr);
		return nkScrollbar;
	}
	case eSUPERSTYLE: {
		NKSuperStyleObject* ptr = static_cast<NKSuperStyleObject*>(pBase);
		NKSuperStyleObject* nkSuperStyleObject = new NKSuperStyleObject(*ptr);
		return nkSuperStyleObject;
	}
	default:
		return nullptr;
	}
}
static void replaceAll(std::string& str, const std::string& from, const std::string& to) {
	size_t start_pos = 0;
	while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
		str.replace(start_pos, from.length(), to);
		start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
	}
}

#endif //UiLibrary_h__