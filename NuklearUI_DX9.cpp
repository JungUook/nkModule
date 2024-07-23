#include "pch.h"
#include "NuklearUI.h"

#ifdef _DX9
#define NK_D3D9_IMPLEMENTATION
#include <nuklear_d3d9.h>

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

void NuklearUI::AddImage(int SID, IDirect3DTexture9* texture)
{
	struct nk_image img;
	memset(&img, 0, sizeof(img));
	img.handle = nk_handle_ptr(texture);
	img.color = nk_white;

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

	img.color = nk_white;

	std::pair<int, struct nk_image> pairData = std::make_pair(SID, img);
	m_mapImage.insert(pairData);
}
#endif