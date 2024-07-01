#include "pch.h"
#include "NKLuaInterface.h"
#include "UiLibrary.h"
#include "NuklearUI.h"

NKLuaInterface::NKLuaInterface()
{
	memset(m_filePath, 0, sizeof(m_filePath));
	m_lua = nullptr;
}

NKLuaInterface::~NKLuaInterface()
{
	m_lua = nullptr;
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
#ifdef _NKDEBUG
	memset(m_filePath, 0, sizeof(m_filePath));
	strcpy_s(m_filePath, filePath);
	if (luaL_dofile(m_lua, m_filePath) != LUA_OK) {
		std::cerr << lua_tostring(m_lua, -1) << std::endl;
	}
#else
	if (luaL_dofile(m_lua, filePath) != LUA_OK) {
		std::cerr << lua_tostring(m_lua, -1) << std::endl;
	}
#endif // _NKDEBUG

	RunFunction("Init");
}

luabridge::LuaRef NKLuaInterface::GetLuaTable(const char* tableName)
{
	return luabridge::getGlobal(m_lua, tableName);
}

bool NKLuaInterface::RunFunction(const char* functionName)
{
	lua_getglobal(m_lua, functionName);
	if (lua_pcall(m_lua, 0, 0, 0) != 0) {
		fprintf(stderr, "%s ÇÔ¼ö È£Ãâ ½ÇÆÐ: %s\n", functionName, lua_tostring(m_lua, -1));
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

void NKLuaInterface::ResponseFunction(const char* functionName, const luabridge::LuaRef& args)
{
}

void NKLuaInterface::AddVariable(CustomData& var)
{
	m_vecVariable.push_back(var);
	std::sort(m_vecVariable.begin(), m_vecVariable.end(), customCompare);
}

void NKLuaInterface::AddFunction(CustomData& func)
{
	m_vecFunction.push_back(func);
	std::sort(m_vecFunction.begin(), m_vecFunction.end(), customCompare);
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
	//luabridge::getGlobalNamespace(m_lua)
	//	.beginClass<NuklearUI>("NuklearUI")
	//	.addFunction("Add", &NuklearUI::Add)
	//	.endClass();

	//luabridge::push(m_lua, this);
	//lua_setglobal(m_lua, "system");

	//luabridge::getGlobalNamespace(m_lua)
	//	.beginClass<NKBase>("NKBase")
	//	.addFunction("SetActive", &NKBase::SetActive)
	//	.addFunction("AddChild", &NKBase::LAddChild)
	//	.addFunction("RemoveChild", &NKBase::LRemoveChild)
	//	.addFunction("SetPrimaryName", &NKBase::SetPrimaryName)
	//	.endClass()
	//	.deriveClass<NKWindow, NKBase>("NKWindow")
	//	.endClass()
	//	.deriveClass<NKSpace, NKBase>("NKSpace")
	//	.addFunction("SetLayout", &NKSpace::SetLayout)
	//	.addFunction("SetCols", &NKSpace::SetCols)
	//	.endClass()
	//	.deriveClass<NKGroup, NKBase>("NKGroup")
	//	.endClass()
	//	.deriveClass<NKPopup, NKBase>("NKPopup")
	//	.endClass()
	//	.deriveClass<NKCombo, NKBase>("NKCombo")
	//	.addFunction("SetComboName", &NKCombo::SetComboName)
	//	.addFunction("SetLabelSize", &NKCombo::SetLabelSize)
	//	.endClass()
	//	.deriveClass<NKComboItem, NKBase>("NKComboItem")
	//	.addFunction("RegistFunction", &NKComboItem::RegistFunction)
	//	.endClass()
	//	.deriveClass<NKButton, NKBase>("NKButton")
	//	.addFunction("RegistFunction", &NKButton::RegistFunction)
	//	.endClass()
	//	.deriveClass<NKEdit, NKBase>("NKEdit")
	//	.addFunction("Clear", &NKEdit::Clear)
	//	.addFunction("RegistFunction", &NKEdit::RegistFunction)
	//	.endClass()
	//	.deriveClass<NKImage, NKBase>("NKImage")
	//	.endClass()
	//	.deriveClass<NKLabel, NKBase>("NKLabel")
	//	.endClass()
	//	.deriveClass<NKCheckbox, NKBase>("NKCheckbox")
	//	.addFunction("SetLabel", &NKCheckbox::SetLabel)
	//	.addFunction("SetChecked", &NKCheckbox::SetChecked)
	//	.addFunction("IsChecked", &NKCheckbox::IsChecked)
	//	.endClass()
	//	.deriveClass<NKSlider, NKBase>("NKSlider")
	//	.addFunction("SetRange", &NKSlider::SetRange)
	//	.addFunction("SetValue", &NKSlider::SetValue)
	//	.addFunction("GetValue", &NKSlider::GetValue)
	//	.endClass()
	//	.deriveClass<NKProgress, NKBase>("NKProgress")
	//	.addFunction("SetProgress", &NKProgress::SetProgress)
	//	.addFunction("GetProgress", &NKProgress::GetProgress)
	//	.endClass()
	//	.deriveClass<NKSelectable, NKBase>("NKSelectable")
	//	.addFunction("SetLabel", &NKSelectable::SetLabel)
	//	.addFunction("SetSelected", &NKSelectable::SetSelected)
	//	.addFunction("IsSelected", &NKSelectable::IsSelected)
	//	.endClass()
	//	.deriveClass<NKTree, NKBase>("NKTree")
	//	.addFunction("SetLabel", &NKTree::SetLabel)
	//	.addFunction("SetState", &NKTree::SetState)
	//	.addFunction("GetState", &NKTree::GetState)
	//	.endClass()
	//	.deriveClass<NKChart, NKBase>("NKChart")
	//	.addFunction("AddValue", &NKChart::AddValue)
	//	.addFunction("Clear", &NKChart::Clear)
	//	.endClass()
	//	.deriveClass<NKColorPicker, NKBase>("NKColorPicker")
	//	.addFunction("SetColor", &NKColorPicker::SetColor)
	//	.addFunction("GetColor", &NKColorPicker::GetColor)
	//	.endClass()
	//	.deriveClass<NKTooltip, NKBase>("NKTooltip")
	//	.endClass()
	//	.deriveClass<NKMenu, NKBase>("NKMenu")
	//	.addFunction("SetLabel", &NKMenu::SetLabel)
	//	.endClass()
	//	.deriveClass<NKScrollbar, NKBase>("NKScrollbar")
	//	.addFunction("SetScroll", &NKScrollbar::SetScroll)
	//	.addFunction("GetScroll", &NKScrollbar::GetScroll)
	//	.endClass()
	//	.beginClass<ObjMaker>("ObjMaker")
	//	.addStaticFunction("createWindow", &ObjMaker::create<NKWindow>)
	//	.addStaticFunction("createSpace", &ObjMaker::create<NKSpace>)
	//	.addStaticFunction("createGroup", &ObjMaker::create<NKGroup>)
	//	.addStaticFunction("createPopup", &ObjMaker::create<NKPopup>)
	//	.addStaticFunction("createCombo", &ObjMaker::create<NKCombo>)
	//	.addStaticFunction("createComboItem", &ObjMaker::create<NKComboItem>)
	//	.addStaticFunction("createButton", &ObjMaker::create<NKButton>)
	//	.addStaticFunction("createEdit", &ObjMaker::create<NKEdit>)
	//	.addStaticFunction("createImage", &ObjMaker::create<NKImage>)
	//	.addStaticFunction("createLabel", &ObjMaker::create<NKLabel>)
	//	.addStaticFunction("createCheckbox", &ObjMaker::create<NKCheckbox>)
	//	.addStaticFunction("createSlider", &ObjMaker::create<NKSlider>)
	//	.addStaticFunction("createProgress", &ObjMaker::create<NKProgress>)
	//	.addStaticFunction("createSelectable", &ObjMaker::create<NKSelectable>)
	//	.addStaticFunction("createTree", &ObjMaker::create<NKTree>)
	//	.addStaticFunction("createChart", &ObjMaker::create<NKChart>)
	//	.addStaticFunction("createColorPicker", &ObjMaker::create<NKColorPicker>)
	//	.addStaticFunction("createTooltip", &ObjMaker::create<NKTooltip>)
	//	.addStaticFunction("createMenu", &ObjMaker::create<NKMenu>)
	//	.addStaticFunction("createScrollbar", &ObjMaker::create<NKScrollbar>)
	//	.endClass();
}
