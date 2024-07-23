// nkmod.cpp : DLL을 위해 내보낸 함수를 정의합니다.
//

#include "pch.h"
#include "framework.h"
#include "nkmod.h"
#include "NuklearUI.h"

#ifdef _DX9
IDirect3DDevice9* g_device;
IDirect3DDevice9Ex* g_deviceEx;
D3DPRESENT_PARAMETERS g_present;
#elif _DX7
#include <d3d.h>
#include "sprLoader.h"
LPDIRECTDRAW7 g_pDD = nullptr;
LPDIRECTDRAWSURFACE7 g_pDDSPrimary = nullptr;
LPDIRECTDRAWSURFACE7 g_pDDSBackBuffer = nullptr;
LPDIRECTDRAWCLIPPER g_pClipper = nullptr;
LPDIRECT3D7 g_pD3D = nullptr;
LPDIRECT3DDEVICE7 g_pD3DDevice = nullptr;
DDSURFACEDESC2 g_ddsd;
sprLoader* g_sprLoader = nullptr;
int g_renderCnt = 0;
int g_width = 0;
int g_height = 0;
#endif

NuklearUI* g_nuklear = nullptr;
HWND hwnd;

void RegistHWND(HWND wnd)
{
    hwnd = wnd;
}

#ifdef _NKDEBUG
BOOL InitSubWindow(HINSTANCE hInstance, const char* fontPath) {
    return g_nuklear->InitSubWindow(hInstance, hwnd, fontPath);
}
#endif

#ifdef _DX9
void CreateD3D9Device(HWND wnd)
{
    HRESULT hr;
    g_present.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
    g_present.BackBufferWidth = 1280;
    g_present.BackBufferHeight = 900;
    g_present.BackBufferFormat = D3DFMT_X8R8G8B8;
    g_present.BackBufferCount = 1;
    g_present.MultiSampleType = D3DMULTISAMPLE_NONE;
    g_present.SwapEffect = D3DSWAPEFFECT_DISCARD;
    g_present.hDeviceWindow = wnd;
    g_present.EnableAutoDepthStencil = TRUE;
    g_present.AutoDepthStencilFormat = D3DFMT_D24S8;
    g_present.Flags = D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;
    g_present.Windowed = TRUE;

    {/* first try to create Direct3D9Ex device if possible (on Windows 7+) */
        typedef HRESULT WINAPI Direct3DCreate9ExPtr(UINT, IDirect3D9Ex**);
        Direct3DCreate9ExPtr* Direct3DCreate9Ex = (Direct3DCreate9ExPtr*)GetProcAddress(GetModuleHandleA("d3d9.dll"), "Direct3DCreate9Ex");
        if (Direct3DCreate9Ex) {
            IDirect3D9Ex* d3d9ex;
            if (SUCCEEDED(Direct3DCreate9Ex(D3D_SDK_VERSION, &d3d9ex))) {
                hr = IDirect3D9Ex_CreateDeviceEx(d3d9ex, D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, wnd,
                    D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_PUREDEVICE | D3DCREATE_FPU_PRESERVE,
                    &g_present, NULL, &g_deviceEx);
                if (SUCCEEDED(hr)) {
                    g_device = (IDirect3DDevice9*)g_deviceEx;
                }
                else {
                    /* hardware vertex processing not supported, no big deal
                    retry with software vertex processing */
                    hr = IDirect3D9Ex_CreateDeviceEx(d3d9ex, D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, wnd,
                        D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_PUREDEVICE | D3DCREATE_FPU_PRESERVE,
                        &g_present, NULL, &g_deviceEx);
                    if (SUCCEEDED(hr)) {
                        g_device = (IDirect3DDevice9*)g_deviceEx;
                    }
                }
                IDirect3D9Ex_Release(d3d9ex);
            }
        }
    }

    if (!g_device) {
        /* otherwise do regular D3D9 setup */
        IDirect3D9* d3d9 = Direct3DCreate9(D3D_SDK_VERSION);

        hr = IDirect3D9_CreateDevice(d3d9, D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, wnd,
            D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_PUREDEVICE | D3DCREATE_FPU_PRESERVE,
            &g_present, &g_device);
        if (FAILED(hr)) {
            /* hardware vertex processing not supported, no big deal
            retry with software vertex processing */
            hr = IDirect3D9_CreateDevice(d3d9, D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, wnd,
                D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_PUREDEVICE | D3DCREATE_FPU_PRESERVE,
                &g_present, &g_device);
            assert(SUCCEEDED(hr));
        }
        IDirect3D9_Release(d3d9);
    }
}

void Initialize(IDirect3DDevice9* device, int width, int height, int lang)
{
    g_nuklear = new NuklearUI();
    if (device) {
        g_nuklear->Initialize(device, width, height, lang);
    }
    else {
        if (hwnd) {
            HRESULT hr = CoInitialize(NULL);
            CreateD3D9Device(hwnd);
            g_nuklear->Initialize(g_device, width, height, lang);
            CoUninitialize();
        }
        else {
            return;
        }
    }
}
#elif _DX7
void CreateD3D7Device(HWND wnd, int width, int height)
{
    HRESULT hr;

    hr = DirectDrawCreateEx(NULL, (void**)&g_pDD, IID_IDirectDraw7, NULL);
    if (FAILED(hr)) return;

    hr = g_pDD->SetCooperativeLevel(wnd, DDSCL_NORMAL);
    if (FAILED(hr)) return;

    memset(&g_ddsd, 0, sizeof(g_ddsd));
    g_ddsd.dwSize = sizeof(g_ddsd);
    g_ddsd.dwFlags = DDSD_CAPS;
    g_ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
    hr = g_pDD->CreateSurface(&g_ddsd, &g_pDDSPrimary, NULL);
    if (FAILED(hr)) return;

    g_ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;
    g_ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_3DDEVICE;
    g_ddsd.dwWidth = width;
    g_ddsd.dwHeight = height;

    hr = g_pDD->CreateSurface(&g_ddsd, &g_pDDSBackBuffer, NULL);
    if (FAILED(hr)) return;

    hr = g_pDD->CreateClipper(0, &g_pClipper, NULL);
    if (FAILED(hr)) return;
    hr = g_pClipper->SetHWnd(0, wnd);
    if (FAILED(hr)) return;
    hr = g_pDDSPrimary->SetClipper(g_pClipper);
    if (FAILED(hr)) return;

    hr = g_pDD->QueryInterface(IID_IDirect3D7, (void**)&g_pD3D);
    if (FAILED(hr)) return;

    hr = g_pD3D->CreateDevice(IID_IDirect3DHALDevice, g_pDDSBackBuffer, &g_pD3DDevice);
    if (FAILED(hr)) return;

    D3DVIEWPORT7 vp;
    vp.dwX = 0;  // X 오프셋을 0으로 설정
    vp.dwY = 0;  // Y 오프셋을 0으로 설정
    vp.dwWidth = width;
    vp.dwHeight = height;
    vp.dvMinZ = 0.0f;
    vp.dvMaxZ = 1.0f;
    g_pD3DDevice->SetViewport(&vp);
}
void CreateD3D7DeviceNew(HWND wnd, IDirectDraw7* pdd, IDirectDrawSurface7* primary, int width, int height)
{
    HRESULT hr;

    g_pDD = pdd;
    g_pDDSPrimary = primary;

    memset(&g_ddsd, 0, sizeof(g_ddsd));
    g_ddsd.dwSize = sizeof(g_ddsd);
    g_ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;
    g_ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_VIDEOMEMORY | DDSCAPS_3DDEVICE;
    g_ddsd.dwWidth = width;
    g_ddsd.dwHeight = height;

    hr = g_pDD->CreateSurface(&g_ddsd, &g_pDDSBackBuffer, NULL);
    if (FAILED(hr)) return;

    hr = g_pDD->QueryInterface(IID_IDirect3D7, (void**)&g_pD3D);
    if (FAILED(hr)) return;

    hr = g_pD3D->CreateDevice(IID_IDirect3DHALDevice, g_pDDSBackBuffer, &g_pD3DDevice);
    if (FAILED(hr)) return;

    D3DVIEWPORT7 vp;
    vp.dwX = 0;  // X 오프셋을 0으로 설정
    vp.dwY = 0;  // Y 오프셋을 0으로 설정
    vp.dwWidth = width;
    vp.dwHeight = height;
    vp.dvMinZ = 0.0f;
    vp.dvMaxZ = 1.0f;
    g_pD3DDevice->SetViewport(&vp);
}
void CreateD3D7DeviceWindow(HWND wnd, IDirectDraw7* pdd, IDirectDrawSurface7* primary, int width, int height, int bpp)
{
    HRESULT hr;

    g_pDD = pdd;
    g_pDDSPrimary = primary;

    ZeroMemory(&g_ddsd, sizeof(g_ddsd));
    g_ddsd.dwSize = sizeof(g_ddsd);
    g_ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;;
    g_ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_VIDEOMEMORY | DDSCAPS_3DDEVICE;
    g_ddsd.dwWidth = width;
    g_ddsd.dwHeight = height;

    hr = g_pDD->CreateSurface(&g_ddsd, &g_pDDSBackBuffer, NULL);
    if (FAILED(hr)) return;

    // Direct3D 인터페이스 가져오기
    hr = g_pDD->QueryInterface(IID_IDirect3D7, (void**)&g_pD3D);
    if (FAILED(hr)) return;

    // Direct3D 디바이스 생성
    hr = g_pD3D->CreateDevice(IID_IDirect3DHALDevice, g_pDDSBackBuffer, &g_pD3DDevice);
    if (FAILED(hr)) return;

    // 뷰포트 설정
    D3DVIEWPORT7 vp;
    vp.dwX = 0;
    vp.dwY = 0;
    vp.dwWidth = width;
    vp.dwHeight = height;
    vp.dvMinZ = 0.0f;
    vp.dvMaxZ = 1.0f;
    g_pD3DDevice->SetViewport(&vp);
}
void Initialize(IDirectDraw7* pdd, void* pvDevice, int width, int height, int lang, const char* fontPath)
{
    IDirect3DDevice7* pdevice = (IDirect3DDevice7*)pvDevice;
    g_nuklear = new NuklearUI();
    g_sprLoader = new sprLoader();
    g_width = width;
    g_height = height;
    if (pdd && pdevice) {
        g_nuklear->Initialize(pdd, pdevice, width, height, lang, fontPath);
        g_sprLoader->Init(pdd);
        g_nuklear->Register_spr(g_sprLoader);

        char path[MAX_PATH];
        HMODULE hModule = GetModuleHandle(NULL);
        if (hModule != NULL) {
            // 현재 실행 파일의 경로를 얻습니다.
            GetModuleFileNameA(hModule, path, MAX_PATH);
        }
        else {
            std::cerr << "Failed to get module handle." << std::endl;
            return;
        }
        std::string basePath(path);
        basePath = basePath.substr(0, basePath.find_last_of("\\\\"));
        std::string dataPath = basePath + "\\NInterface\\Data2";
        if (!g_nuklear->CreateDirectoryIfNotExists(dataPath)) {
            std::cerr << "Failed to create directory: " << dataPath << std::endl;
            return;
        }
        std::string filePath = dataPath + "\\nkmod.json";

        g_nuklear->m_cereal.LoadFile(g_nuklear->m_mapVariable, g_nuklear->m_mapFunction, filePath);
    }
    else {
        if (hwnd) {
            HRESULT hr = CoInitialize(NULL);
            CreateD3D7Device(hwnd, width, height);
            g_nuklear->Initialize(g_pDD, g_pD3DDevice, width, height, lang, fontPath);
            g_sprLoader->Init(g_pDD);
            g_nuklear->Register_spr(g_sprLoader);
            CoUninitialize();
        }
        else {
            return;
        }
    }
}
void LoadSprFile(const char* filename)
{
    if (g_nuklear == nullptr) {
        return;
    }
    sprData* pData = g_sprLoader->LoadSprite(filename);
    g_nuklear->LoadSpriteData(pData->GetSurface()
        , pData->GetSpr()->GetHres(), pData->GetSpr()->GetVres()
        , pData->GetSpr()->GetXSize(), pData->GetSpr()->GetYSize()
        , pData->GetSpr()->GetXCount(), pData->GetSpr()->GetYCount());
}
#endif
void LoadLuaFile(const char* filePath)
{
    if (g_nuklear == nullptr) {
        return;
    }
    g_nuklear->m_luaInterface.LoadLuaFile(filePath);
}

void Release()
{
    if (g_nuklear == nullptr) {
        return;
    }
    g_nuklear->Release();    
    if (g_nuklear != nullptr)
        delete g_nuklear;
    g_nuklear = nullptr;
#ifdef _DX7
    g_sprLoader->Release();
    if (g_sprLoader != nullptr)
        delete g_sprLoader;
    g_sprLoader = nullptr;
#endif
}

void NKInputBegin()
{
    if (g_nuklear == nullptr) {
        return;
    }

    NKFrameSkip();
    g_nuklear->NKInputBegin();
}

void NKInputEnd()
{
    if (g_nuklear == nullptr) {
        return;
    }
    g_nuklear->NKInputEnd();
}

void NKUpdate()
{
    if (g_nuklear == nullptr) {
        return;
    }
    g_nuklear->Update();
}

void NKFrameSkip()
{
    if (g_nuklear == nullptr) {
        return;
    }
    if (g_renderCnt > 0) {
        g_renderCnt = 0;
    }
    else {
        g_nuklear->FrameSkip();
    }
}

#ifdef _DX9
void NKRender(IDirect3DDevice9* device)
{
    g_nuklear->Render(device);
}
#elif _DX7
BOOL NKRender(void* device)
{
    if (g_nuklear == nullptr) {
        return FALSE;
    }
    IDirect3DDevice7* pDevice = (IDirect3DDevice7*)device;

    HRESULT hr;
    BOOL bResult = TRUE;

    hr = IDirect3DDevice7_BeginScene(pDevice);

    if (SUCCEEDED(hr)) {
        g_nuklear->Render(pDevice);
        hr = IDirect3DDevice7_EndScene(pDevice);
    }

    if (FAILED(hr)) {
        bResult = FALSE;
        
        hr = g_pDDSPrimary->IsLost();
        if (hr == DDERR_SURFACELOST) {
            hr = g_pDDSPrimary->Restore();
            if (FAILED(hr)) {
                printf("Failed to restore primary surface with error: 0x%08lx\n", hr);
            }
        }

        hr = g_pDDSBackBuffer->IsLost();
        if (hr == DDERR_SURFACELOST) {
            hr = g_pDDSBackBuffer->Restore();
            if (FAILED(hr)) {
                printf("Failed to restore primary surface with error: 0x%08lx\n", hr);

                g_pDDSBackBuffer->Release();
                g_pDDSBackBuffer = nullptr;

                ZeroMemory(&g_ddsd, sizeof(g_ddsd));
                g_ddsd.dwSize = sizeof(g_ddsd);
                g_ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;;
                g_ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_VIDEOMEMORY | DDSCAPS_3DDEVICE;
                g_ddsd.dwWidth = g_width;
                g_ddsd.dwHeight = g_height;

                hr = g_pDD->CreateSurface(&g_ddsd, &g_pDDSBackBuffer, NULL);
                if (FAILED(hr)) return FALSE;

                hr = g_pDD->QueryInterface(IID_IDirect3D7, (void**)&g_pD3D);
                if (FAILED(hr)) return FALSE;
            }

            g_pD3DDevice->Release();
            g_pD3DDevice = nullptr;

            hr = g_pD3D->CreateDevice(IID_IDirect3DHALDevice, g_pDDSBackBuffer, &g_pD3DDevice);
            if (FAILED(hr)) {
                printf("Failed g_pD3D->CreateDevice: 0x%08lx\n", hr);
                return FALSE;
            }

            D3DVIEWPORT7 vp;
            vp.dwX = 0;  // X 오프셋을 0으로 설정
            vp.dwY = 0;  // Y 오프셋을 0으로 설정
            vp.dwWidth = g_ddsd.dwWidth;
            vp.dwHeight = g_ddsd.dwHeight;
            vp.dvMinZ = 0.0f;
            vp.dvMaxZ = 1.0f;
            g_pD3DDevice->SetViewport(&vp);

            g_nuklear->SetDirectX7(g_pDD, g_pD3DDevice, g_ddsd.dwWidth, g_ddsd.dwHeight);
        }
    }

    ++g_renderCnt;

    return bResult;
}
#endif

int HandleEvent(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    if (g_nuklear == nullptr) {
        return 0;
    }
#ifdef _DX9
    return g_nuklear->HandleEvent(wnd, msg, wparam, lparam, &g_present);
#elif _DX7
    return g_nuklear->HandleEvent(wnd, msg, wparam, lparam);
#endif
}

BOOL IsHovering()
{
    if (g_nuklear == nullptr) {
        return FALSE;
    }
    return g_nuklear->IsMouseHovering();
}

#ifdef _DX9
IDirect3DDevice9* GetDevice()
{
    return g_device;
}

IDirect3DDevice9Ex* GetDeviceEx()
{
    return g_deviceEx;
}
#elif _DX7
IDirectDraw7* GetNKDDraw()
{
    return g_pDD;
}
IDirectDrawSurface7* GetNKPrimary()
{
    return g_pDDSPrimary;
}
IDirectDrawSurface7* GetNKBackBuffer()
{
    return g_pDDSBackBuffer;
}
void* GetDevice()
{
    return (void*)g_pD3DDevice;
}
#endif

bool AddHandler(int key, void(*func)(void*))
{
    if (g_nuklear == nullptr) {
        return false;
    }
    auto found = g_nuklear->m_luaInterface.m_mapBindingEventHandlers.find(key);
    if (found == g_nuklear->m_luaInterface.m_mapBindingEventHandlers.end()) {
        g_nuklear->m_luaInterface.m_mapEventHandlers.insert(std::make_pair(key, func));
        return true;
    }
    return false;
}

bool AddBindHandler(int key, void* callback, void(*func)(void*, void*))
{
    if (g_nuklear == nullptr) {
        return false;
    }
    auto found = g_nuklear->m_luaInterface.m_mapBindingEventHandlers.find(key);
    if (found == g_nuklear->m_luaInterface.m_mapBindingEventHandlers.end()) {
        BindingFunc bf;
        bf.binding = callback;
        bf.func = func;

        g_nuklear->m_luaInterface.m_mapBindingEventHandlers.insert(std::make_pair(key, bf));
        return true;
    }
    return false;
}

bool RemoveHandler(int key)
{
    if (g_nuklear == nullptr) {
        return false;
    }
    auto found = g_nuklear->m_luaInterface.m_mapBindingEventHandlers.find(key);
    if (found != g_nuklear->m_luaInterface.m_mapBindingEventHandlers.end()) {
        g_nuklear->m_luaInterface.m_mapEventHandlers.erase(key);
        return true;
    }
    return false;
}

bool RemoveBindHandler(int key)
{
    if (g_nuklear == nullptr) {
        return false;
    }
    auto found = g_nuklear->m_luaInterface.m_mapBindingEventHandlers.find(key);
    if (found != g_nuklear->m_luaInterface.m_mapBindingEventHandlers.end()) {
        g_nuklear->m_luaInterface.m_mapBindingEventHandlers.erase(key);
        return true;
    }
    return false;
}

bool NKCommand(const char* primaryName, const char* command, void* param)
{
    if (g_nuklear == nullptr) {
        return false;
    }
    return g_nuklear->NKCommand(primaryName, command, param);
}

bool NKLuaCommand(const char* command, const char* tableName)
{
    if (g_nuklear == nullptr) {
        return false;
    }
    if (tableName != nullptr) {
        g_nuklear->m_luaInterface.RunFunction(command);
    }
    else {

        luabridge::LuaRef table = g_nuklear->m_luaInterface.GetLuaTable(tableName);
        g_nuklear->m_luaInterface.RunFunctionArgs(command, table);
    }

    return false;
}

void* NKGetData(const char* key)
{
    if (g_nuklear == nullptr) {
        return nullptr;
    }
    return g_nuklear->m_luaInterface.NKGetData(key);
}

int NKGetDataInt(const char* key)
{
    if (g_nuklear == nullptr) {
        return 0;
    }
    return g_nuklear->m_luaInterface.NKGetDataInt(key);
}

float NKGetDataFloat(const char* key)
{
    if (g_nuklear == nullptr) {
        return 0.f;
    }
    return g_nuklear->m_luaInterface.NKGetDataFloat(key);
}

std::string NKGetDataString(const char* key)
{
    if (g_nuklear == nullptr) {
        return "None";
    }
    return g_nuklear->m_luaInterface.NKGetDataString(key);
}

bool NKGetDataBool(const char* key)
{
    if (g_nuklear == nullptr) {
        return false;
    }
    return g_nuklear->m_luaInterface.NKGetDataBool(key);
}
