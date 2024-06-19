#ifndef NK_D3D7_H_
#define NK_D3D7_H_

#define WIN32_LEAN_AND_MEAN

typedef struct IDirectDrawSurface7 IDirectDrawSurface7;

NK_API struct nk_context* nk_d3d7_init(LPDIRECTDRAW7 pdd, LPDIRECT3DDEVICE7 pdevice);
NK_API void nk_d3d7_font_stash_begin(struct nk_font_atlas** atlas);
NK_API void nk_d3d7_font_stash_end(void);
NK_API int nk_d3d7_handle_event(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
NK_API void nk_d3d7_render(enum nk_anti_aliasing);
NK_API void nk_d3d7_release(void);
NK_API void nk_d3d7_resize(int width, int height);
NK_API void nk_d3d7_shutdown(void);
NK_API void nk_d3d7_create_state(void);

#endif

#ifdef NK_D3D7_IMPLEMENTATION

#define WIN32_LEAN_AND_MEAN
#include <ddraw.h>
#include <d3d.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

struct nk_d3d7_vertex {
    float x, y, z, rhw;
    DWORD col;
    float u, v;
};


DWORD dwD3DRS_SRCBLEND = 0;
DWORD dwD3DRS_DESTBLEND = 0;
DWORD dwD3DRS_ALPHABLENDENABLE = 0;

DWORD dwD3DRS_LIGHTING = 0;
DWORD dwD3DRS_ZENABLE = 0;
DWORD dwD3DRS_ZWRITEENABLE = 0;
DWORD dwD3DRS_CULLMODE = 0;

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

static struct {
    struct nk_context ctx;
    struct nk_font_atlas atlas;
    struct nk_buffer cmds;

    struct nk_draw_null_texture tex_null;

    LPDIRECTDRAW7 dd;
    LPDIRECT3DDEVICE7 device;
    LPDIRECTDRAWSURFACE7 font_texture;
    int bInit;

    nk_rune lastUnicode;
    int keyDown;
} d3d7;

static void
nk_d3d7_clipboard_copy(nk_handle usr, const char* text, int len)
{
    int wsize;

    (void)usr;
    if (!OpenClipboard(NULL)) {
        return;
    }

    wsize = MultiByteToWideChar(CP_UTF8, 0, text, len, NULL, 0);
    if (wsize) {
        HGLOBAL mem = GlobalAlloc(GMEM_MOVEABLE, (wsize + 1) * sizeof(wchar_t));
        if (mem) {
            wchar_t* wstr = (wchar_t*)GlobalLock(mem);
            if (wstr) {
                MultiByteToWideChar(CP_UTF8, 0, text, len, wstr, wsize);
                wstr[wsize] = 0;
                GlobalUnlock(mem);
                SetClipboardData(CF_UNICODETEXT, mem);
            }
        }
    }

    CloseClipboard();
}
static void
nk_d3d7_clipboard_paste(nk_handle usr, struct nk_text_edit* edit)
{
    HGLOBAL mem;
    SIZE_T size;
    LPCWSTR wstr;
    int utf8size;

    (void)usr;
    if (!IsClipboardFormatAvailable(CF_UNICODETEXT) && OpenClipboard(NULL)) {
        return;
    }

    mem = GetClipboardData(CF_UNICODETEXT);
    if (!mem) {
        CloseClipboard();
        return;
    }

    size = GlobalSize(mem) - 1;
    if (!size) {
        CloseClipboard();
        return;
    }

    wstr = (LPCWSTR)GlobalLock(mem);
    if (!wstr) {
        CloseClipboard();
        return;
    }

    utf8size = WideCharToMultiByte(CP_UTF8, 0, wstr, (int)size / sizeof(wchar_t), NULL, 0, NULL, NULL);
    if (utf8size) {
        char* utf8 = (char*)malloc(utf8size);
        if (utf8) {
            WideCharToMultiByte(CP_UTF8, 0, wstr, (int)size / sizeof(wchar_t), utf8, utf8size, NULL, NULL);
            nk_textedit_paste(edit, utf8, utf8size);
            free(utf8);
        }
    }

    GlobalUnlock(mem);
    CloseClipboard();
}

NK_API struct nk_context*
nk_d3d7_init(LPDIRECTDRAW7 pdd, LPDIRECT3DDEVICE7 pdevice) {
    d3d7.dd = pdd;
    d3d7.device = pdevice;
    d3d7.font_texture = NULL;

    nk_init_default(&d3d7.ctx, 0);
    d3d7.ctx.clip.copy = nk_d3d7_clipboard_copy;
    d3d7.ctx.clip.paste = nk_d3d7_clipboard_paste;
    d3d7.ctx.clip.userdata = nk_handle_ptr(0);

    nk_buffer_init_default(&d3d7.cmds);

    d3d7.bInit = 10000;
    d3d7.keyDown = 0;
    return &d3d7.ctx;
}

static void
nk_d3d7_create_font_texture()
{
    const void* image; int w, h;
    image = nk_font_atlas_bake(&d3d7.atlas, &w, &h, NK_FONT_ATLAS_RGBA32);
    if (!image) return;

    DDSURFACEDESC2 ddsd;
    memset(&ddsd, 0, sizeof(ddsd));
    ddsd.dwSize = sizeof(ddsd);
    ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT | DDSD_PIXELFORMAT;
    ddsd.ddsCaps.dwCaps = DDSCAPS_TEXTURE | DDSCAPS_VIDEOMEMORY;
    ddsd.dwWidth = w;
    ddsd.dwHeight = h;
    ddsd.ddpfPixelFormat.dwSize = sizeof(ddsd.ddpfPixelFormat);
    ddsd.ddpfPixelFormat.dwFlags = DDPF_RGB | DDPF_ALPHAPIXELS;
    ddsd.ddpfPixelFormat.dwRGBBitCount = 32;
    ddsd.ddpfPixelFormat.dwRBitMask = 0x00FF0000;
    ddsd.ddpfPixelFormat.dwGBitMask = 0x0000FF00;
    ddsd.ddpfPixelFormat.dwBBitMask = 0x000000FF;
    ddsd.ddpfPixelFormat.dwRGBAlphaBitMask = 0xFF000000;

    HRESULT hr = d3d7.dd->CreateSurface(&ddsd, &d3d7.font_texture, NULL);
    if (FAILED(hr)) return;

    ddsd.dwSize = sizeof(ddsd);
    hr = d3d7.font_texture->Lock(NULL, &ddsd, DDLOCK_WAIT | DDLOCK_SURFACEMEMORYPTR, NULL);
    if (FAILED(hr)) return;

    memcpy(ddsd.lpSurface, image, w * h * 4);

    d3d7.font_texture->Unlock(NULL);

    nk_font_atlas_end(&d3d7.atlas, nk_handle_ptr(d3d7.font_texture), &d3d7.tex_null);
}

NK_API void
nk_d3d7_font_stash_begin(struct nk_font_atlas** atlas) {
    nk_font_atlas_init_default(&d3d7.atlas);
    nk_font_atlas_begin(&d3d7.atlas);
    *atlas = &d3d7.atlas;
}

NK_API void
nk_d3d7_font_stash_end(void) {
    nk_d3d7_create_font_texture();

    if (d3d7.atlas.default_font)
        nk_style_set_font(&d3d7.ctx, &d3d7.atlas.default_font->handle);
}

NK_API void
nk_d3d7_create_state(void) {
    HRESULT hr;

    hr = d3d7.device->GetRenderState(D3DRENDERSTATE_SRCBLEND, &dwD3DRS_SRCBLEND);
    NK_ASSERT(SUCCEEDED(hr));
    hr = d3d7.device->GetRenderState(D3DRENDERSTATE_DESTBLEND, &dwD3DRS_DESTBLEND);
    NK_ASSERT(SUCCEEDED(hr));
    hr = d3d7.device->GetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, &dwD3DRS_ALPHABLENDENABLE);
    NK_ASSERT(SUCCEEDED(hr));

    hr = d3d7.device->GetRenderState(D3DRENDERSTATE_LIGHTING, &dwD3DRS_LIGHTING);
    NK_ASSERT(SUCCEEDED(hr));
    hr = d3d7.device->GetRenderState(D3DRENDERSTATE_ZENABLE, &dwD3DRS_ZENABLE);
    NK_ASSERT(SUCCEEDED(hr));
    hr = d3d7.device->GetRenderState(D3DRENDERSTATE_ZWRITEENABLE, &dwD3DRS_ZWRITEENABLE);
    NK_ASSERT(SUCCEEDED(hr));
    hr = d3d7.device->GetRenderState(D3DRENDERSTATE_CULLMODE, &dwD3DRS_CULLMODE);
    NK_ASSERT(SUCCEEDED(hr));


    hr = d3d7.device->GetTextureStageState(0, D3DTSS_ADDRESSU, &dwD3DSAMP_ADDRESSU); // U 좌표의 텍스처 래핑 모드를 가져옴
    NK_ASSERT(SUCCEEDED(hr));
    hr = d3d7.device->GetTextureStageState(0, D3DTSS_ADDRESSV, &dwD3DSAMP_ADDRESSV); // V 좌표의 텍스처 래핑 모드를 가져옴
    NK_ASSERT(SUCCEEDED(hr));
    hr = d3d7.device->GetTextureStageState(0, D3DTSS_MAGFILTER, &dwD3DSAMP_MAGFILTER); // 확대 필터를 가져옴
    NK_ASSERT(SUCCEEDED(hr));
    hr = d3d7.device->GetTextureStageState(0, D3DTSS_MINFILTER, &dwD3DSAMP_MINFILTER); // 축소 필터를 가져옴
    NK_ASSERT(SUCCEEDED(hr));

    /* Set texture stage state */
    hr = d3d7.device->GetTextureStageState(0, D3DTSS_COLOROP, &dwD3DTSS_COLOROP);
    NK_ASSERT(SUCCEEDED(hr));
    hr = d3d7.device->GetTextureStageState(0, D3DTSS_COLORARG1, &dwD3DTSS_COLORARG1);
    NK_ASSERT(SUCCEEDED(hr));
    hr = d3d7.device->GetTextureStageState(0, D3DTSS_COLORARG2, &dwD3DTSS_COLORARG2);
    NK_ASSERT(SUCCEEDED(hr));
    hr = d3d7.device->GetTextureStageState(0, D3DTSS_ALPHAOP, &dwD3DTSS_ALPHAOP);
    NK_ASSERT(SUCCEEDED(hr));
    hr = d3d7.device->GetTextureStageState(0, D3DTSS_ALPHAARG1, &dwD3DTSS_ALPHAARG1);
    NK_ASSERT(SUCCEEDED(hr));
    hr = d3d7.device->GetTextureStageState(0, D3DTSS_ALPHAARG2, &dwD3DTSS_ALPHAARG2);
    NK_ASSERT(SUCCEEDED(hr));




    hr = d3d7.device->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA);
    NK_ASSERT(SUCCEEDED(hr));
    hr = d3d7.device->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA);
    NK_ASSERT(SUCCEEDED(hr));
    hr = d3d7.device->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);
    NK_ASSERT(SUCCEEDED(hr));

    hr = d3d7.device->SetRenderState(D3DRENDERSTATE_LIGHTING, FALSE);
    NK_ASSERT(SUCCEEDED(hr));
    hr = d3d7.device->SetRenderState(D3DRENDERSTATE_ZENABLE, D3DZB_FALSE);
    NK_ASSERT(SUCCEEDED(hr));
    hr = d3d7.device->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, FALSE);
    NK_ASSERT(SUCCEEDED(hr));
    hr = d3d7.device->SetRenderState(D3DRENDERSTATE_CULLMODE, D3DCULL_NONE);
    NK_ASSERT(SUCCEEDED(hr));

    hr = d3d7.device->SetTextureStageState(0, D3DTSS_ADDRESSU, D3DTADDRESS_CLAMP); // U 좌표의 텍스처 래핑 모드를 가져옴
    NK_ASSERT(SUCCEEDED(hr));
    hr = d3d7.device->SetTextureStageState(0, D3DTSS_ADDRESSV, D3DTADDRESS_CLAMP); // V 좌표의 텍스처 래핑 모드를 가져옴
    NK_ASSERT(SUCCEEDED(hr));
    hr = d3d7.device->SetTextureStageState(0, D3DTSS_MAGFILTER, D3DTFG_LINEAR); // 확대 필터를 가져옴
    NK_ASSERT(SUCCEEDED(hr));
    hr = d3d7.device->SetTextureStageState(0, D3DTSS_MINFILTER, D3DTFN_LINEAR); // 축소 필터를 가져옴
    NK_ASSERT(SUCCEEDED(hr));

    /* Set texture stage state */
    hr = d3d7.device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
    NK_ASSERT(SUCCEEDED(hr));
    hr = d3d7.device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    NK_ASSERT(SUCCEEDED(hr));
    hr = d3d7.device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
    NK_ASSERT(SUCCEEDED(hr));
    hr = d3d7.device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
    NK_ASSERT(SUCCEEDED(hr));
    hr = d3d7.device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
    NK_ASSERT(SUCCEEDED(hr));
    hr = d3d7.device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
    NK_ASSERT(SUCCEEDED(hr));
}


NK_API void
nk_d3d7_create_state_restore(void) {
    HRESULT hr;

    hr = d3d7.device->SetRenderState(D3DRENDERSTATE_SRCBLEND, dwD3DRS_SRCBLEND);
    NK_ASSERT(SUCCEEDED(hr));
    hr = d3d7.device->SetRenderState(D3DRENDERSTATE_DESTBLEND, dwD3DRS_DESTBLEND);
    NK_ASSERT(SUCCEEDED(hr));
    hr = d3d7.device->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, dwD3DRS_ALPHABLENDENABLE);
    NK_ASSERT(SUCCEEDED(hr));

    hr = d3d7.device->SetRenderState(D3DRENDERSTATE_LIGHTING, dwD3DRS_LIGHTING);
    NK_ASSERT(SUCCEEDED(hr));
    hr = d3d7.device->SetRenderState(D3DRENDERSTATE_ZENABLE, dwD3DRS_ZENABLE);
    NK_ASSERT(SUCCEEDED(hr));
    hr = d3d7.device->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, dwD3DRS_ZWRITEENABLE);
    NK_ASSERT(SUCCEEDED(hr));
    hr = d3d7.device->SetRenderState(D3DRENDERSTATE_CULLMODE, dwD3DRS_CULLMODE);
    NK_ASSERT(SUCCEEDED(hr));


    hr = d3d7.device->SetTextureStageState(0, D3DTSS_ADDRESSU, dwD3DSAMP_ADDRESSU); // U 좌표의 텍스처 래핑 모드를 가져옴
    NK_ASSERT(SUCCEEDED(hr));
    hr = d3d7.device->SetTextureStageState(0, D3DTSS_ADDRESSV, dwD3DSAMP_ADDRESSV); // V 좌표의 텍스처 래핑 모드를 가져옴
    NK_ASSERT(SUCCEEDED(hr));
    hr = d3d7.device->SetTextureStageState(0, D3DTSS_MAGFILTER, dwD3DSAMP_MAGFILTER); // 확대 필터를 가져옴
    NK_ASSERT(SUCCEEDED(hr));
    hr = d3d7.device->SetTextureStageState(0, D3DTSS_MINFILTER, dwD3DSAMP_MINFILTER); // 축소 필터를 가져옴
    NK_ASSERT(SUCCEEDED(hr));

    /* Set texture stage state */
    hr = d3d7.device->SetTextureStageState(0, D3DTSS_COLOROP, dwD3DTSS_COLOROP);
    NK_ASSERT(SUCCEEDED(hr));
    hr = d3d7.device->SetTextureStageState(0, D3DTSS_COLORARG1, dwD3DTSS_COLORARG1);
    NK_ASSERT(SUCCEEDED(hr));
    hr = d3d7.device->SetTextureStageState(0, D3DTSS_COLORARG2, dwD3DTSS_COLORARG2);
    NK_ASSERT(SUCCEEDED(hr));
    hr = d3d7.device->SetTextureStageState(0, D3DTSS_ALPHAOP, dwD3DTSS_ALPHAOP);
    NK_ASSERT(SUCCEEDED(hr));
    hr = d3d7.device->SetTextureStageState(0, D3DTSS_ALPHAARG1, dwD3DTSS_ALPHAARG1);
    NK_ASSERT(SUCCEEDED(hr));
    hr = d3d7.device->SetTextureStageState(0, D3DTSS_ALPHAARG2, dwD3DTSS_ALPHAARG2);
    NK_ASSERT(SUCCEEDED(hr));
}

NK_API void
nk_d3d7_render(enum nk_anti_aliasing AA) {
    nk_d3d7_create_state();

    struct nk_buffer vbuf, ebuf;
    const struct nk_draw_command* cmd;
    const nk_draw_index* offset = NULL;
    struct nk_convert_config config;
    struct nk_d3d7_vertex* vertices;
    int vertex_count;

    static const struct nk_draw_vertex_layout_element vertex_layout[] = {
        {NK_VERTEX_POSITION, NK_FORMAT_FLOAT, NK_OFFSETOF(struct nk_d3d7_vertex, x)},
        {NK_VERTEX_COLOR, NK_FORMAT_R8G8B8A8, NK_OFFSETOF(struct nk_d3d7_vertex, col)},
        {NK_VERTEX_TEXCOORD, NK_FORMAT_FLOAT, NK_OFFSETOF(struct nk_d3d7_vertex, u)},
        {NK_VERTEX_LAYOUT_END}
    };

    memset(&config, 0, sizeof(config));
    config.vertex_layout = vertex_layout;
    config.vertex_size = sizeof(struct nk_d3d7_vertex);
    config.vertex_alignment = NK_ALIGNOF(struct nk_d3d7_vertex);
    config.global_alpha = 1.0f;
    config.shape_AA = AA;
    config.line_AA = AA;
    config.circle_segment_count = 22;
    config.curve_segment_count = 22;
    config.arc_segment_count = 22;
    config.tex_null = d3d7.tex_null;

    nk_buffer_init_default(&vbuf);
    nk_buffer_init_default(&ebuf);
    nk_convert(&d3d7.ctx, &d3d7.cmds, &vbuf, &ebuf, &config);

    vertex_count = (int)(vbuf.needed / sizeof(struct nk_d3d7_vertex));
    offset = (const nk_draw_index*)nk_buffer_memory_const(&ebuf);

    vertices = (struct nk_d3d7_vertex*)nk_buffer_memory_const(&vbuf);
    for (int i = 0; i < vertex_count; ++i) {
        struct nk_d3d7_vertex vertex = vertices[i];
        vertices[i].x += 0.5f;
        vertices[i].y += 0.5f;
        vertices[i].z = 0.0f;
        vertices[i].rhw = 1.0f;
    }

    D3DVIEWPORT7 originalViewport;
    d3d7.device->GetViewport(&originalViewport);

    for (cmd = nk__draw_begin(&d3d7.ctx, &d3d7.cmds); cmd != NULL; cmd = nk__draw_next(cmd, &d3d7.cmds, &d3d7.ctx)) {
        if (!cmd->elem_count) continue;

        HRESULT hr = d3d7.device->SetTexture(0, (IDirectDrawSurface7*)cmd->texture.ptr);
        NK_ASSERT(SUCCEEDED(hr));

        // 클리핑 영역을 설정
        RECT scissor;
        scissor.left = max((LONG)cmd->clip_rect.x, 0);
        scissor.top = max((LONG)cmd->clip_rect.y, 0);
        scissor.right = min((LONG)(cmd->clip_rect.x + cmd->clip_rect.w), (LONG)originalViewport.dwWidth);
        scissor.bottom = min((LONG)(cmd->clip_rect.y + cmd->clip_rect.h), (LONG)originalViewport.dwHeight);

        D3DVIEWPORT7 vp;
        vp.dwX = scissor.left;
        vp.dwY = scissor.top;
        vp.dwWidth = scissor.right - scissor.left;
        vp.dwHeight = scissor.bottom - scissor.top;
        vp.dvMinZ = 0.0f;
        vp.dvMaxZ = 1.0f;

        if (vp.dwWidth > 0 && vp.dwHeight > 0) {
            d3d7.device->SetViewport(&vp);

            for (unsigned int i = 0; i < cmd->elem_count; i += 3) {
                struct nk_d3d7_vertex v0 = vertices[offset[i]];
                struct nk_d3d7_vertex v1 = vertices[offset[i + 1]];
                struct nk_d3d7_vertex v2 = vertices[offset[i + 2]];

                D3DTLVERTEX tl_vertices[3] = { 0, };
                tl_vertices[0].sx = v0.x; tl_vertices[0].sy = v0.y; tl_vertices[0].sz = v0.z; tl_vertices[0].rhw = v0.rhw;
                tl_vertices[0].color = v0.col; tl_vertices[0].specular = 0; tl_vertices[0].tu = v0.u; tl_vertices[0].tv = v0.v;
                tl_vertices[1].sx = v1.x; tl_vertices[1].sy = v1.y; tl_vertices[1].sz = v1.z; tl_vertices[1].rhw = v1.rhw;
                tl_vertices[1].color = v1.col; tl_vertices[1].specular = 0; tl_vertices[1].tu = v1.u; tl_vertices[1].tv = v1.v;
                tl_vertices[2].sx = v2.x; tl_vertices[2].sy = v2.y; tl_vertices[2].sz = v2.z; tl_vertices[2].rhw = v2.rhw;
                tl_vertices[2].color = v2.col; tl_vertices[2].specular = 0; tl_vertices[2].tu = v2.u; tl_vertices[2].tv = v2.v;

                d3d7.device->DrawPrimitive(D3DPT_TRIANGLELIST, D3DFVF_TLVERTEX, tl_vertices, 3, D3DDP_WAIT);
            }
        }

        offset += cmd->elem_count;
    }
    
    d3d7.device->SetViewport(&originalViewport);

    nk_buffer_free(&vbuf);
    nk_buffer_free(&ebuf);

    nk_clear(&d3d7.ctx);
    nk_buffer_clear(&d3d7.cmds);

    nk_d3d7_create_state_restore();
}

NK_API void
nk_d3d7_resize(int width, int height) {
    if (d3d7.font_texture) {
        d3d7.font_texture->Release();
        d3d7.font_texture = nullptr;
        nk_d3d7_create_font_texture();
    }
}

NK_API void
nk_d3d7_release(void) {
    if (d3d7.font_texture)
    {
        d3d7.font_texture->Release();
        d3d7.font_texture = NULL;
    }
}

NK_API void
nk_d3d7_shutdown(void) {
    nk_d3d7_release();
    nk_font_atlas_clear(&d3d7.atlas);
    nk_buffer_free(&d3d7.cmds);
    nk_free(&d3d7.ctx);
}

NK_API int
nk_d3d7_handle_event(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
    if (d3d7.bInit != 10000) 
        return 0;

    RECT rect;
    GetClientRect(hwnd, &rect);
    if (ClientToScreen(hwnd, (POINT*)&rect) == FALSE) return 0;
    if (ClientToScreen(hwnd, (POINT*)&rect + 1) == FALSE) return 0;

    POINT pt;
    pt.x = LOWORD(lparam);
    pt.y = HIWORD(lparam);
    ClientToScreen(hwnd, &pt);

    pt.x -= rect.left;
    pt.y -= rect.top;

    switch (msg)
    {
    case WM_KEYDOWN:
    case WM_KEYUP:
    case WM_SYSKEYDOWN:
    case WM_SYSKEYUP:
    case WM_IME_KEYDOWN:
    case WM_IME_KEYUP:
    {
        int down = !((lparam >> 31) & 1);
        int ctrl = GetKeyState(VK_CONTROL) & (1 << 15);
        switch (wparam)
        {
        case VK_SHIFT:
        case VK_LSHIFT:
        case VK_RSHIFT:
            nk_input_key(&d3d7.ctx, NK_KEY_SHIFT, down);
            return 1;

        case VK_DELETE:
            d3d7.lastUnicode = 0;
            nk_input_key(&d3d7.ctx, NK_KEY_DEL, down);
            return 1;

        case VK_RETURN:
            nk_input_key(&d3d7.ctx, NK_KEY_ENTER, down);
            return 1;

        case VK_TAB:
            d3d7.lastUnicode = 0;
            nk_input_key(&d3d7.ctx, NK_KEY_TAB, down);
            return 1;

        case VK_LEFT:
            d3d7.lastUnicode = 0;
            if (ctrl) {
                nk_input_key(&d3d7.ctx, NK_KEY_TEXT_WORD_LEFT, down);
            }
            else {
                nk_input_key(&d3d7.ctx, NK_KEY_LEFT, down);
            }
            return 1;

        case VK_RIGHT:
            d3d7.lastUnicode = 0;
			if (ctrl) {
				nk_input_key(&d3d7.ctx, NK_KEY_TEXT_WORD_RIGHT, down);
			}
			else {
				nk_input_key(&d3d7.ctx, NK_KEY_RIGHT, down);
			}
            return 1;

        case VK_BACK:
            if (d3d7.ctx.text_edit.bComposition) {
                nk_input_key(&d3d7.ctx, NK_KEY_BACKSPACE, down);
            }
            else {
                if (down) {
                    ++d3d7.keyDown;
                }
                else {
                    --d3d7.keyDown;
                }

                if (d3d7.keyDown < 0) {
                    down = 1;
                    d3d7.keyDown = 0;
                }

                d3d7.lastUnicode = 0;
                nk_input_key(&d3d7.ctx, NK_KEY_BACKSPACE, down);
            }
            return 1;

        case VK_HOME:
            d3d7.lastUnicode = 0;
            nk_input_key(&d3d7.ctx, NK_KEY_TEXT_START, down);
            nk_input_key(&d3d7.ctx, NK_KEY_SCROLL_START, down);
            return 1;

        case VK_END:
            d3d7.lastUnicode = 0;
            nk_input_key(&d3d7.ctx, NK_KEY_TEXT_END, down);
            nk_input_key(&d3d7.ctx, NK_KEY_SCROLL_END, down);
            return 1;

        case VK_NEXT:
            nk_input_key(&d3d7.ctx, NK_KEY_SCROLL_DOWN, down);
            return 1;

        case VK_PRIOR:
            nk_input_key(&d3d7.ctx, NK_KEY_SCROLL_UP, down);
            return 1;

        case 'C':
            if (ctrl) {
                nk_input_key(&d3d7.ctx, NK_KEY_COPY, down);
                return 1;
            }
            break;

        case 'V':
            if (ctrl) {
                nk_input_key(&d3d7.ctx, NK_KEY_PASTE, down);
                return 1;
            }
            break;

        case 'X':
            if (ctrl) {
                nk_input_key(&d3d7.ctx, NK_KEY_CUT, down);
                return 1;
            }
            break;

        case 'Z':
            if (ctrl) {
                nk_input_key(&d3d7.ctx, NK_KEY_TEXT_UNDO, down);
                return 1;
            }
            break;

        case 'R':
            if (ctrl) {
                nk_input_key(&d3d7.ctx, NK_KEY_TEXT_REDO, down);
                return 1;
            }
            break;
        }
        return 0;
    }
    case WM_LBUTTONDOWN:
        d3d7.lastUnicode = 0;
        nk_input_button(&d3d7.ctx, NK_BUTTON_LEFT, pt.x, pt.y, 1);
        SetCapture(hwnd);
        return 1;

    case WM_LBUTTONUP:
        d3d7.lastUnicode = 0;
        nk_input_button(&d3d7.ctx, NK_BUTTON_DOUBLE, pt.x, pt.y, 0);
        nk_input_button(&d3d7.ctx, NK_BUTTON_LEFT, pt.x, pt.y, 0);
        ReleaseCapture();
        return 1;

    case WM_RBUTTONDOWN:
        d3d7.lastUnicode = 0;
        nk_input_button(&d3d7.ctx, NK_BUTTON_RIGHT, pt.x, pt.y, 1);
        SetCapture(hwnd);
        return 1;

    case WM_RBUTTONUP:
        d3d7.lastUnicode = 0;
        nk_input_button(&d3d7.ctx, NK_BUTTON_RIGHT, pt.x, pt.y, 0);
        ReleaseCapture();
        return 1;

    case WM_MBUTTONDOWN:
        d3d7.lastUnicode = 0;
        nk_input_button(&d3d7.ctx, NK_BUTTON_MIDDLE, pt.x, pt.y, 1);
        SetCapture(hwnd);
        return 1;

    case WM_MBUTTONUP:
        d3d7.lastUnicode = 0;
        nk_input_button(&d3d7.ctx, NK_BUTTON_MIDDLE, pt.x, pt.y, 0);
        ReleaseCapture();
        return 1;

    case WM_MOUSEWHEEL:
        d3d7.lastUnicode = 0;
        nk_input_scroll(&d3d7.ctx, nk_vec2(0, (float)(short)HIWORD(wparam) / WHEEL_DELTA));
        return 1;

    case WM_MOUSEMOVE:
        d3d7.lastUnicode = 0;
        nk_input_motion(&d3d7.ctx, pt.x, pt.y);
        return 1;

    case WM_LBUTTONDBLCLK:
        d3d7.lastUnicode = 0;
        nk_input_button(&d3d7.ctx, NK_BUTTON_DOUBLE, pt.x, pt.y, 1);
        return 1;

    case WM_CHAR:
        if (wparam >= 32)
        {
            if (d3d7.lastUnicode != 0) {
                nk_input_unicode(&d3d7.ctx, d3d7.lastUnicode);
                d3d7.lastUnicode = 0;
            }
            nk_input_unicode(&d3d7.ctx, (nk_rune)wparam);
            return 1;
        }
		break;
	case WM_IME_STARTCOMPOSITION:
		d3d7.ctx.text_edit.bComposition = true;
		return 1;
	case WM_IME_COMPOSITION: {
		HIMC hIMC = ImmGetContext(hwnd);
		if (!hIMC) {
			return 0;
		}
		//조합 완료된 문자열을 가져오는 부분
		if (lparam & GCS_RESULTSTR) {
			DWORD size = ImmGetCompositionStringW(hIMC, GCS_RESULTSTR, NULL, 0);
			if (size > 0) {
				WCHAR* buffer = new WCHAR[size / sizeof(WCHAR) + 1];
				ImmGetCompositionStringW(hIMC, GCS_RESULTSTR, buffer, size);
				buffer[size / sizeof(WCHAR)] = 0;  // null-terminate
				// buffer를 사용하여 입력 처리
				d3d7.lastUnicode = buffer[0];
				//nk_input_unicode(&d3d7.ctx, buffer[0]);
				delete[] buffer;
			}
		}
		// 조합 중인 문자열을 가져오는 부분
		else if (lparam & GCS_COMPSTR) {

			if (d3d7.ctx.text_edit.bComposition && d3d7.lastUnicode != 0) {
				nk_input_unicode(&d3d7.ctx, d3d7.lastUnicode);
				d3d7.lastUnicode = 0;
			}

			DWORD size = ImmGetCompositionStringW(hIMC, GCS_COMPSTR, NULL, 0);
			if (size > 0) {
				WCHAR* buffer = new WCHAR[size / sizeof(WCHAR) + 1];
				ImmGetCompositionStringW(hIMC, GCS_COMPSTR, buffer, size);
				buffer[size / sizeof(WCHAR)] = 0;  // null-terminate
				// nk_input_unicode 함수를 사용하여 Nuklear에 문자열 입력을 반영할 수 있습니다.
				// 각 문자에 대해 nk_input_unicode 호출이 필요할 수 있습니다.

				for (int i = 0; buffer[i] != 0; ++i) {
					nk_input_unicode(&d3d7.ctx, buffer[i]);
				}
				delete[] buffer;
			}
		}
		ImmReleaseContext(hwnd, hIMC);
		return 1;
	}
	case WM_IME_ENDCOMPOSITION:
		d3d7.ctx.text_edit.bComposition = false;
		return 1;
	}
    return 0;
}

#endif // NK_D3D7_IMPLEMENTATION
