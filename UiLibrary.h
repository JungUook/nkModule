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

//object
CEREAL_REGISTER_TYPE(NKBase);
CEREAL_REGISTER_TYPE(NKWindow);
CEREAL_REGISTER_TYPE(NKSpace);
CEREAL_REGISTER_TYPE(NKGroup);
CEREAL_REGISTER_TYPE(NKPopup);
CEREAL_REGISTER_TYPE(NKCombo);
CEREAL_REGISTER_TYPE(NKButton);
CEREAL_REGISTER_TYPE(NKEdit);
CEREAL_REGISTER_TYPE(NKImage);
CEREAL_REGISTER_TYPE(NKLabel);
CEREAL_REGISTER_TYPE(NKComboItem);
CEREAL_REGISTER_TYPE(NKCheckbox);
CEREAL_REGISTER_TYPE(NKSlider);
CEREAL_REGISTER_TYPE(NKProgress);
CEREAL_REGISTER_TYPE(NKSelectable);
CEREAL_REGISTER_TYPE(NKTree);
CEREAL_REGISTER_TYPE(NKChart);
CEREAL_REGISTER_TYPE(NKTooltip);
CEREAL_REGISTER_TYPE(NKMenu);
CEREAL_REGISTER_TYPE(NKScrollbar);
CEREAL_REGISTER_TYPE(NKColorPicker);
CEREAL_REGISTER_TYPE(NKSuperStyleObject);

//module
CEREAL_REGISTER_TYPE(NKBaseLabel);
CEREAL_REGISTER_TYPE(NKBaseStyle);
CEREAL_REGISTER_TYPE(NKBaseWindow);
CEREAL_REGISTER_TYPE(NKHandler);
CEREAL_REGISTER_TYPE(NKObjectFinder);
CEREAL_REGISTER_TYPE(NKProperty);
CEREAL_REGISTER_TYPE(NKTransform);

//style
CEREAL_REGISTER_TYPE(NKStyleButton);
CEREAL_REGISTER_TYPE(NKStyleChart);
CEREAL_REGISTER_TYPE(NKStyleCheckbox);
CEREAL_REGISTER_TYPE(NKStyleCombo);
CEREAL_REGISTER_TYPE(NKStyleContextualButton);
CEREAL_REGISTER_TYPE(NKStyleEdit);
CEREAL_REGISTER_TYPE(NKStyleHeader);
CEREAL_REGISTER_TYPE(NKStyleMenuButton);
CEREAL_REGISTER_TYPE(NKStyleOption);
CEREAL_REGISTER_TYPE(NKStyleProgress);
CEREAL_REGISTER_TYPE(NKStyleProperty);
CEREAL_REGISTER_TYPE(NKStyleScrollbarH);
CEREAL_REGISTER_TYPE(NKStyleScrollbarV);
CEREAL_REGISTER_TYPE(NKStyleSelectedable);
CEREAL_REGISTER_TYPE(NKStyleSlider);
CEREAL_REGISTER_TYPE(NKStyleTab);
CEREAL_REGISTER_TYPE(NKStyleText);
CEREAL_REGISTER_TYPE(NKStyleWindow);

//component

CEREAL_REGISTER_TYPE(ComponentButton);
CEREAL_REGISTER_TYPE(ComponentCombo);
CEREAL_REGISTER_TYPE(ComponentEdit);
CEREAL_REGISTER_TYPE(ComponentHeader);
CEREAL_REGISTER_TYPE(ComponentProgress);
CEREAL_REGISTER_TYPE(ComponentProperty);
CEREAL_REGISTER_TYPE(ComponentScrollbar);
CEREAL_REGISTER_TYPE(ComponentSelectable);
CEREAL_REGISTER_TYPE(ComponentSlider);
CEREAL_REGISTER_TYPE(ComponentTab);
CEREAL_REGISTER_TYPE(ComponentToggle);
CEREAL_REGISTER_TYPE(NKStyleItem);

CEREAL_REGISTER_POLYMORPHIC_RELATION(NKBaseStyle, NKProperty);
CEREAL_REGISTER_POLYMORPHIC_RELATION(NKTransform, NKProperty);

CEREAL_REGISTER_POLYMORPHIC_RELATION(NKProperty, NKBase);

CEREAL_REGISTER_POLYMORPHIC_RELATION(NKBase, NKWindow);
CEREAL_REGISTER_POLYMORPHIC_RELATION(NKBaseWindow, NKWindow);
CEREAL_REGISTER_POLYMORPHIC_RELATION(NKStyleHeader, NKWindow);
CEREAL_REGISTER_POLYMORPHIC_RELATION(NKStyleWindow, NKWindow);


CEREAL_REGISTER_POLYMORPHIC_RELATION(NKBase, NKSpace);


CEREAL_REGISTER_POLYMORPHIC_RELATION(NKBase, NKGroup);
CEREAL_REGISTER_POLYMORPHIC_RELATION(NKBaseWindow, NKGroup);
CEREAL_REGISTER_POLYMORPHIC_RELATION(NKStyleHeader, NKGroup);
CEREAL_REGISTER_POLYMORPHIC_RELATION(NKStyleWindow, NKGroup);


CEREAL_REGISTER_POLYMORPHIC_RELATION(NKBase, NKPopup);
CEREAL_REGISTER_POLYMORPHIC_RELATION(NKBaseWindow, NKPopup);
CEREAL_REGISTER_POLYMORPHIC_RELATION(NKStyleHeader, NKPopup);
CEREAL_REGISTER_POLYMORPHIC_RELATION(NKStyleWindow, NKPopup);


CEREAL_REGISTER_POLYMORPHIC_RELATION(NKBase, NKCombo);
CEREAL_REGISTER_POLYMORPHIC_RELATION(NKStyleCombo, NKCombo);


CEREAL_REGISTER_POLYMORPHIC_RELATION(NKBase, NKButton);
CEREAL_REGISTER_POLYMORPHIC_RELATION(NKHandler, NKButton);
CEREAL_REGISTER_POLYMORPHIC_RELATION(NKBaseLabel, NKButton);
CEREAL_REGISTER_POLYMORPHIC_RELATION(NKStyleButton, NKButton);


CEREAL_REGISTER_POLYMORPHIC_RELATION(NKBase, NKEdit);
CEREAL_REGISTER_POLYMORPHIC_RELATION(NKHandler, NKEdit);
CEREAL_REGISTER_POLYMORPHIC_RELATION(NKStyleEdit, NKEdit);


CEREAL_REGISTER_POLYMORPHIC_RELATION(NKBase, NKImage);


CEREAL_REGISTER_POLYMORPHIC_RELATION(NKBase, NKLabel);
CEREAL_REGISTER_POLYMORPHIC_RELATION(NKBaseLabel, NKLabel);
CEREAL_REGISTER_POLYMORPHIC_RELATION(NKStyleText, NKLabel);


CEREAL_REGISTER_POLYMORPHIC_RELATION(NKBase, NKComboItem);
CEREAL_REGISTER_POLYMORPHIC_RELATION(NKHandler, NKComboItem);
CEREAL_REGISTER_POLYMORPHIC_RELATION(NKBaseLabel, NKComboItem);


CEREAL_REGISTER_POLYMORPHIC_RELATION(NKBase, NKCheckbox);
CEREAL_REGISTER_POLYMORPHIC_RELATION(NKBaseLabel, NKCheckbox);
CEREAL_REGISTER_POLYMORPHIC_RELATION(NKStyleCheckbox, NKCheckbox);


CEREAL_REGISTER_POLYMORPHIC_RELATION(NKBase, NKSlider);
CEREAL_REGISTER_POLYMORPHIC_RELATION(NKStyleSlider, NKSlider);


CEREAL_REGISTER_POLYMORPHIC_RELATION(NKBase, NKProgress);
CEREAL_REGISTER_POLYMORPHIC_RELATION(NKStyleProgress, NKProgress);


CEREAL_REGISTER_POLYMORPHIC_RELATION(NKBase, NKSelectable);
CEREAL_REGISTER_POLYMORPHIC_RELATION(NKHandler, NKSelectable);
CEREAL_REGISTER_POLYMORPHIC_RELATION(NKBaseLabel, NKSelectable);
CEREAL_REGISTER_POLYMORPHIC_RELATION(NKStyleSelectedable, NKSelectable);


CEREAL_REGISTER_POLYMORPHIC_RELATION(NKBase, NKTree);
CEREAL_REGISTER_POLYMORPHIC_RELATION(NKBaseLabel, NKTree);
CEREAL_REGISTER_POLYMORPHIC_RELATION(NKStyleTab, NKTree);


CEREAL_REGISTER_POLYMORPHIC_RELATION(NKBase, NKChart);
CEREAL_REGISTER_POLYMORPHIC_RELATION(NKStyleChart, NKChart);


CEREAL_REGISTER_POLYMORPHIC_RELATION(NKBase, NKTooltip);
CEREAL_REGISTER_POLYMORPHIC_RELATION(NKBaseLabel, NKTooltip);
CEREAL_REGISTER_POLYMORPHIC_RELATION(NKObjectFinder, NKTooltip);
CEREAL_REGISTER_POLYMORPHIC_RELATION(NKStyleWindow, NKTooltip);
CEREAL_REGISTER_POLYMORPHIC_RELATION(NKStyleText, NKTooltip);


CEREAL_REGISTER_POLYMORPHIC_RELATION(NKBase, NKMenu);
CEREAL_REGISTER_POLYMORPHIC_RELATION(NKHandler, NKMenu);
CEREAL_REGISTER_POLYMORPHIC_RELATION(NKBaseLabel, NKMenu);
CEREAL_REGISTER_POLYMORPHIC_RELATION(NKStyleMenuButton, NKMenu);


CEREAL_REGISTER_POLYMORPHIC_RELATION(NKBase, NKScrollbar);
CEREAL_REGISTER_POLYMORPHIC_RELATION(NKStyleScrollbarH, NKScrollbar);
CEREAL_REGISTER_POLYMORPHIC_RELATION(NKStyleScrollbarV, NKScrollbar);


CEREAL_REGISTER_POLYMORPHIC_RELATION(NKBase, NKColorPicker);


CEREAL_REGISTER_POLYMORPHIC_RELATION(NKBase, NKSuperStyleObject);
CEREAL_REGISTER_POLYMORPHIC_RELATION(NKStyleButton, NKSuperStyleObject);
CEREAL_REGISTER_POLYMORPHIC_RELATION(NKStyleChart, NKSuperStyleObject);
CEREAL_REGISTER_POLYMORPHIC_RELATION(NKStyleCheckbox, NKSuperStyleObject);
CEREAL_REGISTER_POLYMORPHIC_RELATION(NKStyleCombo, NKSuperStyleObject);
CEREAL_REGISTER_POLYMORPHIC_RELATION(NKStyleContextualButton, NKSuperStyleObject);
CEREAL_REGISTER_POLYMORPHIC_RELATION(NKStyleEdit, NKSuperStyleObject);
CEREAL_REGISTER_POLYMORPHIC_RELATION(NKStyleHeader, NKSuperStyleObject);
CEREAL_REGISTER_POLYMORPHIC_RELATION(NKStyleMenuButton, NKSuperStyleObject);
CEREAL_REGISTER_POLYMORPHIC_RELATION(NKStyleOption, NKSuperStyleObject);
CEREAL_REGISTER_POLYMORPHIC_RELATION(NKStyleProgress, NKSuperStyleObject);
CEREAL_REGISTER_POLYMORPHIC_RELATION(NKStyleProperty, NKSuperStyleObject);
CEREAL_REGISTER_POLYMORPHIC_RELATION(NKStyleScrollbarH, NKSuperStyleObject);
CEREAL_REGISTER_POLYMORPHIC_RELATION(NKStyleScrollbarV, NKSuperStyleObject);
CEREAL_REGISTER_POLYMORPHIC_RELATION(NKStyleSelectedable, NKSuperStyleObject);
CEREAL_REGISTER_POLYMORPHIC_RELATION(NKStyleSlider, NKSuperStyleObject);
CEREAL_REGISTER_POLYMORPHIC_RELATION(NKStyleTab, NKSuperStyleObject);
CEREAL_REGISTER_POLYMORPHIC_RELATION(NKStyleText, NKSuperStyleObject);
CEREAL_REGISTER_POLYMORPHIC_RELATION(NKStyleWindow, NKSuperStyleObject);


static void SaveSwitch(NKBase* ptr, cereal::JSONOutputArchive& archive) {
	eTypeUI eType = ptr->GetType();
	switch (eType)
	{
	case eWINDOW: {
		NKWindow* nkWindow = static_cast<NKWindow*>(ptr);
		NKWindow& nWindow = *nkWindow; 
		archive(nWindow);
		break;
	}
	case eSPACE: {
		NKSpace* nkSpace = static_cast<NKSpace*>(ptr);
		NKSpace& nSpace = *nkSpace; 
		archive(nSpace);
		break;
	}
	case eGROUP: {
		NKGroup* nkGroup = static_cast<NKGroup*>(ptr);
		NKGroup& nGroup = *nkGroup; 
		archive(nGroup);
		break;
	}
	case ePOPUP: {
		NKPopup* nkPopup = static_cast<NKPopup*>(ptr);
		NKPopup& nPopup = *nkPopup; 
		archive(nPopup);
		break;
	}
	case eCOMBO: {
		NKCombo* nkCombo = static_cast<NKCombo*>(ptr);
		NKCombo& nCombo = *nkCombo; 
		archive(nCombo);
		break;
	}
	case eBUTTON: {
		NKButton* nkButton = static_cast<NKButton*>(ptr);
		NKButton& nButton = *nkButton; 
		archive(nButton);
		break;
	}
	case eEDIT: {
		NKEdit* nkEdit = static_cast<NKEdit*>(ptr);
		NKEdit& nEdit = *nkEdit; 
		archive(nEdit);
		break;
	}
	case eIMAGE: {
		NKImage* nkImage = static_cast<NKImage*>(ptr);
		NKImage& nImage = *nkImage; 
		archive(nImage);
		break;
	}
	case eLABEL: {
		NKLabel* nkLabel = static_cast<NKLabel*>(ptr);
		NKLabel& nLabel = *nkLabel; 
		archive(nLabel);
		break;
	}
	case eCOMBO_ITEM: {
		NKComboItem* nkComboItem = static_cast<NKComboItem*>(ptr);
		NKComboItem& nComboItem = *nkComboItem; 
		archive(nComboItem);
		break;
	}
	case eCHECKBOX: {
		NKCheckbox* nkCheckbox = static_cast<NKCheckbox*>(ptr);
		NKCheckbox& nCheckbox = *nkCheckbox; 
		archive(nCheckbox);
		break;
	}
	case eSLIDER: {
		NKSlider* nkSlider = static_cast<NKSlider*>(ptr);
		NKSlider& nSlider = *nkSlider; 
		archive(nSlider);
		break;
	}
	case ePROGRESS: {
		NKProgress* nkProgress = static_cast<NKProgress*>(ptr);
		NKProgress& nProgress = *nkProgress; 
		archive(nProgress);
		break;
	}
	case eSELECTABLE: {
		NKSelectable* nkSelectable = static_cast<NKSelectable*>(ptr);
		NKSelectable& nSelectable = *nkSelectable; 
		archive(nSelectable);
		break;
	}
	case eTREE: {
		NKTree* nkTree = static_cast<NKTree*>(ptr);
		NKTree& nTree = *nkTree; 
		archive(nTree);
		break;
	}
	case eCHART: {
		NKChart* nkChart = static_cast<NKChart*>(ptr);
		NKChart& nChart = *nkChart; 
		archive(nChart);
		break;
	}
	case eCOLOR_PICKER: {
		NKTooltip* nkTooltip = static_cast<NKTooltip*>(ptr);
		NKTooltip& nTooltip = *nkTooltip; 
		archive(nTooltip);
		break;
	}
	case eTOOLTIP: {
		NKMenu* nkMenu = static_cast<NKMenu*>(ptr);
		NKMenu& nMenu = *nkMenu; 
		archive(nMenu);
		break;
	}
	case eMENU: {
		NKScrollbar* nkScrollbar = static_cast<NKScrollbar*>(ptr);
		NKScrollbar& nScrollbar = *nkScrollbar; 
		archive(nScrollbar);
		break;
	}
	case eSCROLLBAR: {
		NKColorPicker* nkColorPicker = static_cast<NKColorPicker*>(ptr);
		NKColorPicker& nColorPicker = *nkColorPicker; 
		archive(nColorPicker);
		break;
	}
	case eSUPERSTYLE: {
		NKSuperStyleObject* nkSuperStyleObject = static_cast<NKSuperStyleObject*>(ptr);
		NKSuperStyleObject& nSuperStyleObject = *nkSuperStyleObject; 
		archive(nSuperStyleObject);
		break;
	}
	default:
		break;
	}
}

static void LoadSwitch(NKBase* ptr, cereal::JSONInputArchive& archive, size_t i) {
	eTypeUI type = ptr->GetType();

	switch (type)
	{
	case eWINDOW: {
		NKWindow* nkWindow = static_cast<NKWindow*>(ptr);
		NKWindow& cWindow = *nkWindow;
		archive(cWindow);
		break;
	}
	case eSPACE: {
		NKSpace* nkSpace = static_cast<NKSpace*>(ptr);
		NKSpace& cSpace = *nkSpace;
		archive(cSpace);
		break;
	}
	case eGROUP: {
		NKGroup* nkGroup = static_cast<NKGroup*>(ptr);
		NKGroup& cGroup = *nkGroup;
		archive(cGroup);
		break;
	}
	case ePOPUP: {
		NKPopup* nkPopup = static_cast<NKPopup*>(ptr);
		NKPopup& cPopup = *nkPopup;
		archive(cPopup);
		break;
	}
	case eCOMBO: {
		NKCombo* nkCombo = static_cast<NKCombo*>(ptr);
		NKCombo& cCombo = *nkCombo;
		archive(cCombo);
		break;
	}
	case eBUTTON: {
		NKButton* nkButton = static_cast<NKButton*>(ptr);
		NKButton& cButton = *nkButton;
		archive(cButton);
		break;
	}
	case eEDIT: {
		NKEdit* nkEdit = static_cast<NKEdit*>(ptr);
		NKEdit& cEdit = *nkEdit;
		archive(cEdit);
		break;
	}
	case eIMAGE: {
		NKImage* nkImage = static_cast<NKImage*>(ptr);
		NKImage& cImage = *nkImage;
		archive(cImage);
		break;
	}
	case eLABEL: {
		NKLabel* nkLabel = static_cast<NKLabel*>(ptr);
		NKLabel& cLabel = *nkLabel;
		archive(cLabel);
		break;
	}
	case eCOMBO_ITEM: {
		NKComboItem* nkComboItem = static_cast<NKComboItem*>(ptr);
		NKComboItem& cComboItem = *nkComboItem;
		archive(cComboItem);
		break;
	}
	case eCHECKBOX: {
		NKCheckbox* nkCheckbox = static_cast<NKCheckbox*>(ptr);
		NKCheckbox& cCheckbox = *nkCheckbox;
		archive(cCheckbox);
		break;
	}
	case eSLIDER: {
		NKSlider* nkSlider = static_cast<NKSlider*>(ptr);
		NKSlider& cSlider = *nkSlider;
		archive(cSlider);
		break;
	}
	case ePROGRESS: {
		NKProgress* nkProgress = static_cast<NKProgress*>(ptr);
		NKProgress& cProgress = *nkProgress;
		archive(cProgress);
		break;
	}
	case eSELECTABLE: {
		NKSelectable* nkSelectable = static_cast<NKSelectable*>(ptr);
		NKSelectable& cSelectable = *nkSelectable;
		archive(cSelectable);
		break;
	}
	case eTREE: {
		NKTree* nkTree = static_cast<NKTree*>(ptr);
		NKTree& cTree = *nkTree;
		archive(cTree);
		break;
	}
	case eCHART: {
		NKChart* nkChart = static_cast<NKChart*>(ptr);
		NKChart& cChart = *nkChart;
		archive(cChart);
		break;
	}
	case eCOLOR_PICKER: {
		NKTooltip* nkTooltip = static_cast<NKTooltip*>(ptr);
		NKTooltip& cTooltip = *nkTooltip;
		archive(cTooltip);
		break;
	}
	case eTOOLTIP: {
		NKMenu* nkMenu = static_cast<NKMenu*>(ptr);
		NKMenu& cMenu = *nkMenu;
		archive(cMenu);
		break;
	}
	case eMENU: {
		NKScrollbar* nkScrollbar = static_cast<NKScrollbar*>(ptr);
		NKScrollbar& cScrollbar = *nkScrollbar;
		archive(cScrollbar);
		break;
	}
	case eSCROLLBAR: {
		NKColorPicker* nkColorPicker = static_cast<NKColorPicker*>(ptr);
		NKColorPicker& cColorPicker = *nkColorPicker;
		archive(cColorPicker);
		break;
	}
	case eSUPERSTYLE: {
		NKSuperStyleObject* nkSuperStyleObject = static_cast<NKSuperStyleObject*>(ptr);
		NKSuperStyleObject& cSuperStyleObject = *nkSuperStyleObject;
		archive(cSuperStyleObject);
		break;
	}
	default:
		break;
	}
}
static NKBase* CopyObject(NKBase* pBase) {
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
		NKTooltip* ptr = static_cast<NKTooltip*>(pBase);
		NKTooltip* nkTooltip = new NKTooltip(*ptr);
		return nkTooltip;
	}
	case eTOOLTIP: {
		NKMenu* ptr = static_cast<NKMenu*>(pBase);
		NKMenu* nkMenu = new NKMenu(*ptr);
		return nkMenu;
	}
	case eMENU: {
		NKScrollbar* ptr = static_cast<NKScrollbar*>(pBase);
		NKScrollbar* nkScrollbar = new NKScrollbar(*ptr);
		return nkScrollbar;
	}
	case eSCROLLBAR: {
		NKColorPicker* ptr = static_cast<NKColorPicker*>(pBase);
		NKColorPicker* nkColorPicker = new NKColorPicker(*ptr);
		return nkColorPicker;
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