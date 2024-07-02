#include "pch.h"
#include "NuklearUI.h"
#include "UiLibrary.h"
#include "NuklearEditor.h"

#ifdef _NKDEBUG
NuklearEditor g_editor;
#endif

NuklearUI::NuklearUI():
	m_mapVariable(m_luaInterface.m_mapVariable),
	m_mapFunction(m_luaInterface.m_mapFunction),
	m_vecObject(m_cereal.m_vecObject),
	m_vecModule(m_cereal.m_vecModule),
	m_mapModuleID(m_cereal.m_mapModuleID),
	m_mapModuleName(m_cereal.m_mapModuleName),
	m_mapSpr(m_cereal.m_mapSpr)
{
	m_ctx = nullptr;
	m_font = nullptr;
	m_bMouseHovering = false;
	m_bEditActive = false;
	m_pivot = nk_vec2(0, 0);
	m_viewRect = nk_rect(0, 0, 0, 0);
	Register_UI();
	m_cereal.m_pManager = this;
	m_luaInterface.m_pManager = this;

#ifdef _NKDEBUG
	g_editor.EditorInit(this, &m_vecObject, &m_vecModule, &m_mapModuleID, &m_mapModuleName, &m_mapImage, &m_mapSpr, &m_mapVariable, &m_mapFunction, &m_cereal.m_vecPrefab);
#endif // _NKDEBUG
}

NuklearUI::~NuklearUI()
{
	m_ctx = nullptr;
	m_font = nullptr;
	m_bMouseHovering = false;
	m_bEditActive = false;
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
	m_luaInterface.Release();
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
	//m_luaInterface.DebugLoadLuaFile(m_luaInterface.m_filePath);
	m_luaInterface.RunFunction("Modify");
#endif // _NKDEBUG
	m_luaInterface.RunFunction("Update");

	for (std::vector<NKBase*>::iterator iter = m_vecModule.begin(); iter != m_vecModule.end(); ++iter)
	{
		(*iter)->SafeRenderStart(m_ctx);
	}

	for (std::vector<NKBase*>::iterator iter = m_vecObject.begin(); iter != m_vecObject.end(); ++iter)
	{
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
		m_dx7.nk_d3d7_render_skip();
	}

	if (g_editor.m_ctx) {
		g_editor.m_dx7.nk_d3d7_render_skip();
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
NKBase* NuklearUI::SimpleCreateUI(const char* classname)
{
	NKBase* pBase = m_factory.create(classname, m_ctx, this);
	return pBase;
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

NKBase* NuklearUI::RegistUI(NKBase* pBase)
{
	if (pBase->GetType() == eWINDOW)
	{
		m_vecObject.push_back(pBase);
	}

	pBase->SetNuklearIndex(m_vecModule.size());
	m_vecModule.push_back(pBase);
	m_mapModuleID.insert(std::make_pair(pBase->GetPrimaryID(), pBase));


	auto found = m_mapModuleName.find(pBase->GetPrimaryName());
	if (found != m_mapModuleName.end()) {
		char primaryName[256] = { 0, };
		sprintf_s(primaryName, "%s%ld", pBase->getClassName().c_str(), reinterpret_cast<intptr_t>(pBase));
		pBase->SetPrimaryName(primaryName);
	}
	else {
		m_mapModuleName.insert(std::make_pair(pBase->GetPrimaryName(), pBase));
	}

	auto bFinder = dynamic_cast<NKObjectFinder*>(pBase);
	if (bFinder) {
		m_mapOF.insert(std::make_pair(pBase->GetPrimaryID(), bFinder));
	}

	auto bHandler = dynamic_cast<NKHandler*>(pBase);
	if (bHandler) {
		m_luaInterface.SubscribeFunction(bHandler->GetFunctionName(), bHandler);
		m_luaInterface.SubscribeVariable(bHandler->GetArgsName(), bHandler);
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

void NuklearUI::Move(unsigned int child, unsigned int parent)
{
	NKBase* pChild = nullptr;
	NKBase* pParent = nullptr;

	std::map<unsigned int, NKBase*>::iterator it_c = m_mapModuleID.find(child);
	if (it_c != m_mapModuleID.end()) {
		pChild = it_c->second;
	}

	std::map<unsigned int, NKBase*>::iterator it_p = m_mapModuleID.find(parent);
	if (it_p != m_mapModuleID.end()) {
		pParent = it_p->second;
	}

	if (pChild == nullptr || pParent == nullptr || pChild->GetParent() == nullptr) {
		return;
	}

	pChild->GetParent()->RemoveChildDisConnect(pChild);
	pParent->RegistChild(pChild);
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

void NuklearUI::LoadNode(NKBase* pBase, bool bBegin)
{
	unsigned int ppID = pBase->GetParentPrimaryID();

	if (ppID != 0 && !bBegin) {
		auto found = m_mapModuleID.find(ppID);
		if (found != m_mapModuleID.end()) {
			NKBase* parent = found->second;
			parent->RegistChild(pBase);
		}
	}
}

void NuklearUI::LoadLinkNode(NKBase* pBase)
{
	auto bFinder = dynamic_cast<NKObjectFinder*>(pBase);
	if (bFinder) {
		unsigned int ppID = bFinder->GetLinkObjPrimaryID();
		if (ppID != 0) {
			auto found = m_mapModuleID.find(ppID);
			if (found != m_mapModuleID.end()) {
				bFinder->RegistObjectEvent(found->second);
			}
		}
		else {
			bFinder->FailRegist();
		}
		m_mapOF.insert(std::make_pair(pBase->GetPrimaryID(), bFinder));
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