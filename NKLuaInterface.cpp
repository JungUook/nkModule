#include "pch.h"
#include "NKLuaInterface.h"
#include "UiLibrary.h"
#include "NuklearUI.h"

NKLuaInterface::NKLuaInterface()
{
	m_lua = nullptr;
	m_pManager = nullptr;
}

NKLuaInterface::~NKLuaInterface()
{
	m_lua = nullptr;
	m_pManager = nullptr;
}

void NKLuaInterface::Init()
{
	m_lua = luaL_newstate();
	luaL_openlibs(m_lua);
	RegisterBase();
}

void NKLuaInterface::Release()
{
	lua_close(m_lua);
}

void NKLuaInterface::LoadLuaFile(const char* filePath)
{
	if (luaL_dofile(m_lua, filePath) != LUA_OK) {
		std::cerr << lua_tostring(m_lua, -1) << std::endl;
	}
}

luabridge::LuaRef NKLuaInterface::GetLuaTable(const char* tableName)
{
	return luabridge::getGlobal(m_lua, tableName);
}

bool NKLuaInterface::RunFunction(const char* functionName)
{
	lua_getglobal(m_lua, functionName);
	if (lua_pcall(m_lua, 0, 0, 0) != 0) {
		fprintf(stderr, "%s function call failed: %s\n", functionName, lua_tostring(m_lua, -1));
		lua_pop(m_lua, 1);
		return false;
	}
	return true;
}

bool NKLuaInterface::RunFunctionArgs(const char* functionName, const luabridge::LuaRef& args)
{
	luabridge::LuaRef func = luabridge::getGlobal(m_lua, functionName);
	try {
		if (func.isFunction()) {
			func(args);  // ÀÎ¼ö¸¦ »ç¿ëÇÏ¿© ÇÔ¼ö È£Ãâ
		}
	}
	catch (const luabridge::LuaException& e) {
#ifdef _NKDEBUG
		std::cerr << "LuaException: " << e.what() << std::endl;
#endif // _NKDEBUG
		return false;
	}
	return true;
}

void NKLuaInterface::ResponseFunction(const char* functionName)
{
}

void NKLuaInterface::ResponseFunctionArgs(const char* functionName, const luabridge::LuaRef& args)
{
}

void NKLuaInterface::SubscribeVariable(std::string key, NKHandler* handler)
{
	auto found = m_mapVariable.find(key);
	if (found != m_mapVariable.end()) {
		CustomData& var = found->second;
		var.vUseObj.push_back(handler);
	}
	else {
		CustomData var;
		strcpy_s(var.name, key.c_str());
		var.bFunction = false;
		var.vUseObj.push_back(handler);
		m_mapVariable.insert(std::make_pair(key, var));
		//std::sort(m_mapVariable.begin(), m_mapVariable.end(), customCompare);
	}
}

void NKLuaInterface::SubscribeFunction(std::string key, NKHandler* handler)
{
	auto found = m_mapFunction.find(key);
	if (found != m_mapFunction.end()) {
		CustomData& func = found->second;
		func.vUseObj.push_back(handler);
	}
	else {
		CustomData func;
		strcpy_s(func.name, key.c_str());
		func.bFunction = true;
		func.vUseObj.push_back(handler);
		m_mapFunction.insert(std::make_pair(key, func));
		//std::sort(m_mapFunction.begin(), m_mapFunction.end(), customCompare);
	}
}

void NKLuaInterface::UnsubscribeVariable(std::string key, NKHandler* handler)
{
	auto found = m_mapVariable.find(key);
	if (found != m_mapVariable.end()) {
		CustomData& var = found->second;

		for (auto it = var.vUseObj.begin(); it != var.vUseObj.end();) {
			NKHandler* ptr = *it;

			if (ptr == handler) {
				it = var.vUseObj.erase(it);
			}
			else {
				++it;
			}
		}

		if (var.vUseObj.size() <= 0) {
			m_mapVariable.erase(key);
		}
	}
}

void NKLuaInterface::UnsubscribeFunction(std::string key, NKHandler* handler)
{
	auto found = m_mapFunction.find(key);
	if (found != m_mapFunction.end()) {
		CustomData& func = found->second;

		for (auto it = func.vUseObj.begin(); it != func.vUseObj.end();) {
			NKHandler* ptr = *it;

			if (ptr == handler) {
				it = func.vUseObj.erase(it);
			}
			else {
				++it;
			}
		}

		if (func.vUseObj.size() <= 0) {
			m_mapVariable.erase(key);
		}
	}
}

bool NKLuaInterface::IsActiveFunction(std::string functionname)
{
	luabridge::LuaRef func = luabridge::getGlobal(m_lua, functionname.c_str());
	return func.isFunction();
}

bool NKLuaInterface::IsActiveVariable(std::string variablename)
{
	luabridge::LuaRef var = luabridge::getGlobal(m_lua, variablename.c_str());
	return var.isTable() || var.isNumber() || var.isString() || var.isBool();
}

luabridge::LuaRef NKLuaInterface::GetLuaTable(std::string variablename)
{
	return luabridge::getGlobal(m_lua, variablename.c_str());
}

std::wstring NKLuaInterface::utf8ToWstring(const char* str)
{
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
	std::wstring wstrTo(size_needed - 1, 0); // -1 to exclude the null terminator
	MultiByteToWideChar(CP_UTF8, 0, str, -1, &wstrTo[0], size_needed);
	return wstrTo;
}

bool NKLuaInterface::customCompare(const CustomData aData, const CustomData bData)
{
	const wchar_t kFirstHangulConsonant = L'°¡'; // Unicode value for '°¡'
	const wchar_t kLastHangulConsonant = L'ÆR'; // Unicode value for 'ÆR'

	std::wstring a = utf8ToWstring(aData.name);
	std::wstring b = utf8ToWstring(bData.name);

	std::locale loc("ko_KR.UTF-8");

	// µÎ ¹®ÀÚ¿­ÀÌ ¿µ¾î·Î¸¸ ÀÌ·ç¾îÁø °æ¿ì ¾ËÆÄºª ¼ø¼­·Î Á¤·Ä
	if (std::isalpha(a[0], loc) && std::isalpha(b[0], loc)) {
		return a < b;
	}

	// µÎ ¹®ÀÚ¿­ÀÌ ÇÑ±Û·Î¸¸ ÀÌ·ç¾îÁø °æ¿ì ÀÚ¸ð ¼ø¼­·Î Á¤·Ä
	if (a[0] >= kFirstHangulConsonant && a[0] <= kLastHangulConsonant &&
		b[0] >= kFirstHangulConsonant && b[0] <= kLastHangulConsonant) {
		return a < b;
	}

	// ¿µ¾î¿Í ÇÑ±ÛÀÌ ¼¯¿© ÀÖ´Â °æ¿ì ¿µ¾î¸¦ ¸ÕÀú, ÇÑ±ÛÀ» ³ªÁß¿¡ Á¤·Ä
	if (std::isalpha(a[0], loc) && (b[0] >= kFirstHangulConsonant && b[0] <= kLastHangulConsonant)) {
		return true;
	}
	if ((a[0] >= kFirstHangulConsonant && a[0] <= kLastHangulConsonant) && std::isalpha(b[0], loc)) {
		return false;
	}

	// ±× ¿ÜÀÇ °æ¿ì¿¡´Â ±âº» ºñ±³
	return a < b;
}

#ifdef _NKDEBUG
void NKLuaInterface::DebugLoadLuaFile(const char* filePath)
{
	if (luaL_dofile(m_lua, filePath) != LUA_OK) {
		std::cerr << lua_tostring(m_lua, -1) << std::endl;
	}
}
#endif // _NKDEBUG

void NKLuaInterface::RegisterBase()
{
	luabridge::getGlobalNamespace(m_lua)
		.beginClass<NuklearUI>("NuklearUI")
		.addFunction("Find", &NuklearUI::Find<NKBase>)
		.addFunction("FindWindow", &NuklearUI::Find<NKWindow>)
		.addFunction("FindSpace", &NuklearUI::Find<NKSpace>)
		.addFunction("FindGroup", &NuklearUI::Find<NKGroup>)
		.addFunction("FindPopup", &NuklearUI::Find<NKPopup>)
		.addFunction("FindCombo", &NuklearUI::Find<NKCombo>)
		.addFunction("FindButton", &NuklearUI::Find<NKButton>)
		.addFunction("FindEdit", &NuklearUI::Find<NKEdit>)
		.addFunction("FindImage", &NuklearUI::Find<NKImage>)
		.addFunction("FindLabel", &NuklearUI::Find<NKLabel>)
		.addFunction("FindComboItem", &NuklearUI::Find<NKComboItem>)
		.addFunction("FindCheckbox", &NuklearUI::Find<NKCheckbox>)
		.addFunction("FindSlider", &NuklearUI::Find<NKSlider>)
		.addFunction("FindProgress", &NuklearUI::Find<NKProgress>)
		.addFunction("FindSelectable", &NuklearUI::Find<NKSelectable>)
		.addFunction("FindTree", &NuklearUI::Find<NKTree>)
		.addFunction("FindChart", &NuklearUI::Find<NKChart>)
		.addFunction("FindTooltip", &NuklearUI::Find<NKTooltip>)
		.addFunction("FindMenu", &NuklearUI::Find<NKMenu>)
		.addFunction("FindScrollbar", &NuklearUI::Find<NKScrollbar>)
		.addFunction("FindColorPicker", &NuklearUI::Find<NKColorPicker>)
		.addFunction("FindSuperStyleObject", &NuklearUI::Find<NKSuperStyleObject>)
		.endClass();

	luabridge::push(m_lua, this->m_pManager);
	lua_setglobal(m_lua, "system");

	luabridge::getGlobalNamespace(m_lua)
		.beginClass<NKLuaInterface>("NKLuaInterface")
		.addFunction("ResFunc", &NKLuaInterface::ResponseFunction)
		.addFunction("ResFuncArgs", &NKLuaInterface::ResponseFunctionArgs)
		.endClass();
	luabridge::push(m_lua, this);
	lua_setglobal(m_lua, "interface");

	luabridge::getGlobalNamespace(m_lua)
		.beginClass<NKCereal>("NKCereal")
		.addFunction("LoadPrefab", &NKCereal::LLoadPrefab)
		.endClass();
	luabridge::push(m_lua, this->m_pManager->m_cereal);
	lua_setglobal(m_lua, "io");


	luabridge::getGlobalNamespace(m_lua)
		.beginClass<NKBase>("NKBase")
		.addFunction("SetActive", &NKBase::LSetActive)
		.addFunction("AddChild", &NKBase::LAddChild)
		.addFunction("RemoveChild", &NKBase::LRemoveChild)
		.endClass()
		.deriveClass<NKWindow, NKBase>("NKWindow")
		.endClass()
		.deriveClass<NKSpace, NKBase>("NKSpace")
		.addFunction("SetLayout", &NKSpace::LSetLayout)
		.addFunction("SetCols", &NKSpace::LSetCols)
		.endClass()
		.deriveClass<NKGroup, NKBase>("NKGroup")
		.endClass()
		.deriveClass<NKPopup, NKBase>("NKPopup")
		.endClass()
		.deriveClass<NKCombo, NKBase>("NKCombo")
		.addFunction("SetComboName", &NKCombo::LSetComboName)
		.addFunction("SetLabelSize", &NKCombo::LSetLabelSize)
		.endClass()
		.deriveClass<NKComboItem, NKBase>("NKComboItem")
		//.addFunction("RegistFunction", &NKComboItem::RegistFunction)
		.endClass()
		.deriveClass<NKButton, NKBase>("NKButton")
		//.addFunction("RegistFunction", &NKButton::RegistFunction)
		.endClass()
		.deriveClass<NKEdit, NKBase>("NKEdit")
		.addFunction("Clear", &NKEdit::Clear)
		//.addFunction("RegistFunction", &NKEdit::RegistFunction)
		.endClass()
		.deriveClass<NKImage, NKBase>("NKImage")
		.addFunction("SetImagePath", &NKImage::LSetImagePath)
		.addFunction("SetIndex", &NKImage::LSetIndex)
		.endClass()
		.deriveClass<NKLabel, NKBase>("NKLabel")
		.addFunction("SetLabel", &NKLabel::LSetLabel)
		.endClass()
		.deriveClass<NKCheckbox, NKBase>("NKCheckbox")
		.addFunction("SetLabel", &NKCheckbox::LSetLabel)
		.addFunction("SetChecked", &NKCheckbox::LSetChecked)
		.addFunction("IsChecked", &NKCheckbox::IsChecked)
		.endClass()
		.deriveClass<NKSlider, NKBase>("NKSlider")
		.addFunction("SetRange", &NKSlider::LSetRange)
		.addFunction("SetValue", &NKSlider::LSetValue)
		.addFunction("GetValue", &NKSlider::GetValue)
		.endClass()
		.deriveClass<NKProgress, NKBase>("NKProgress")
		.addFunction("SetProgress", &NKProgress::LSetProgress)
		.addFunction("GetProgress", &NKProgress::GetProgress)
		.endClass()
		.deriveClass<NKSelectable, NKBase>("NKSelectable")
		.addFunction("SetLabel", &NKSelectable::LSetLabel)
		.addFunction("SetSelected", &NKSelectable::LSetSelected)
		.addFunction("IsSelected", &NKSelectable::IsSelected)
		.endClass()
		.deriveClass<NKTree, NKBase>("NKTree")
		.addFunction("SetLabel", &NKTree::LSetLabel)
		.addFunction("SetState", &NKTree::SetState)
		.addFunction("GetState", &NKTree::GetState)
		.endClass()
		.deriveClass<NKChart, NKBase>("NKChart")
		.addFunction("AddValue", &NKChart::AddValue)
		.addFunction("Clear", &NKChart::Clear)
		.endClass()
		.deriveClass<NKColorPicker, NKBase>("NKColorPicker")
		.addFunction("SetColor", &NKColorPicker::SetColor)
		.addFunction("GetColor", &NKColorPicker::GetColor)
		.endClass()
		.deriveClass<NKTooltip, NKBase>("NKTooltip")
		.endClass()
		.deriveClass<NKMenu, NKBase>("NKMenu")
		.addFunction("SetLabel", &NKMenu::LSetLabel)
		.endClass()
		.deriveClass<NKScrollbar, NKBase>("NKScrollbar")
		.addFunction("SetScroll", &NKScrollbar::SetScroll)
		.addFunction("GetScroll", &NKScrollbar::GetScroll)
		.endClass()
		.beginClass<ObjMaker>("ObjMaker")
		.addStaticFunction("createWindow", &ObjMaker::create<NKWindow>)
		.addStaticFunction("createSpace", &ObjMaker::create<NKSpace>)
		.addStaticFunction("createGroup", &ObjMaker::create<NKGroup>)
		.addStaticFunction("createPopup", &ObjMaker::create<NKPopup>)
		.addStaticFunction("createCombo", &ObjMaker::create<NKCombo>)
		.addStaticFunction("createComboItem", &ObjMaker::create<NKComboItem>)
		.addStaticFunction("createButton", &ObjMaker::create<NKButton>)
		.addStaticFunction("createEdit", &ObjMaker::create<NKEdit>)
		.addStaticFunction("createImage", &ObjMaker::create<NKImage>)
		.addStaticFunction("createLabel", &ObjMaker::create<NKLabel>)
		.addStaticFunction("createCheckbox", &ObjMaker::create<NKCheckbox>)
		.addStaticFunction("createSlider", &ObjMaker::create<NKSlider>)
		.addStaticFunction("createProgress", &ObjMaker::create<NKProgress>)
		.addStaticFunction("createSelectable", &ObjMaker::create<NKSelectable>)
		.addStaticFunction("createTree", &ObjMaker::create<NKTree>)
		.addStaticFunction("createChart", &ObjMaker::create<NKChart>)
		.addStaticFunction("createColorPicker", &ObjMaker::create<NKColorPicker>)
		.addStaticFunction("createTooltip", &ObjMaker::create<NKTooltip>)
		.addStaticFunction("createMenu", &ObjMaker::create<NKMenu>)
		.addStaticFunction("createScrollbar", &ObjMaker::create<NKScrollbar>)
		.endClass();
}
