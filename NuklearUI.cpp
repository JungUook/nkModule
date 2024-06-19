#include "pch.h"

#ifndef NK_ASSERT
#include <assert.h>
#define NK_ASSERT(expr) assert(expr)
#endif

#define NK_IMPLEMENTATION
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#include "NuklearUI.h"

#ifdef _DX9
#define NK_D3D9_IMPLEMENTATION
#include <nuklear_d3d9.h>
#elif _DX7
#define NK_D3D7_IMPLEMENTATION
#include <nuklear_d3d7.h>
#endif

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <shlobj.h>

#include <cereal/types/vector.hpp>
#include <cereal/types/array.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/types/polymorphic.hpp>

#include "UiLibrary.h"
#include "NuklearEditor.h"


struct nk_image g_img;
NuklearEditor g_editor;

NuklearUI::NuklearUI()
{
	m_ctx = nullptr;
	m_font = nullptr;
	m_bMouseHovering = false;
	m_bEditActive = false;
	m_original_height = 0;
	m_primaryIDCheck = 0;
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
	cfg.oversample_h = 1; // ¼öÆò ¿À¹ö»ùÇÃ¸µ
	cfg.oversample_v = 1; // ¼öÁ÷ ¿À¹ö»ùÇÃ¸µ

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
	cfg.oversample_h = 1; // ¼öÆò ¿À¹ö»ùÇÃ¸µ
	cfg.oversample_v = 1; // ¼öÁ÷ ¿À¹ö»ùÇÃ¸µ

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
	default:
		sprintf_s(fontPath, "%s\\gulim.ttc", path);
		cfg.range = nk_font_korean_glyph_ranges();

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

	char* buffer = new char[(unsigned int)file_size];
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

#ifdef _NKDEBUG
	DebugLoadLuaFile(m_filePath);
	RunFunction("Modify");
#endif // _NKDEBUG

	for (std::vector<NKBase*>::iterator iter = m_vecObject.begin(); iter != m_vecObject.end(); ++iter)
	{
		(*iter)->SafeRenderStart();
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

	g_editor.EditorLayout(m_ctx, debugRect);
}

void NuklearUI::ErrorPopup(const char* content)
{
	g_editor.OpenErrorPopup(content);
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
	ReleaseRenderData();
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
		for (int y = 0; y < countY; ++y)
		{
			for (int x = 0; x < countX; ++x)
			{
				uint16_t region[4] = { 0, };
				region[0] = sliceSizeX * x;
				region[1] = sliceSizeY * y;
				region[2] = sliceSizeX;
				region[3] = sliceSizeY;
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
	unsigned char* data = stbi_load(filename, &width, &height, &channels, 4); // 4´Â RGBA·Î ·ÎµåÇÏ¶ó´Â ÀÇ¹Ì
	if (!data)
		return false;

	// ÅØ½ºÃ³ »ý¼º
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

	// ÅØ½ºÃ³¿¡ ÀÌ¹ÌÁö µ¥ÀÌÅÍ º¹»ç
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
nk_flags NuklearUI::IMEInputSystem(char* buffer, int max, int* len, nk_flags flag, nk_plugin_filter filter)
{
	nk_flags result = nk_edit_string_zero_terminated(m_ctx, flag, buffer, max, filter);

	if (result & NK_EDIT_ACTIVE) {
		IMEInputSystem(buffer, len);
	}
	return result;
}
void NuklearUI::IMEInputSystem(char* memory, int* len)
{
	if (d3d7.ctx.text_edit.bComposition) {
		nk_hash hash;
		struct nk_text_edit* edit;
		struct nk_window* win;
		win = d3d7.ctx.current;
		hash = win->edit.seq;
		edit = &d3d7.ctx.text_edit;

		if (edit->cursor <= 0) {
			return;
		}

		edit->select_start = edit->cursor - 1;
		edit->select_end = edit->cursor;

		win->edit.sel_start = edit->select_start;
		win->edit.sel_end = edit->select_end;
	}
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

NKBase* NuklearUI::RegistUI(const char* classname, NKBase* loadPtr)
{
	NKBase* pBase = m_factory.create(classname, m_ctx, this);
	(*pBase) = *loadPtr;
	pBase->Load(m_ctx, this);

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
struct nk_rect* NuklearUI::GetViewport()
{
#ifdef _DX9
	m_viewRect = nk_rect(0, 0, d3d9.viewport.Width, d3d9.viewport.Height);

#elif _DX7
	D3DVIEWPORT7 viewport;
	d3d7.device->GetViewport(&viewport);
	m_viewRect = nk_rect(0,0, (float)viewport.dwWidth, (float)viewport.dwHeight);
#endif // _DX9

	return &m_viewRect;
}
void NuklearUI::SetPrimary(NKBase* pBase)
{
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

	pBase->SetPrimaryID(m_primaryIDCheck);
	if (m_primaryIDCheck == 4294967295)
	{
		m_primaryIDCheck = 0;
	}
	pBase->SetNuklearIndex(m_vecModule.size());

	char primaryName[256] = { 0, };
	sprintf_s(primaryName, "%s%ld", pBase->GetPrimaryName(), m_primaryIDCheck++);
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
void NuklearUI::Add(NKBase* type)
{
	NKBase* base = type;
	
	if (base->GetType() == eWINDOW)
	{
		base->Initialize(this);
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

#ifdef _DX7
void NuklearUI::Register_spr(sprLoader* pSpr)
{
	m_sprLoader = pSpr;
}
void NuklearUI::OpenFileDialog()
{
	OPENFILENAMEW ofn;
	const size_t buffer_size = 65536; // ÃæºÐÈ÷ Å« ¹öÆÛ Å©±â
	wchar_t* szFile = new wchar_t[buffer_size];
	ZeroMemory(szFile, buffer_size * sizeof(wchar_t));
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = buffer_size;
	ofn.lpstrFilter = L"All Files\0*.*\0SPR Files\0*.spr;*.Spr;*.SPR\0";
	ofn.nFilterIndex = 2; // ±âº» ¼±ÅÃÀ» SPR Files·Î ¼³Á¤
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
			if (extension == L".spr") {
				int size_needed = WideCharToMultiByte(CP_UTF8, 0, filePath.c_str(), -1, NULL, 0, NULL, NULL);
				char* result = new char[size_needed];
				WideCharToMultiByte(CP_UTF8, 0, filePath.c_str(), -1, result, size_needed, NULL, NULL);
				LoadSprFile(result);
				delete[] result;
			}
			p += wcslen(p) + 1;
		}

		// If only one file is selected, GetOpenFileNameW does not add the directory separately
		if (directory.length() > 0 && *p == '\0') {
			std::filesystem::path path(directory);
			std::wstring extension = path.extension().wstring();
			std::transform(extension.begin(), extension.end(), extension.begin(), towlower);
			if (extension == L".spr") {
				int size_needed = WideCharToMultiByte(CP_UTF8, 0, directory.c_str(), -1, NULL, 0, NULL, NULL);
				char* result = new char[size_needed];
				WideCharToMultiByte(CP_UTF8, 0, directory.c_str(), -1, result, size_needed, NULL, NULL);
				LoadSprFile(result);
				delete[] result;
			}
		}
	}

	delete[] szFile; // µ¿ÀûÀ¸·Î ÇÒ´çÇÑ ¸Þ¸ð¸® ÇØÁ¦
}
void NuklearUI::LoadSprFile(const char* filename)
{
	sprData* pData = m_sprLoader->LoadSprite(filename);
	m_mapSpr.insert(std::make_pair(filename, pData));
}
void NuklearUI::GetSprite(const char* filename, int index, struct nk_image& outimg, bool bImmortal)
{
	auto it = m_mapSpr.find(filename);
	if (it != m_mapSpr.end()) {
		sprData* pSpr = (*it).second;
		bool bSuccess = RegisterRenderData(pSpr, bImmortal);
		if (!bSuccess) {
			throw;
		}
		int totalSprites = pSpr->GetSpr()->GetXCount() * pSpr->GetSpr()->GetYCount();

		if (index >= 0 && index < totalSprites)	{

			int x = index % pSpr->GetSpr()->GetXCount();
			int y = index % pSpr->GetSpr()->GetYCount();

			struct nk_image img;
			memset(&img, 0, sizeof(img));
			img.handle = nk_handle_ptr(pSpr->GetSurface());

			img.w = pSpr->GetSpr()->GetHres();
			img.h = pSpr->GetSpr()->GetVres();

			img.region[0] = pSpr->GetSpr()->GetXSize() * x;
			img.region[1] = pSpr->GetSpr()->GetYSize() * y;
			img.region[2] = pSpr->GetSpr()->GetXSize();
			img.region[3] = pSpr->GetSpr()->GetYSize();

			outimg = img;
		}
	}
}
void NuklearUI::GetImage(const char* filename, struct nk_image& outimg, bool bImmortal)
{
	auto it = m_mapSpr.find(filename);
	if (it != m_mapSpr.end()) {
		sprData* pSpr = (*it).second;
		bool bSuccess = RegisterRenderData(pSpr, bImmortal);
		if (!bSuccess) {
			throw;
		}
		struct nk_image img;
		memset(&img, 0, sizeof(img));
		img.handle = nk_handle_ptr(pSpr->GetSurface());
		outimg = img;
	}
}
std::map<std::string, sprData*>* NuklearUI::GetSprMap()
{
	return &m_mapSpr;
}
bool NuklearUI::RegisterRenderData(sprData* pData, bool bImmortal)
{
	if (pData->GetSurface() == nullptr) {
		pData->LoadTexture(d3d7.dd);
		if (bImmortal) {
			m_vecImmortalRenderData.push_back(pData);
		}
		else {
			m_vecRenderData.push_back(pData);
		}
	}

	if (pData->GetSurface()) {
		return true;
	}
	else {
		return false;
	}
}
void NuklearUI::ReleaseRenderData()
{
	for (auto it = m_vecRenderData.begin(); it != m_vecRenderData.end();) {
		sprData* pData = *it;
		pData->Release();
		it = m_vecRenderData.erase(it);
	}
}
#endif // _DX7

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
		//.addFunction("SetPivot", &NKBase::SetPivot)
		//.addFunction("SetPosition", &NKBase::SetPosition)
		//.addFunction("SetSize", &NKBase::SetSize)
		//.addFunction("SetBackground", &NKBase::SetBackground)
		.addFunction("SetPrimaryName", &NKBase::SetPrimaryName)
		.endClass()
		.deriveClass<NKWindow, NKBase>("NKWindow")
		.endClass()
		.deriveClass<NKSpace, NKBase>("NKSpace")
		.addFunction("SetLayout", &NKSpace::SetLayout)
		.addFunction("SetCols", &NKSpace::SetCols)
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
		.addFunction("RegistFunction", &NKComboItem::RegistFunction)
		.endClass()
		.deriveClass<NKButton, NKBase>("NKButton")
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
		.deriveClass<NKCheckbox, NKBase>("NKCheckbox")
		.addFunction("SetLabel", &NKCheckbox::SetLabel)
		.addFunction("SetChecked", &NKCheckbox::SetChecked)
		.addFunction("IsChecked", &NKCheckbox::IsChecked)
		.endClass()
		.deriveClass<NKSlider, NKBase>("NKSlider")
		.addFunction("SetRange", &NKSlider::SetRange)
		.addFunction("SetValue", &NKSlider::SetValue)
		.addFunction("GetValue", &NKSlider::GetValue)
		.endClass()
		.deriveClass<NKProgress, NKBase>("NKProgress")
		.addFunction("SetProgress", &NKProgress::SetProgress)
		.addFunction("GetProgress", &NKProgress::GetProgress)
		.endClass()
		.deriveClass<NKSelectable, NKBase>("NKSelectable")
		.addFunction("SetLabel", &NKSelectable::SetLabel)
		.addFunction("SetSelected", &NKSelectable::SetSelected)
		.addFunction("IsSelected", &NKSelectable::IsSelected)
		.endClass()
		.deriveClass<NKTree, NKBase>("NKTree")
		.addFunction("SetLabel", &NKTree::SetLabel)
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
		.addFunction("SetLabel", &NKMenu::SetLabel)
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

void NuklearUI::SaveFile(const std::string& filename)
{
	std::vector<std::shared_ptr<NKBase>> vec;
	size_t size = m_vecObject.size();
	for (size_t i = 0; i < size; ++i) {
		NKBase* ptr = m_vecObject.at(i);
		SaveSwitch(vec, ptr);
	}


	std::ofstream os(filename);
	cereal::JSONOutputArchive archive(os);
	archive(CEREAL_NVP(vec));


	std::vector<NKBase*>::iterator it = m_vecObject.begin() + size;
	m_vecObject.erase(it, m_vecObject.end());
}

void NuklearUI::SaveSwitch(std::vector<std::shared_ptr<NKBase>>& vec, NKBase* ptr)
{
	eTypeUI eType = ptr->GetType();
	switch (eType)
	{
	case eWINDOW: {
		NKWindow* nkWindow = static_cast<NKWindow*>(ptr);
		NKWindow nWindow = *nkWindow;
		vec.push_back(std::make_shared<NKWindow>(nWindow));
		break;
	}
	case eSPACE: {
		NKSpace* nkSpace = static_cast<NKSpace*>(ptr);
		NKSpace* nSpace = new NKSpace(*nkSpace);
		vec.push_back(std::make_shared<NKSpace>(*nSpace));
		break;
	}
	case eGROUP: {
		NKGroup* nkGroup = static_cast<NKGroup*>(ptr);
		NKGroup* nGroup = new NKGroup(*nkGroup);
		vec.push_back(std::make_shared<NKGroup>(*nGroup));
		break;
	}
	case ePOPUP: {
		NKPopup* nkPopup = static_cast<NKPopup*>(ptr);
		NKPopup* nPopup = new NKPopup(*nkPopup);
		vec.push_back(std::make_shared<NKPopup>(*nPopup));
		break;
	}
	case eCOMBO: {
		NKCombo* nkCombo = static_cast<NKCombo*>(ptr);
		NKCombo* nCombo = new NKCombo(*nkCombo);
		vec.push_back(std::make_shared<NKCombo>(*nCombo));
		break;
	}
	case eBUTTON: {
		NKButton* nkButton = static_cast<NKButton*>(ptr);
		NKButton* nButton = new NKButton(*nkButton);
		vec.push_back(std::make_shared<NKButton>(*nButton));
		break;
	}
	case eEDIT: {
		NKEdit* nkEdit = static_cast<NKEdit*>(ptr);
		NKEdit* nEdit = new NKEdit(*nkEdit);
		vec.push_back(std::make_shared<NKEdit>(*nEdit));
		break;
	}
	case eIMAGE: {
		NKImage* nkImage = static_cast<NKImage*>(ptr);
		NKImage* nImage = new NKImage(*nkImage);
		vec.push_back(std::make_shared<NKImage>(*nImage));
		break;
	}
	case eLABEL: {
		NKLabel* nkLabel = static_cast<NKLabel*>(ptr);
		NKLabel* nLabel = new NKLabel(*nkLabel);
		vec.push_back(std::make_shared<NKLabel>(*nLabel));
		break;
	}
	case eCOMBO_ITEM: {
		NKComboItem* nkComboItem = static_cast<NKComboItem*>(ptr);
		NKComboItem* nComboItem = new NKComboItem(*nkComboItem);
		vec.push_back(std::make_shared<NKComboItem>(*nComboItem));
		break;
	}
	case eCHECKBOX: {
		NKCheckbox* nkCheckbox = static_cast<NKCheckbox*>(ptr);
		NKCheckbox* nCheckbox = new NKCheckbox(*nkCheckbox);
		vec.push_back(std::make_shared<NKCheckbox>(*nCheckbox));
		break;
	}
	case eSLIDER: {
		NKSlider* nkSlider = static_cast<NKSlider*>(ptr);
		NKSlider* nSlider = new NKSlider(*nkSlider);
		vec.push_back(std::make_shared<NKSlider>(*nSlider));
		break;
	}
	case ePROGRESS: {
		NKProgress* nkProgress = static_cast<NKProgress*>(ptr);
		NKProgress* nProgress = new NKProgress(*nkProgress);
		vec.push_back(std::make_shared<NKProgress>(*nProgress));
		break;
	}
	case eSELECTABLE: {
		NKSelectable* nkSelectable = static_cast<NKSelectable*>(ptr);
		NKSelectable* nSelectable = new NKSelectable(*nkSelectable);
		vec.push_back(std::make_shared<NKSelectable>(*nSelectable));
		break;
	}
	case eTREE: {
		NKTree* nkTree = static_cast<NKTree*>(ptr);
		NKTree* nTree = new NKTree(*nkTree);
		vec.push_back(std::make_shared<NKTree>(*nTree));
		break;
	}
	case eCHART: {
		NKChart* nkChart = static_cast<NKChart*>(ptr);
		NKChart* nChart = new NKChart(*nkChart);
		vec.push_back(std::make_shared<NKChart>(*nChart));
		break;
	}
	case eCOLOR_PICKER: {
		NKTooltip* nkTooltip = static_cast<NKTooltip*>(ptr);
		NKTooltip* nTooltip = new NKTooltip(*nkTooltip);
		vec.push_back(std::make_shared<NKTooltip>(*nTooltip));
		break;
	}
	case eTOOLTIP: {
		NKMenu* nkMenu = static_cast<NKMenu*>(ptr);
		NKMenu* nMenu = new NKMenu(*nkMenu);
		vec.push_back(std::make_shared<NKMenu>(*nMenu));
		break;
	}
	case eMENU: {
		NKScrollbar* nkScrollbar = static_cast<NKScrollbar*>(ptr);
		NKScrollbar* nScrollbar = new NKScrollbar(*nkScrollbar);
		vec.push_back(std::make_shared<NKScrollbar>(*nScrollbar));
		break;
	}
	case eSCROLLBAR: {
		NKColorPicker* nkColorPicker = static_cast<NKColorPicker*>(ptr);
		NKColorPicker* nColorPicker = new NKColorPicker(*nkColorPicker);
		vec.push_back(std::make_shared<NKColorPicker>(*nColorPicker));
		break;
	}
	case eSUPERSTYLE: {
		NKSuperStyleObject* nkSuperStyleObject = static_cast<NKSuperStyleObject*>(ptr);
		NKSuperStyleObject* nSuperStyleObject = new NKSuperStyleObject(*nkSuperStyleObject);
		vec.push_back(std::make_shared<NKSuperStyleObject>(*nSuperStyleObject));
		break;
	}
	default:
		break;
	}
}

void NuklearUI::LoadFile(const std::string& filename)
{
	std::vector<std::shared_ptr<NKBase>> vec;
	std::ifstream is(filename);
	cereal::JSONInputArchive archive(is);
	archive(CEREAL_NVP(vec));

	for (auto it = vec.begin(); it != vec.end(); ++it) {
		LoadNode(it->get());
	}
}

void NuklearUI::LoadNode(NKBase* pBase)
{
	NKBase* ptr = RegistUI(pBase->getClassName().c_str(), pBase);

	auto childrens = ptr->GetChildList();

	for (auto it = childrens->begin(); it != childrens->end(); ++it) {
		NKBase* pChild = *it;
		if (pChild) {
			LoadNode(pChild);
		}
	}
}
