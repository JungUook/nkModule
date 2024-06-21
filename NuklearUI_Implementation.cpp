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

#include "NuklearEditor.h"

NuklearEditor g_editor;

NuklearUI::NuklearUI()
{
	m_ctx = nullptr;
	m_font = nullptr;
	m_bMouseHovering = false;
	m_bEditActive = false;
	m_original_height = 0;
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
	const size_t buffer_size = 65536; // 충분히 큰 버퍼 크기
	wchar_t* szFile = new wchar_t[buffer_size];
	ZeroMemory(szFile, buffer_size * sizeof(wchar_t));
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = buffer_size;
	ofn.lpstrFilter = L"All Files\0*.*\0SPR Files\0*.spr;*.Spr;*.SPR\0";
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

	delete[] szFile; // 동적으로 할당한 메모리 해제
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

		if (index >= 0 && index < totalSprites) {

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


struct nk_rect* NuklearUI::GetViewport()
{
#ifdef _DX9
	m_viewRect = nk_rect(0, 0, d3d9.viewport.Width, d3d9.viewport.Height);

#elif _DX7
	D3DVIEWPORT7 viewport;
	d3d7.device->GetViewport(&viewport);
	m_viewRect = nk_rect(0, 0, (float)viewport.dwWidth, (float)viewport.dwHeight);
#endif // _DX9

	return &m_viewRect;
}