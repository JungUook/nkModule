#include "pch.h"
#include "NuklearUI.h"

#ifndef NK_ASSERT
#include <assert.h>
#define NK_ASSERT(expr) assert(expr)
#endif


#ifdef _DX9
#define NK_D3D9_IMPLEMENTATION
#include <nuklear_d3d9.h>
#elif _DX7
#define NK_D3D7_IMPLEMENTATION
#include <nuklear_d3d7.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#endif

#include <shlobj.h>

#include "UiLibrary.h"

struct nk_image g_img;

NuklearUI::NuklearUI()
{
	m_ctx = nullptr;
	m_font = nullptr;
	m_bMouseHovering = false;
	m_bEditActive = false;
	m_original_height = 0;
	m_primaryIDCheck = 0;

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

#ifdef _DX9
void NuklearUI::Initialize(IDirect3DDevice9* device, int width, int height, int lang)
{
	D3DVIEWPORT9 viewport;
	D3DMATRIX projection;
	IDirect3DDevice9_GetViewport(device, &viewport);
	IDirect3DDevice9_GetTransform(device, D3DTS_PROJECTION, &projection);

	CHAR path[MAX_PATH];
	if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_FONTS, NULL, 0, path))) {
		std::cout << "System font path: " << path << std::endl;
	}
	m_ctx = nk_d3d9_init(device, viewport.Width, viewport.Height);

	d3d9.viewport = viewport;
	d3d9.projection = projection;

	struct nk_font_atlas* atlas;
	nk_d3d9_font_stash_begin(&atlas);

	m_original_height = 24.0f;
	struct nk_font_config cfg = nk_font_config(m_original_height);
	cfg.oversample_h = 1; // 수평 오버샘플링
	cfg.oversample_v = 1; // 수직 오버샘플링

	eLang language = (eLang)lang;

	char fontPath[512] = { 0, };
	switch (language)
	{
		case NuklearUI::JPN: {
			sprintf_s(fontPath, "%s\\msgothic.ttc", path);

			NK_STORAGE const nk_rune ranges[] = {
				0x0020, 0x007E,
				0x3001, 0x3003,
				0x3005, 0x301F,
				0x3036, 0x3036,
				0x3041, 0x309F,
				0x30A0, 0x30FF,
				0x4E00, 0x9FFF,
				0
			};

			cfg.range = ranges;
		}
		break;

		case NuklearUI::TWA: {
			sprintf_s(fontPath, "%s\\simsun.ttc", path);
			cfg.range = nk_font_chinese_glyph_ranges();
		}
		break;

		case NuklearUI::CHI: {
			sprintf_s(fontPath, "%s\\Msjhl.ttc", path);
			cfg.range = nk_font_chinese_glyph_ranges();
		}
		break;

		case NuklearUI::KOR:
		default: {
			sprintf_s(fontPath, "%s\\gulim.ttc", path);
			cfg.range = nk_font_korean_glyph_ranges();
		}
		break;
	}


	NK_ASSERT(atlas);
	NK_ASSERT(atlas->temporary.alloc);
	NK_ASSERT(atlas->temporary.free);
	NK_ASSERT(atlas->permanent.alloc);
	NK_ASSERT(atlas->permanent.free);

	std::ifstream file(fontPath, std::ios::binary | std::ios::ate);
	if (!file.is_open()) {
		std::cerr << "Failed to open file: " << fontPath << std::endl;
		return;
	}
	std::streamsize file_size = file.tellg();
	file.seekg(0, std::ios::beg);

	char* buffer = new char[file_size];
	nk_size size;
	if (file.read(buffer, file_size)) {
		size = static_cast<nk_size>(file_size);
	}
	else {
		delete[] buffer;
		std::cerr << "Failed to read file: " << fontPath << std::endl;
		return;
	}

	cfg = (&cfg) ? cfg : nk_font_config(m_original_height);
	cfg.ttf_blob = buffer;
	cfg.ttf_size = size;
	cfg.size = m_original_height;
	cfg.ttf_data_owned_by_atlas = 1;
	m_font = nk_font_atlas_add(atlas, &cfg);

	//m_font = nk_font_atlas_add_from_file(atlas, fontPath, m_original_height, &cfg);
	nk_d3d9_font_stash_end();
	nk_style_set_font(m_ctx, &m_font->handle);

	m_bMouseHovering = false;
	m_bEditActive = false;

	m_lua = luaL_newstate();
	luaL_openlibs(m_lua);
	RegisterBase();
}
#elif _DX7
void NuklearUI::Initialize(IDirectDraw7* pdd, IDirect3DDevice7* pdevice, int width, int height, int lang)
{
	CHAR path[MAX_PATH];
	if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_FONTS, NULL, 0, path))) {
		std::cout << "System font path: " << path << std::endl;
	}

	m_ctx = nk_d3d7_init(pdd, pdevice);

	struct nk_font_atlas* atlas;
	nk_d3d7_font_stash_begin(&atlas);

	m_original_height = 24.0f;
	struct nk_font_config cfg = nk_font_config(m_original_height);
	cfg.oversample_h = 1; // 수평 오버샘플링
	cfg.oversample_v = 1; // 수직 오버샘플링

	eLang language = (eLang)lang;

	char fontPath[512] = { 0, };
	switch (language)
	{
	case NuklearUI::JPN: {
		sprintf_s(fontPath, "%s\\msgothic.ttc", path);

		NK_STORAGE const nk_rune ranges[] = {
			0x0020, 0x007E,
			0x3001, 0x3003,
			0x3005, 0x301F,
			0x3036, 0x3036,
			0x3041, 0x309F,
			0x30A0, 0x30FF,
			0x4E00, 0x9FFF,
			0
		};

		cfg.range = ranges;
	}
					   break;

	case NuklearUI::TWA: {
		sprintf_s(fontPath, "%s\\simsun.ttc", path);
		cfg.range = nk_font_chinese_glyph_ranges();
	}
					   break;

	case NuklearUI::CHI: {
		sprintf_s(fontPath, "%s\\Msjhl.ttc", path);
		cfg.range = nk_font_chinese_glyph_ranges();
	}
					   break;

	case NuklearUI::KOR:
	default: {
		sprintf_s(fontPath, "%s\\gulim.ttc", path);
		cfg.range = nk_font_korean_glyph_ranges();
	}
		   break;
	}


	NK_ASSERT(atlas);
	NK_ASSERT(atlas->temporary.alloc);
	NK_ASSERT(atlas->temporary.free);
	NK_ASSERT(atlas->permanent.alloc);
	NK_ASSERT(atlas->permanent.free);

	std::ifstream file(fontPath, std::ios::binary | std::ios::ate);
	if (!file.is_open()) {
		std::cerr << "Failed to open file: " << fontPath << std::endl;
		return;
	}
	std::streamsize file_size = file.tellg();
	file.seekg(0, std::ios::beg);

	char* buffer = new char[file_size];
	nk_size size;
	if (file.read(buffer, file_size)) {
		size = static_cast<nk_size>(file_size);
	}
	else {
		delete[] buffer;
		std::cerr << "Failed to read file: " << fontPath << std::endl;
		return;
	}

	cfg = (&cfg) ? cfg : nk_font_config(m_original_height);
	cfg.ttf_blob = buffer;
	cfg.ttf_size = size;
	cfg.size = m_original_height;
	cfg.ttf_data_owned_by_atlas = 1;
	m_font = nk_font_atlas_add(atlas, &cfg);

	//m_font = nk_font_atlas_add_from_file(atlas, fontPath, m_original_height, &cfg);
	nk_d3d7_font_stash_end();
	nk_style_set_font(m_ctx, &m_font->handle);

	m_bMouseHovering = false;
	m_bEditActive = false;

	m_lua = luaL_newstate();
	luaL_openlibs(m_lua);
	RegisterBase();
	
	//g_img = nk_image_id(0);
	//IDirectDrawSurface7* tImg = nullptr;
	//if (ReadImageFile("C:/Users/kimjw/Downloads/dsdfasd.jpg", &tImg))
	//{
	//	g_img.handle.ptr = tImg;
	//}
}

#endif

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
		nk_input_begin(m_ctx);
}

void NuklearUI::NKInputEnd()
{
	if (m_ctx)
		nk_input_end(m_ctx);
}

void NuklearUI::Update()
{
	m_bMouseHovering = false;
	m_bEditActive = false;

#ifdef _DEBUG
	DebugLoadLuaFile(m_filePath);
	RunFunction("Modify");
#endif // _DEBUG

	for (std::vector<NKBase*>::iterator iter = m_vecObject.begin(); iter != m_vecObject.end(); ++iter)
	{
		(*iter)->SafeRenderStart();
		(*iter)->Update(m_ctx);

		if ((*iter)->IsHovering())
			m_bMouseHovering = true;
		if ((*iter)->IsEditActive())
			m_bEditActive = true;
	}

#ifdef _DEBUG
	DebugLayout();
#endif // _DEBUG
}

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
	d3d7.device->GetViewport(&viewport);

	static float debugRectHeight = viewport.dwHeight - 50.f;
	static float debugRectPosX = viewport.dwWidth - debugRectWidth;
	static float debugRectPosY = 0;
	static struct nk_rect debugRect = nk_rect(viewport.dwWidth - debugRectWidth, debugRectPosY, debugRectWidth, debugRectHeight);
#endif

	if (nk_begin(m_ctx, "debug", debugRect, NK_WINDOW_TITLE | NK_WINDOW_MINIMIZABLE | NK_WINDOW_NO_SCROLLBAR))
	{
		nk_layout_row_dynamic(m_ctx, 50.f, 2);
		if (nk_button_label(m_ctx, "New"))
		{

		}
		if (nk_button_label(m_ctx, "Refresh"))
		{
			for (std::vector<NKBase*>::iterator iter = m_vecObject.begin(); iter != m_vecObject.end();)
			{
				NKBase* pNKBase = *iter;

				if (pNKBase)
				{
					pNKBase->Release();
					delete pNKBase;
					pNKBase = NULL;
				}
				iter = m_vecObject.erase(iter);
			}
			m_vecObject.clear();
			m_vecModule.clear();
			m_mapModuleID.clear();
			m_mapModuleName.clear();

			RunFunction("Init");
		}


		if (nk_contextual_begin(m_ctx, 0, nk_vec2(100, 220), nk_window_get_bounds(m_ctx))) {
			const char* grid_option[] = { "Show Grid", "Hide Grid" };
			nk_layout_row_dynamic(m_ctx, 25, 1);
			if (nk_contextual_item_label(m_ctx, "New", NK_TEXT_CENTERED))
			{
			}
			if (nk_contextual_item_label(m_ctx, grid_option[0], NK_TEXT_CENTERED))
			{

			}
			nk_contextual_end(m_ctx);
		}
		enum { EASY, HARD, NORMAL };
		static int op = EASY;
		nk_layout_row_dynamic(m_ctx, 30, 3);
		if (nk_option_label(m_ctx, "easy", op == EASY)) op = EASY;
		if (nk_option_label(m_ctx, "hard", op == HARD)) op = HARD;
		if (nk_option_label(m_ctx, "normal", op == NORMAL)) op = NORMAL;

		static int groupLeft = 150;
		nk_layout_row_dynamic(m_ctx, 22, 1);
		nk_property_int(m_ctx, "#Left:", 150, &groupLeft, debugRectWidth - 100, 1, 1);

		float row_layout[2];
		row_layout[0] = groupLeft;
		row_layout[1] = debugRectWidth - groupLeft;

		nk_layout_row(m_ctx, NK_STATIC, debugRectHeight, 2, row_layout);
		DebugLayoutLeft(row_layout[0]);
		DebugLayoutRight(row_layout[1]);
	}
	nk_end(m_ctx);
}

void NuklearUI::DebugLayoutLeft(int width)
{
	if (nk_group_begin(m_ctx, "Node", 0)) {
		int i = 0;
		nk_layout_row_static(m_ctx, 18, width, 1);

		for (i = 0; i < m_vecObject.size(); ++i)
		{
			DebugLayoutLeftNodes(m_vecObject.at(i), NK_TREE_TAB, NK_MINIMIZED);
		}

		//for (i = 0; i < 16; ++i)
			//nk_selectable_label(m_ctx, (selected[i]) ? "Selected" : "Unselected", NK_TEXT_LEFT, &selected[i]);
		nk_group_end(m_ctx);
	}
}

void NuklearUI::DebugLayoutLeftNodes(NKBase* pBase, nk_tree_type nkType, nk_collapse_states nkState)
{
	if (nk_tree_push(m_ctx, nkType, pBase->GetPrimaryName(), nkState)) {
		
		std::list<NKBase*>* plist = pBase->GetChildList();
		auto it = plist->begin();
		for (; it != plist->end(); ++it)
		{
			DebugLayoutLeftNodes(*it, (*it)->GetTreeType(), (*it)->GetCollapseState());
		}
		nk_tree_pop(m_ctx);
	}
}

void NuklearUI::DebugLayoutRight(int width)
{
	if (nk_group_begin(m_ctx, "ObjectInfo", 0)) {
		if (nk_tree_push(m_ctx, NK_TREE_TAB, "Transform", NK_MINIMIZED)) {

			static int PosX=0;
			static int PosY=0;
			nk_layout_row_dynamic(m_ctx, 22, 2);
			nk_property_int(m_ctx, "#X:", 0, &PosX, 1920, 1, 1);
			nk_property_int(m_ctx, "#Y:", 0, &PosY, 1920, 1, 1);


			static int Width = 0;
			static int Height = 0;
			nk_layout_row_dynamic(m_ctx, 22, 2);
			nk_property_int(m_ctx, "#W:", 0, &Width, 1920, 1, 1);
			nk_property_int(m_ctx, "#H:", 0, &Height, 1920, 1, 1);
			nk_tree_pop(m_ctx);
		}
		if (nk_tree_push(m_ctx, NK_TREE_TAB, "Style", NK_MINIMIZED)) {
			if (nk_tree_push(m_ctx, NK_TREE_NODE, "Image", NK_MINIMIZED)) {
				nk_layout_row_static(m_ctx, 256, 256, 1);

				g_img = nk_image_id(0);
				struct nk_image* tImg = nullptr;
				tImg = &m_mapImage[0];
				if (tImg)
				{
					g_img = *tImg;
					nk_image(m_ctx, g_img);
				}
				nk_tree_pop(m_ctx);
			}
			if (nk_tree_push(m_ctx, NK_TREE_NODE, "text", NK_MINIMIZED)) {
				nk_tree_pop(m_ctx);
			}
			if (nk_tree_push(m_ctx, NK_TREE_NODE, "contextual_button", NK_MINIMIZED)) {
				nk_tree_pop(m_ctx);
			}
			if (nk_tree_push(m_ctx, NK_TREE_NODE, "menu_button", NK_MINIMIZED)) {
				nk_tree_pop(m_ctx);
			}
			if (nk_tree_push(m_ctx, NK_TREE_NODE, "option", NK_MINIMIZED)) {
				nk_tree_pop(m_ctx);
			}
			if (nk_tree_push(m_ctx, NK_TREE_NODE, "checkbox", NK_MINIMIZED)) {
				nk_tree_pop(m_ctx);
			}
			if (nk_tree_push(m_ctx, NK_TREE_NODE, "selectable", NK_MINIMIZED)) {
				nk_tree_pop(m_ctx);
			}
			if (nk_tree_push(m_ctx, NK_TREE_NODE, "slider", NK_MINIMIZED)) {
				nk_tree_pop(m_ctx);
			}
			if (nk_tree_push(m_ctx, NK_TREE_NODE, "progress", NK_MINIMIZED)) {
				nk_tree_pop(m_ctx);
			}
			if (nk_tree_push(m_ctx, NK_TREE_NODE, "property", NK_MINIMIZED)) {
				nk_tree_pop(m_ctx);
			}
			if (nk_tree_push(m_ctx, NK_TREE_NODE, "edit", NK_MINIMIZED)) {
				nk_tree_pop(m_ctx);
			}
			if (nk_tree_push(m_ctx, NK_TREE_NODE, "chart", NK_MINIMIZED)) {
				nk_tree_pop(m_ctx);
			}
			if (nk_tree_push(m_ctx, NK_TREE_NODE, "scrollh", NK_MINIMIZED)) {
				nk_tree_pop(m_ctx);
			}
			if (nk_tree_push(m_ctx, NK_TREE_NODE, "scrollv", NK_MINIMIZED)) {
				nk_tree_pop(m_ctx);
			}
			if (nk_tree_push(m_ctx, NK_TREE_NODE, "tab", NK_MINIMIZED)) {
				nk_tree_pop(m_ctx);
			}
			if (nk_tree_push(m_ctx, NK_TREE_NODE, "combo", NK_MINIMIZED)) {
				nk_tree_pop(m_ctx);
			}
			if (nk_tree_push(m_ctx, NK_TREE_NODE, "window", NK_MINIMIZED)) {
				nk_tree_pop(m_ctx);
			}

			nk_tree_pop(m_ctx);
		}
		nk_group_end(m_ctx);
	}
}

#ifdef _DX9
void NuklearUI::Render(IDirect3DDevice9* device)
{
	DWORD dwPrevFVF = 0;
	IDirect3DDevice9_GetFVF(device, &dwPrevFVF);

	IDirect3DDevice9_SetFVF(device, D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);


	DWORD dwD3DRS_SRCBLEND = 0;
	DWORD dwD3DRS_DESTBLEND = 0;
	DWORD dwD3DRS_ALPHABLENDENABLE = 0;
	DWORD dwD3DRS_BLENDOP = 0;

	DWORD dwD3DRS_LIGHTING = 0;
	DWORD dwD3DRS_ZENABLE = 0;
	DWORD dwD3DRS_ZWRITEENABLE = 0;
	DWORD dwD3DRS_CULLMODE = 0;
	DWORD dwD3DRS_SCISSORTESTENABLE = 0;

	DWORD dwD3DSAMP_ADDRESSU = 0;
	DWORD dwD3DSAMP_ADDRESSV = 0;
	DWORD dwD3DSAMP_MAGFILTER = 0;
	DWORD dwD3DSAMP_MINFILTER = 0;

	DWORD dwD3DTSS_COLOROP = 0;
	DWORD dwD3DTSS_COLORARG1 = 0;
	DWORD dwD3DTSS_COLORARG2 = 0;
	DWORD dwD3DTSS_ALPHAOP = 0;
	DWORD dwD3DTSS_ALPHAARG1 = 0;
	DWORD dwD3DTSS_ALPHAARG2 = 0;

	IDirect3DDevice9_GetRenderState(device, D3DRS_SRCBLEND, &dwD3DRS_SRCBLEND);
	IDirect3DDevice9_GetRenderState(device, D3DRS_DESTBLEND, &dwD3DRS_DESTBLEND);
	IDirect3DDevice9_GetRenderState(device, D3DRS_ALPHABLENDENABLE, &dwD3DRS_ALPHABLENDENABLE);
	IDirect3DDevice9_GetRenderState(device, D3DRS_BLENDOP, &dwD3DRS_BLENDOP);

	IDirect3DDevice9_GetRenderState(device, D3DRS_LIGHTING, &dwD3DRS_LIGHTING);
	IDirect3DDevice9_GetRenderState(device, D3DRS_ZENABLE, &dwD3DRS_ZENABLE);
	IDirect3DDevice9_GetRenderState(device, D3DRS_ZWRITEENABLE, &dwD3DRS_ZWRITEENABLE);
	IDirect3DDevice9_GetRenderState(device, D3DRS_CULLMODE, &dwD3DRS_CULLMODE);
	IDirect3DDevice9_GetRenderState(device, D3DRS_SCISSORTESTENABLE, &dwD3DRS_SCISSORTESTENABLE);

	IDirect3DDevice9_GetSamplerState(device, 0, D3DSAMP_ADDRESSU, &dwD3DSAMP_ADDRESSU);
	IDirect3DDevice9_GetSamplerState(device, 0, D3DSAMP_ADDRESSV, &dwD3DSAMP_ADDRESSV);
	IDirect3DDevice9_GetSamplerState(device, 0, D3DSAMP_MAGFILTER, &dwD3DSAMP_MAGFILTER);
	IDirect3DDevice9_GetSamplerState(device, 0, D3DSAMP_MINFILTER, &dwD3DSAMP_MINFILTER);

	IDirect3DDevice9_GetTextureStageState(device, 0, D3DTSS_COLOROP, &dwD3DTSS_COLOROP);
	IDirect3DDevice9_GetTextureStageState(device, 0, D3DTSS_COLORARG1, &dwD3DTSS_COLORARG1);
	IDirect3DDevice9_GetTextureStageState(device, 0, D3DTSS_COLORARG2, &dwD3DTSS_COLORARG2);
	IDirect3DDevice9_GetTextureStageState(device, 0, D3DTSS_ALPHAOP, &dwD3DTSS_ALPHAOP);
	IDirect3DDevice9_GetTextureStageState(device, 0, D3DTSS_ALPHAARG1, &dwD3DTSS_ALPHAARG1);
	IDirect3DDevice9_GetTextureStageState(device, 0, D3DTSS_ALPHAARG2, &dwD3DTSS_ALPHAARG2);


	/* blend state */
	IDirect3DDevice9_SetRenderState(device, D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	IDirect3DDevice9_SetRenderState(device, D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	IDirect3DDevice9_SetRenderState(device, D3DRS_ALPHABLENDENABLE, TRUE);
	IDirect3DDevice9_SetRenderState(device, D3DRS_BLENDOP, D3DBLENDOP_ADD);

	/* render state */
	IDirect3DDevice9_SetRenderState(device, D3DRS_LIGHTING, FALSE);
	IDirect3DDevice9_SetRenderState(device, D3DRS_ZENABLE, FALSE);
	IDirect3DDevice9_SetRenderState(device, D3DRS_ZWRITEENABLE, FALSE);
	IDirect3DDevice9_SetRenderState(device, D3DRS_CULLMODE, D3DCULL_NONE);
	IDirect3DDevice9_SetRenderState(device, D3DRS_SCISSORTESTENABLE, TRUE);

	/* sampler state */
	IDirect3DDevice9_SetSamplerState(device, 0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	IDirect3DDevice9_SetSamplerState(device, 0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
	IDirect3DDevice9_SetSamplerState(device, 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	IDirect3DDevice9_SetSamplerState(device, 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);

	/* texture stage state */
	IDirect3DDevice9_SetTextureStageState(device, 0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	IDirect3DDevice9_SetTextureStageState(device, 0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	IDirect3DDevice9_SetTextureStageState(device, 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	IDirect3DDevice9_SetTextureStageState(device, 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	IDirect3DDevice9_SetTextureStageState(device, 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	IDirect3DDevice9_SetTextureStageState(device, 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);


	nk_d3d9_render(NK_ANTI_ALIASING_ON);


	/* blend state */
	IDirect3DDevice9_SetRenderState(device, D3DRS_SRCBLEND, dwD3DRS_SRCBLEND);
	IDirect3DDevice9_SetRenderState(device, D3DRS_DESTBLEND, dwD3DRS_DESTBLEND);
	IDirect3DDevice9_SetRenderState(device, D3DRS_ALPHABLENDENABLE, dwD3DRS_ALPHABLENDENABLE);
	IDirect3DDevice9_SetRenderState(device, D3DRS_BLENDOP, dwD3DRS_BLENDOP);

	/* render state */
	IDirect3DDevice9_SetRenderState(device, D3DRS_LIGHTING, dwD3DRS_LIGHTING);
	IDirect3DDevice9_SetRenderState(device, D3DRS_ZENABLE, dwD3DRS_ZENABLE);
	IDirect3DDevice9_SetRenderState(device, D3DRS_ZWRITEENABLE, dwD3DRS_ZWRITEENABLE);
	IDirect3DDevice9_SetRenderState(device, D3DRS_CULLMODE, dwD3DRS_CULLMODE);
	IDirect3DDevice9_SetRenderState(device, D3DRS_SCISSORTESTENABLE, dwD3DRS_SCISSORTESTENABLE);

	/* sampler state */
	IDirect3DDevice9_SetSamplerState(device, 0, D3DSAMP_ADDRESSU, dwD3DSAMP_ADDRESSU);
	IDirect3DDevice9_SetSamplerState(device, 0, D3DSAMP_ADDRESSV, dwD3DSAMP_ADDRESSV);
	IDirect3DDevice9_SetSamplerState(device, 0, D3DSAMP_MAGFILTER, dwD3DSAMP_MAGFILTER);
	IDirect3DDevice9_SetSamplerState(device, 0, D3DSAMP_MINFILTER, dwD3DSAMP_MINFILTER);

	/* texture stage state */
	IDirect3DDevice9_SetTextureStageState(device, 0, D3DTSS_COLOROP, dwD3DTSS_COLOROP);
	IDirect3DDevice9_SetTextureStageState(device, 0, D3DTSS_COLORARG1, dwD3DTSS_COLORARG1);
	IDirect3DDevice9_SetTextureStageState(device, 0, D3DTSS_COLORARG2, dwD3DTSS_COLORARG2);
	IDirect3DDevice9_SetTextureStageState(device, 0, D3DTSS_ALPHAOP, dwD3DTSS_ALPHAOP);
	IDirect3DDevice9_SetTextureStageState(device, 0, D3DTSS_ALPHAARG1, dwD3DTSS_ALPHAARG1);
	IDirect3DDevice9_SetTextureStageState(device, 0, D3DTSS_ALPHAARG2, dwD3DTSS_ALPHAARG2);


	for (std::vector<NKBase*>::iterator iter = m_vecModule.begin(); iter != m_vecModule.end(); ++iter)
	{
		(*iter)->SafeRenderEnd();
	}
}

int NuklearUI::HandleEvent(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam, D3DPRESENT_PARAMETERS* present)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_SIZE:
		if (d3d9.device)
		{
			UINT width = LOWORD(lparam);
			UINT height = HIWORD(lparam);
			if (width != 0 && height != 0 &&
				(width != present->BackBufferWidth || height != present->BackBufferHeight))
			{
				HRESULT hr;
				nk_d3d9_release();
				present->BackBufferWidth = width;
				present->BackBufferHeight = height;
				hr = IDirect3DDevice9_Reset(d3d9.device, present);
				NK_ASSERT(SUCCEEDED(hr));
				nk_d3d9_resize(width, height);
			}
		}
		break;

	}
	return nk_d3d9_handle_event(wnd, msg, wparam, lparam);
}
#elif _DX7
void NuklearUI::Render(IDirect3DDevice7* pdevice)
{
	nk_d3d7_render(NK_ANTI_ALIASING_ON);

	for (std::vector<NKBase*>::iterator iter = m_vecModule.begin(); iter != m_vecModule.end(); ++iter)
	{
		(*iter)->SafeRenderEnd();
	}
}
int NuklearUI::HandleEvent(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_SIZE:
		if (d3d7.device)
		{
			UINT width = LOWORD(lparam);
			UINT height = HIWORD(lparam);
			if (width != 0 && height != 0)
			{
				nk_d3d7_resize(width, height);
			}
		}
		break;
	}
	return nk_d3d7_handle_event(wnd, msg, wparam, lparam);
}

bool NuklearUI::LoadSpriteData(IDirectDrawSurface7* sprite, int width, int height, int sliceSizeX, int sliceSizeY, int countX, int countY)
{
	int index = 0;
	if (sliceSizeX && sliceSizeY && countX && countY)
	{
		for (int x = 0; x < countX; ++x)
		{
			for (int y = 0; y < countY; ++y)
			{
				uint16_t region[4];
				region[0] = sliceSizeX * x;
				region[1] = sliceSizeY * y;
				region[2] = sliceSizeX * (x + 1);
				region[3] = sliceSizeY * (y + 1);
				AddImage(index++, sprite, width, height, region);
			}
		}
	}
	else
	{
		AddImage(index, sprite);
	}

	return true;
}
bool NuklearUI::ReadImageFile(const char* filename, IDirectDrawSurface7** pTexture)
{
	int width, height, channels;
	unsigned char* data = stbi_load(filename, &width, &height, &channels, 4); // 4는 RGBA로 로드하라는 의미
	if (!data)
		return false;

	// 텍스처 생성
	DDSURFACEDESC2 ddsd;
	ZeroMemory(&ddsd, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT | DDSD_PIXELFORMAT;
	ddsd.ddsCaps.dwCaps = DDSCAPS_TEXTURE;
	ddsd.dwWidth = width;
	ddsd.dwHeight = height;
	ddsd.ddpfPixelFormat.dwSize = sizeof(DDPIXELFORMAT);
	ddsd.ddpfPixelFormat.dwFlags = DDPF_RGB | DDPF_ALPHAPIXELS;
	ddsd.ddpfPixelFormat.dwRGBBitCount = 32;
	ddsd.ddpfPixelFormat.dwRBitMask = 0x00FF0000;
	ddsd.ddpfPixelFormat.dwGBitMask = 0x0000FF00;
	ddsd.ddpfPixelFormat.dwBBitMask = 0x000000FF;
	ddsd.ddpfPixelFormat.dwRGBAlphaBitMask = 0xFF000000;

	HRESULT hr = d3d7.dd->CreateSurface(&ddsd, pTexture, NULL);
	if (FAILED(hr)) {
		stbi_image_free(data);
		return false;
	}

	// 텍스처에 이미지 데이터 복사
	DDSURFACEDESC2 lockedSurface;
	ZeroMemory(&lockedSurface, sizeof(lockedSurface));
	lockedSurface.dwSize = sizeof(lockedSurface);
	if (FAILED((*pTexture)->Lock(NULL, &lockedSurface, 0, NULL))) {
		stbi_image_free(data);
		return false;
	}

	BYTE* dest = (BYTE*)lockedSurface.lpSurface;
	for (int y = 0; y < height; y++) {
		memcpy(dest + y * lockedSurface.lPitch, data + y * width * 4, width * 4);
	}
	(*pTexture)->Unlock(NULL);

	stbi_image_free(data);
	return true;
}
#endif
void NuklearUI::Add(NKBase* type)
{
	NKBase* base = type;
	base->Initialize(this);
	while (true)
	{
		std::map<unsigned int, NKBase*>::iterator it = m_mapModuleID.find(++m_primaryIDCheck);
		if (it != m_mapModuleID.end()) {
			continue;
		}
		else
		{
			--m_primaryIDCheck;
			break;
		}
	}

	base->SetPrimaryID(m_primaryIDCheck++);
	if (m_primaryIDCheck == 4294967295)
	{
		m_primaryIDCheck = 0;
	}
	base->SetNuklearIndex(m_vecModule.size());


	char primaryName[256] = { 0, };
	strcpy_s(primaryName, base->GetPrimaryName());
	int idx = 1;
	while (true)
	{
		auto it = m_mapModuleName.find(primaryName);
		if (it != m_mapModuleName.end()) {
			sprintf_s(primaryName, "%s%d", base->GetPrimaryName(), idx++);
		}
		else {
			base->SetPrimaryName(primaryName);
			break;
		}
	}

	if (base->GetType() == eWINDOW)
	{
		m_vecObject.push_back(base);
	}
	m_vecModule.push_back(base);
	m_mapModuleID.insert(std::make_pair(base->GetPrimaryID(), base));
	m_mapModuleName.insert(std::make_pair(base->GetPrimaryName(), base));
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

		for (int i = pBase->GetNuklearIndex(); i < m_vecModule.size(); ++i)
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
			parent->RemoveChild(pBase);
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
	std::map<const char*, NKBase*>::iterator it = m_mapModuleName.find(name);
	if (it != m_mapModuleName.end())
	{
		NKBase* pBase = it->second;

		unsigned int id = pBase->GetPrimaryID();

		m_mapModuleID.erase(id);
		m_mapModuleName.erase(name);
		m_vecModule.erase(m_vecModule.begin() + pBase->GetNuklearIndex());

		for (int i = pBase->GetNuklearIndex(); i < m_vecModule.size(); ++i)
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
			parent->RemoveChild(pBase);
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

		for (int i = obj->GetNuklearIndex(); i < m_vecModule.size(); ++i)
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
			parent->RemoveChild(obj);
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

		for (int i = pBase->GetNuklearIndex(); i < m_vecModule.size(); ++i)
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
			parent->RemoveChild(pBase);
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

#ifdef _DX9
void NuklearUI::AddImage(int SID, IDirect3DTexture9* texture)
{
	struct nk_image img;
	memset(&img, 0, sizeof(img));
	img.handle = nk_handle_ptr(texture);

	std::pair<int, struct nk_image> pairData = std::make_pair(SID, img);
	m_mapImage.insert(pairData);
}

void NuklearUI::AddImage(int SID, IDirect3DTexture9* texture, uint16_t width, uint16_t height, uint16_t region[])
{
	struct nk_image img;
	memset(&img, 0, sizeof(img));
	img.handle = nk_handle_ptr(texture);

	img.w = width;
	img.h = height;

	img.region[0] = region[0];
	img.region[1] = region[1];
	img.region[2] = region[2];
	img.region[3] = region[3];

	std::pair<int, struct nk_image> pairData = std::make_pair(SID, img);
	m_mapImage.insert(pairData);
}
#elif _DX7
void NuklearUI::AddImage(int SID, IDirectDrawSurface7* texture)
{
	struct nk_image img;
	memset(&img, 0, sizeof(img));
	img.handle = nk_handle_ptr(texture);

	std::pair<int, struct nk_image> pairData = std::make_pair(SID, img);
	m_mapImage.insert(pairData);
}

void NuklearUI::AddImage(int SID, IDirectDrawSurface7* texture, uint16_t width, uint16_t height, uint16_t region[])
{
	struct nk_image img;
	memset(&img, 0, sizeof(img));
	img.handle = nk_handle_ptr(texture);

	img.w = width;
	img.h = height;

	img.region[0] = region[0];
	img.region[1] = region[1];
	img.region[2] = region[2];
	img.region[3] = region[3];

	std::pair<int, struct nk_image> pairData = std::make_pair(SID, img);
	m_mapImage.insert(pairData);
}
#endif

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

void NuklearUI::LoadLuaFile(const char* filePath)
{
#ifdef _DEBUG
	memset(m_filePath, 0, sizeof(m_filePath));
	strcpy_s(m_filePath, filePath);
	if (luaL_dofile(m_lua, m_filePath) != LUA_OK) {
		std::cerr << lua_tostring(m_lua, -1) << std::endl;
	}
#else
	if (luaL_dofile(m_lua, filePath) != LUA_OK) {
		std::cerr << lua_tostring(m_lua, -1) << std::endl;
	}
#endif // _DEBUG

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
		fprintf(stderr, "%s 함수 호출 실패: %s\n", functionName, lua_tostring(m_lua, -1));
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
			func(args);  // 인수를 사용하여 함수 호출
		}
	}
	catch (const luabridge::LuaException& e) {
#ifdef _DEBUG
		std::cerr << "LuaException: " << e.what() << std::endl;
#endif // _DEBUG
		return false;
	}
	return true;
}

void NuklearUI::DebugLoadLuaFile(const char* filePath)
{
	if (luaL_dofile(m_lua, filePath) != LUA_OK) {
		std::cerr << lua_tostring(m_lua, -1) << std::endl;
	}
}

void NuklearUI::RegisterBase()
{
	luabridge::getGlobalNamespace(m_lua)
		.beginClass<NuklearUI>("NuklearUI")
		.addFunction("Add", &NuklearUI::Add)
		.endClass();

	luabridge::push(m_lua, this);
	lua_setglobal(m_lua, "system");

	luabridge::getGlobalNamespace(m_lua)
		.beginClass<NKBase>("NKBase")
		.addFunction("SetActive", &NKBase::SetActive)
		.addFunction("AddChild", &NKBase::LAddChild)
		.addFunction("RemoveChild", &NKBase::LRemoveChild)
		.addFunction("SetPivot", &NKBase::SetPivot)
		.addFunction("SetPosition", &NKBase::SetPosition)
		.addFunction("SetSize", &NKBase::SetSize)
		.addFunction("SetBackground", &NKBase::SetBackground)
		.addFunction("SetPrimaryName", &NKBase::SetPrimaryName)
		.endClass()
		.deriveClass<NKWindow, NKBase>("NKWindow")
		.endClass()
		.deriveClass<NKSpace, NKBase>("NKSpace")
		.endClass()
		.deriveClass<NKGroup, NKBase>("NKGroup")
		.endClass()
		.deriveClass<NKPopup, NKBase>("NKPopup")
		.endClass()
		.deriveClass<NKCombo, NKBase>("NKCombo")
		.addFunction("SetComboName", &NKCombo::SetComboName)
		.addFunction("SetLabelSize", &NKCombo::SetLabelSize)
		.endClass()
		.deriveClass<NKComboItem, NKBase>("NKComboItem")
		.addFunction("SetComboName", &NKComboItem::SetComboName)
		.addFunction("RegistFunction", &NKComboItem::RegistFunction)
		.endClass()
		.deriveClass<NKButton, NKBase>("NKButton")
		.addFunction("SetComboName", &NKButton::SetButtonName)
		.addFunction("RegistFunction", &NKButton::RegistFunction)
		.endClass()
		.deriveClass<NKEdit, NKBase>("NKEdit")
		.addFunction("Clear", &NKEdit::Clear)
		.addFunction("RegistFunction", &NKEdit::RegistFunction)
		.endClass()
		.deriveClass<NKImage, NKBase>("NKImage")
		.endClass()
		.deriveClass<NKLabel, NKBase>("NKLabel")
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
		.endClass();
}
