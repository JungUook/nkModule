#pragma once
#ifndef DX7Renderer_H_
#define DX7Renderer_H_

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_DEFAULT_FONT
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_STANDARD_VARARGS_h__
#define NK_INCLUDE_DEFAULT_ALLOCATOR_h__
#define NK_BUTTON_TRIGGER_ON_RELEASE
#include <nuklear.h>

#define WIN32_LEAN_AND_MEAN
#include <ddraw.h>
#include <d3d.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

typedef struct IDirectDrawSurface7 IDirectDrawSurface7;

struct nk_d3d7_vertex {
	float x, y, z, rhw;
	DWORD col;
	float u, v;
};

struct nk_d3d7_obj {
	struct nk_context ctx;
	struct nk_font_atlas atlas;
	struct nk_buffer cmds;
	struct nk_font* font;
	float original_height;

	struct nk_draw_null_texture tex_null;

	LPDIRECTDRAW7 dd;
	LPDIRECT3DDEVICE7 device;
	LPDIRECTDRAWSURFACE7 font_texture;
	int bInit;

	nk_rune lastUnicode;
	int keyDown;
};

class DX7Renderer
{
public:
	enum eLang {
		KOR = 0,
		JPN = 1,
		TWA = 2,
		CHI = 3
	};

public:
	DX7Renderer();
	~DX7Renderer();

public:
	static void nk_d3d7_clipboard_copy(nk_handle usr, const char* text, int len);
	static void nk_d3d7_clipboard_paste(nk_handle usr, struct nk_text_edit* edit);

public:
	struct nk_context* nk_d3d7_init(LPDIRECTDRAW7 pdd, LPDIRECT3DDEVICE7 pdevice);
	void nk_d3d7_font_stash_begin(struct nk_font_atlas** atlas, CHAR* path, int lang = 0);
	void nk_d3d7_font_stash_end(void);
	int nk_d3d7_handle_event(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	void nk_d3d7_render(enum nk_anti_aliasing antialiasing);
	void nk_d3d7_render_skip();
	void nk_d3d7_release(void);
	void nk_d3d7_resize(int width, int height);
	void nk_d3d7_shutdown(void);
	void nk_d3d7_create_state(void);
	void nk_d3d7_create_state_restore(void);
	void nk_d3d7_create_font_texture();


private:
	DWORD dwD3DRS_SRCBLEND;
	DWORD dwD3DRS_DESTBLEND;
	DWORD dwD3DRS_ALPHABLENDENABLE;

	DWORD dwD3DRS_LIGHTING;
	DWORD dwD3DRS_ZENABLE;
	DWORD dwD3DRS_ZWRITEENABLE;
	DWORD dwD3DRS_CULLMODE;

	DWORD dwD3DSAMP_ADDRESSU;
	DWORD dwD3DSAMP_ADDRESSV;
	DWORD dwD3DSAMP_MAGFILTER;
	DWORD dwD3DSAMP_MINFILTER;

	DWORD dwD3DTSS_COLOROP;
	DWORD dwD3DTSS_COLORARG1;
	DWORD dwD3DTSS_COLORARG2;
	DWORD dwD3DTSS_ALPHAOP;
	DWORD dwD3DTSS_ALPHAARG1;
	DWORD dwD3DTSS_ALPHAARG2;

public:
	nk_d3d7_obj d3d7;
};

#endif //DX7Renderer_H_