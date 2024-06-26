#include "pch.h"
#include "NuklearUI.h"

#include <cereal/types/vector.hpp>
#include <cereal/types/array.hpp>
#include <cereal/types/string.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/types/polymorphic.hpp>

#include "UiLibrary.h"
#include "NuklearEditor.h"

#ifdef _NKDEBUG
NuklearEditor g_editor;
#endif

NuklearUI::NuklearUI()
{
	m_ctx = nullptr;
	m_font = nullptr;
	m_bMouseHovering = false;
	m_bEditActive = false;
	m_pivot = nk_vec2(0, 0);
	m_viewRect = nk_rect(0, 0, 0, 0);
	Register_UI();
#ifdef _NKDEBUG
	g_editor.EditorInit(this, &m_vecObject, &m_vecModule, &m_mapModuleID, &m_mapModuleName, &m_mapImage, &m_mapSpr, &m_vecVariable, &m_vecFunction);
#endif // _NKDEBUG

	m_lua = nullptr;
}

NuklearUI::~NuklearUI()
{
	m_ctx = nullptr;
	m_font = nullptr;
	m_bMouseHovering = false;
	m_bEditActive = false;

	m_lua = nullptr;
}

void NuklearUI::Release()
{
	for (std::vector<NKBase*>::iterator iter = m_vecModule.begin(); iter != m_vecModule.end();)
	{
		NKBase* pNKBase = *iter;

		if (pNKBase)
		{
			unsigned int id = pNKBase->GetPrimaryID();
			const char* name = pNKBase->GetPrimaryName();
			m_mapModuleID.erase(id);
			m_mapModuleName.erase(name);

			pNKBase->Release();
			delete pNKBase;
			pNKBase = NULL;
		}
		iter = m_vecModule.erase(iter);
	}

	lua_close(m_lua);
}

void NuklearUI::NKInputBegin()
{
	if (m_ctx)
	{
		nk_input_begin(m_ctx);
	}

#ifdef _NKDEBUG
	if (g_editor.m_ctx)
	{
		nk_input_begin(g_editor.m_ctx);
	}
#endif
}

void NuklearUI::NKInputEnd()
{
	if (m_ctx)
		nk_input_end(m_ctx);

#ifdef _NKDEBUG
	if (g_editor.m_ctx)
		nk_input_end(g_editor.m_ctx);
#endif
}

void NuklearUI::Update()
{
	m_bMouseHovering = false;
	m_bEditActive = false;

#ifdef _NKDEBUG
	DebugLoadLuaFile(m_filePath);
	RunFunction("Modify");
#endif // _NKDEBUG

	for (std::vector<NKBase*>::iterator iter = m_vecObject.begin(); iter != m_vecObject.end(); ++iter)
	{
		(*iter)->SafeRenderStart(m_ctx);
		(*iter)->Update(m_ctx);

		if ((*iter)->IsHovering())
			m_bMouseHovering = true;
		if ((*iter)->IsEditActive())
			m_bEditActive = true;
	}

#ifdef _NKDEBUG
	DebugLayout();
#endif // _NKDEBUG
}

void NuklearUI::FrameSkip()
{
	if (m_ctx) {
		nk_clear(&m_dx7.d3d7.ctx);
		nk_buffer_clear(&m_dx7.d3d7.cmds);
	}

	if (g_editor.m_ctx) {
		nk_clear(&g_editor.m_dx7.d3d7.ctx);
		nk_buffer_clear(&g_editor.m_dx7.d3d7.cmds);
	}

	for (std::vector<NKBase*>::iterator iter = m_vecModule.begin(); iter != m_vecModule.end(); ++iter)
	{
		(*iter)->SafeRenderEnd(m_ctx);
	}
	ReleaseRenderData();
}

#ifdef _NKDEBUG
void NuklearUI::DebugLayout()
{
	static float debugRectWidth = 500.f;

#ifdef _DX9
	static float debugRectHeight = d3d9.viewport.Height - 50.f;
	static float debugRectPosX = d3d9.viewport.Width - debugRectWidth;
	static float debugRectPosY = 0;
	static struct nk_rect debugRect = nk_rect(d3d9.viewport.Width - debugRectWidth, debugRectPosY, debugRectWidth, debugRectHeight);
#elif _DX7

	D3DVIEWPORT7 viewport;
	m_dx7.d3d7.device->GetViewport(&viewport);

	//static float debugRectHeight = viewport.dwHeight - 50.f;
	static float debugRectHeight = 950.f;
	static float debugRectPosX = viewport.dwWidth - debugRectWidth;
	static float debugRectPosY = 0;
	//static struct nk_rect debugRect = nk_rect(viewport.dwWidth - debugRectWidth, debugRectPosY, debugRectWidth, debugRectHeight);
	static struct nk_rect debugRect = nk_rect(0, debugRectPosY, debugRectWidth, debugRectHeight);

#endif
	g_editor.EditorLayout(debugRect);
}

void NuklearUI::ErrorPopup(const char* content)
{
	g_editor.OpenErrorPopup(content);
}
#endif

nk_flags NuklearUI::IMEInputSystem(nk_context* ctx, char* buffer, int max, int* len, nk_flags flag, nk_plugin_filter filter)
{
	nk_flags result = nk_edit_string_zero_terminated(ctx, flag, buffer, max, filter);

	if (result & NK_EDIT_ACTIVE) {
		IMEInputSystem(ctx, buffer, len);
	}
	return result;
}
void NuklearUI::IMEInputSystem(nk_context* ctx, char* memory, int* len)
{
	if (ctx->text_edit.bComposition) {
		nk_hash hash;
		struct nk_text_edit* edit;
		struct nk_window* win;
		win = ctx->current;
		hash = win->edit.seq;
		edit = &ctx->text_edit;

		if (edit->cursor <= 0) {
			return;
		}

		edit->select_start = edit->cursor - 1;
		edit->select_end = edit->cursor;

		win->edit.sel_start = edit->select_start;
		win->edit.sel_end = edit->select_end;
	}
}

struct nk_image* NuklearUI::SearchImage(int SID)
{
	auto found = m_mapImage.find(SID);

	if (found != m_mapImage.end())
	{
		return &found->second;
	}
	else
	{
		return nullptr;
	}
}

struct nk_rect* NuklearUI::GetViewport()
{
#ifdef _DX9
	m_viewRect = nk_rect(0, 0, d3d9.viewport.Width, d3d9.viewport.Height);

#elif _DX7
	D3DVIEWPORT7 viewport;
	m_dx7.d3d7.device->GetViewport(&viewport);
	m_viewRect = nk_rect(0, 0, (float)viewport.dwWidth, (float)viewport.dwHeight);
#endif // _DX9

	return &m_viewRect;
}

#ifdef _NKDEBUG
BOOL NuklearUI::InitSubWindow(HINSTANCE hInstance, HWND hMainWnd)
{
	return g_editor.InitSubWindow(hInstance, hMainWnd);
}

void NuklearUI::EditorRender()
{
	g_editor.Render();
}
#endif
void NuklearUI::Register_UI()
{
	REGISTER_CHILD(NKWindow);
	REGISTER_CHILD(NKSpace);
	REGISTER_CHILD(NKGroup);
	REGISTER_CHILD(NKPopup);
	REGISTER_CHILD(NKCombo);
	REGISTER_CHILD(NKButton);
	REGISTER_CHILD(NKEdit);
	REGISTER_CHILD(NKImage);
	REGISTER_CHILD(NKLabel);
	REGISTER_CHILD(NKComboItem);
	REGISTER_CHILD(NKCheckbox);
	REGISTER_CHILD(NKSlider);
	REGISTER_CHILD(NKProgress);
	REGISTER_CHILD(NKSelectable);
	REGISTER_CHILD(NKTree);
	REGISTER_CHILD(NKChart);
	REGISTER_CHILD(NKTooltip);
	REGISTER_CHILD(NKMenu);
	REGISTER_CHILD(NKScrollbar);
	REGISTER_CHILD(NKColorPicker);
	REGISTER_CHILD(NKSuperStyleObject);
}
std::vector<NKBase*>* NuklearUI::GetNodes()
{
	return &m_vecModule;
}
void NuklearUI::CreateUI(const char* classname, NKBase* parent)
{
	NKBase* pBase = m_factory.create(classname, m_ctx, this);

	if (pBase) {
		if (parent) {
			parent->AddChild(pBase);
		}
		else {
			Add(pBase);
		}
	}
	else
	{
		throw;
	}
}

void NuklearUI::CopyUI(NKBase* pBase, NKBase* parent)
{
	NKBase* ptr = CopyObject(pBase);

	if (parent) {
		parent->AddChild(ptr, false);
	}
	else {
		Add(ptr, false);
	}
}

NKBase* NuklearUI::RegistUI(const char* classname, NKBase* pBase)
{
	if (pBase->GetType() == eWINDOW)
	{
		m_vecObject.push_back(pBase);
	}

	m_vecModule.push_back(pBase);
	m_mapModuleID.insert(std::make_pair(pBase->GetPrimaryID(), pBase));
	m_mapModuleName.insert(std::make_pair(pBase->GetPrimaryName(), pBase));

	auto bFinder = dynamic_cast<NKObjectFinder*>(pBase);
	if (bFinder) {
		m_mapOF.insert(std::make_pair(pBase->GetPrimaryID(), bFinder));
	}

	return pBase;
}

struct nk_vec2* NuklearUI::GetPivot()
{
	return &m_pivot;
}
void NuklearUI::SetPrimary(NKBase* pBase)
{
	pBase->SetPrimaryID(reinterpret_cast<intptr_t>(pBase));
	pBase->SetNuklearIndex(m_vecModule.size());

	char primaryName[256] = { 0, };
	sprintf_s(primaryName, "%s%ld", pBase->GetPrimaryName(), reinterpret_cast<intptr_t>(pBase));
	pBase->SetPrimaryName(primaryName);
}
bool NuklearUI::SetPrimaryname(NKBase* pBase, const char* name)
{
	bool bSuccess = false;
	auto it = m_mapModuleName.find(name);
	if (it == m_mapModuleName.end()) {
		m_mapModuleName.erase(pBase->GetPrimaryName());
		pBase->SetPrimaryName(name);
		m_mapModuleName.insert(std::make_pair(pBase->GetPrimaryName(), pBase));
		bSuccess = true;
	}
	return bSuccess;
}
void NuklearUI::Add(NKBase* type, bool bStyle)
{
	NKBase* base = type;
	
	if (base->GetType() == eWINDOW)
	{
		base->Initialize(this, bStyle);
		m_vecObject.push_back(base);
	}

	SetPrimary(base);

	m_vecModule.push_back(base);
	m_mapModuleID.insert(std::make_pair(base->GetPrimaryID(), base));
	m_mapModuleName.insert(std::make_pair(base->GetPrimaryName(), base));

	auto bFinder = dynamic_cast<NKObjectFinder*>(base);
	if (bFinder) {
		m_mapOF.insert(std::make_pair(base->GetPrimaryID(), bFinder));
	}
}

void NuklearUI::Move(unsigned int id)
{

}

void NuklearUI::Remove(unsigned int id)
{
	std::map<unsigned int, NKBase*>::iterator it = m_mapModuleID.find(id);
	if (it != m_mapModuleID.end()) {
		NKBase* pBase = it->second;

		const char* name = pBase->GetPrimaryName();

		m_mapModuleID.erase(id);
		m_mapModuleName.erase(name);
		m_vecModule.erase(m_vecModule.begin() + pBase->GetNuklearIndex());
		auto pOF = m_mapOF.find(id);
		if (pOF != m_mapOF.end()) {
			m_mapOF.erase(pOF);
		}

		if (pBase->GetType() == eWINDOW)
		{
			auto it = m_vecObject.begin();
			for (; it != m_vecObject.end();)
			{
				if ((*it)->GetPrimaryID() == pBase->GetPrimaryID()) {
					it = m_vecObject.erase(it);
				}
				else {
					++it;
				}
			}
		}

		for (size_t i = (size_t)pBase->GetNuklearIndex(); i < m_vecModule.size(); ++i)
		{
			NKBase* base = m_vecModule.at(i);
			if (base)
			{
				base->SetNuklearIndex(i);
			}
		}

		NKBase* parent = pBase->GetParent();
		if (parent != nullptr)
		{
			parent->RemoveChildDisConnect(pBase);
		}

		for (auto it = m_mapOF.begin(); it != m_mapOF.end(); ++it) {
			it->second->LostObjectEvent(id);
		}

		pBase->Release();
		delete pBase;
		pBase = nullptr;
	}
	else
	{
		//not found id
	}
}

void NuklearUI::Remove(const char* name)
{
	std::map<std::string, NKBase*>::iterator it = m_mapModuleName.find(name);
	if (it != m_mapModuleName.end())
	{
		NKBase* pBase = it->second;

		unsigned int id = pBase->GetPrimaryID();

		m_mapModuleID.erase(id);
		m_mapModuleName.erase(name);
		m_vecModule.erase(m_vecModule.begin() + pBase->GetNuklearIndex());
		auto pOF = m_mapOF.find(id);
		if (pOF != m_mapOF.end()) {
			m_mapOF.erase(pOF);
		}

		if (pBase->GetType() == eWINDOW)
		{
			auto it = m_vecObject.begin();
			for (; it != m_vecObject.end();)
			{
				if ((*it)->GetPrimaryID() == pBase->GetPrimaryID()) {
					it = m_vecObject.erase(it);
				}
				else {
					++it;
				}
			}
		}

		for (size_t i = (size_t)pBase->GetNuklearIndex(); i < m_vecModule.size(); ++i)
		{
			NKBase* base = m_vecModule.at(i);
			if (base)
			{
				base->SetNuklearIndex(i);
			}
		}

		NKBase* parent = pBase->GetParent();
		if (parent != nullptr)
		{
			parent->RemoveChildDisConnect(pBase);
		}

		for (auto it = m_mapOF.begin(); it != m_mapOF.end(); ++it) {
			it->second->LostObjectEvent(id);
		}

		pBase->Release();
		delete pBase;
		pBase = nullptr;
	}
	else
	{
		//not found name
	}
}

void NuklearUI::Remove(NKBase* obj)
{
	if (obj)
	{
		unsigned int id = obj->GetPrimaryID();
		const char* name = obj->GetPrimaryName();
		m_mapModuleID.erase(id);
		m_mapModuleName.erase(name);
		m_vecModule.erase(m_vecModule.begin() + obj->GetNuklearIndex());
		auto pOF = m_mapOF.find(id);
		if (pOF != m_mapOF.end()) {
			m_mapOF.erase(pOF);
		}

		if (obj->GetType() == eWINDOW)
		{
			auto it = m_vecObject.begin();
			for (; it != m_vecObject.end();)
			{
				if ((*it)->GetPrimaryID() == obj->GetPrimaryID()) {
					it = m_vecObject.erase(it);
				}
				else {
					++it;
				}
			}
		}

		for (size_t i = (size_t)obj->GetNuklearIndex(); i < m_vecModule.size(); ++i)
		{
			NKBase* base = m_vecModule.at(i);
			if (base)
			{
				base->SetNuklearIndex(i);
			}
		}

		NKBase* parent = obj->GetParent();
		if (parent != nullptr)
		{
			parent->RemoveChildDisConnect(obj);
		}

		for (auto it = m_mapOF.begin(); it != m_mapOF.end(); ++it) {
			it->second->LostObjectEvent(id);
		}

		obj->Release();
		delete obj;
		obj = nullptr;
	}
	else
	{
		// obj is null
	}
}

void NuklearUI::Remove(int idx)
{
	NKBase* pBase = m_vecModule.at(idx);

	if (pBase)
	{
		unsigned int id = pBase->GetPrimaryID();
		const char* name = pBase->GetPrimaryName();
		m_mapModuleID.erase(id);
		m_mapModuleName.erase(name);
		m_vecModule.erase(m_vecModule.begin() + pBase->GetNuklearIndex());
		auto pOF = m_mapOF.find(id);
		if (pOF != m_mapOF.end()) {
			m_mapOF.erase(pOF);
		}

		if (pBase->GetType() == eWINDOW)
		{
			auto it = m_vecObject.begin();
			for (; it != m_vecObject.end();)
			{
				if ((*it)->GetPrimaryID() == pBase->GetPrimaryID()) {
					it = m_vecObject.erase(it);
				}
				else {
					++it;
				}
			}
		}

		for (size_t i = (size_t)pBase->GetNuklearIndex(); i < m_vecModule.size(); ++i)
		{
			NKBase* base = m_vecModule.at(i);
			if (base)
			{
				base->SetNuklearIndex(i);
			}
		}

		NKBase* parent = pBase->GetParent();
		if (parent != nullptr)
		{
			parent->RemoveChildDisConnect(pBase);
		}

		for (auto it = m_mapOF.begin(); it != m_mapOF.end(); ++it) {
			it->second->LostObjectEvent(id);
		}

		pBase->Release();
		delete pBase;
		pBase = nullptr;
	}
	else
	{
		//not found index
	}
}

void NuklearUI::LoadLuaFile(const char* filePath)
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

luabridge::LuaRef NuklearUI::GetLuaTable(const char* tableName)
{
	return luabridge::getGlobal(m_lua, tableName);
}

bool NuklearUI::RunFunction(const char* functionName)
{
	lua_getglobal(m_lua, functionName);
	if (lua_pcall(m_lua, 0, 0, 0) != 0) {
		fprintf(stderr, "%s ÇÔ¼ö È£Ãâ ½ÇÆÐ: %s\n", functionName, lua_tostring(m_lua, -1));
		lua_pop(m_lua, 1);
		return false;
	}
	return true;
}

bool NuklearUI::RunFunctionArgs(const char* functionName, const luabridge::LuaRef& args)
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

void NuklearUI::AddVariable(CustomData& var)
{
	m_vecVariable.push_back(var);
	std::sort(m_vecVariable.begin(), m_vecVariable.end(), customCompare);
}

void NuklearUI::AddFunction(CustomData& func)
{
	m_vecFunction.push_back(func);
	std::sort(m_vecFunction.begin(), m_vecFunction.end(), customCompare);
}

std::wstring NuklearUI::utf8ToWstring(const char* str)
{
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
	std::wstring wstrTo(size_needed - 1, 0); // -1 to exclude the null terminator
	MultiByteToWideChar(CP_UTF8, 0, str, -1, &wstrTo[0], size_needed);
	return wstrTo;
}

bool NuklearUI::customCompare(const CustomData aData, const CustomData bData)
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
void NuklearUI::DebugLoadLuaFile(const char* filePath)
{
	if (luaL_dofile(m_lua, filePath) != LUA_OK) {
		std::cerr << lua_tostring(m_lua, -1) << std::endl;
	}
}
#endif // _NKDEBUG
void NuklearUI::RegisterBase()
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

void NuklearUI::SaveFile(const std::string& filename)
{
	std::vector<std::string> vSprData;
	size_t size = m_vecModule.size();
	std::vector<std::string> vStr;
	std::vector<std::shared_ptr<NKBase>> vec;

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

	archive(CEREAL_NVP(m_vecVariable));
	archive(CEREAL_NVP(m_vecFunction));

	archive(CEREAL_NVP(size));
	archive(CEREAL_NVP(vStr));


	for (size_t i = 0; i < size; ++i) {
		NKBase* ptr = m_vecModule.at(i);
		SaveSwitch(vec, ptr, archive);
	}
}

void NuklearUI::LoadFile(const std::string& filename)
{
	std::vector<std::string> vSprData;
	size_t size;
	std::vector<std::string> vStr;
	std::vector<std::shared_ptr<NKBase>> vec;

	std::ifstream is(filename);
	cereal::JSONInputArchive archive(is);

	archive(CEREAL_NVP(vSprData));

	for (size_t i = 0; i < vSprData.size(); ++i) {
		std::string str = vSprData.at(i);
		LoadSprFile(str.c_str());
	}

	archive(CEREAL_NVP(m_vecVariable));
	archive(CEREAL_NVP(m_vecFunction));

	archive(CEREAL_NVP(size));
	archive(CEREAL_NVP(vStr));

	for (size_t i = 0; i < size; ++i) {
		NKBase* ptr = m_factory.create(vStr.at(i), m_ctx, this);
		ptr->Initialize(this);
		LoadSwitch(vec, ptr, archive, i);
		RegistUI(vStr.at(i).c_str(), ptr);
	}

	for (auto it = m_vecModule.begin(); it != m_vecModule.end(); ++it) {
		LoadNode(*it);
	}

	for (auto it = m_vecModule.begin(); it != m_vecModule.end(); ++it) {
		NKBase* pBase = *it;
		ResetPrimaryID(pBase);
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

void NuklearUI::LoadNode(NKBase* pBase)
{
	unsigned int ppID = pBase->GetParentPrimaryID();

	if (ppID != 0) {
		auto found = m_mapModuleID.find(ppID);
		if (found != m_mapModuleID.end()) {
			NKBase* parent = found->second;
			parent->RegistChild(pBase);
		}
	}
}

void NuklearUI::ResetPrimaryID(NKBase* pBase)
{
	auto found = m_mapModuleID.find(pBase->GetPrimaryID());

	if (found != m_mapModuleID.end()) {
		m_mapModuleID.erase(found);
		unsigned int id = reinterpret_cast<intptr_t>(pBase);
		pBase->SetPrimaryID(id);
	}
}