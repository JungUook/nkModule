# nkmod.h 개요

## 예제

#### c++
```markdown
IDirect3DDevice7* device = nullptr;
IDirectDrawSurface7* primary = nullptr;
IDirectDrawSurface7* backBuffer = nullptr;
IDirectDraw7* ddraw = nullptr;

int main(void)
{
    WNDCLASSW wc;
    RECT rect = { 0, 0, 1280, 960 };
    DWORD style = WS_OVERLAPPEDWINDOW;
    DWORD exstyle = WS_EX_APPWINDOW;
    HWND wnd;
    int running = 1;

    memset(&wc, 0, sizeof(wc));
    wc.style = CS_DBLCLKS;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandleW(0);
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.lpszClassName = L"NuklearWindowClass";
    RegisterClassW(&wc);

    AdjustWindowRectEx(&rect, style, FALSE, exstyle);

    wnd = CreateWindowExW(exstyle, wc.lpszClassName, L"Nuklear Direct3D 7 Demo",
        style | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT,
        rect.right - rect.left, rect.bottom - rect.top,
        NULL, NULL, wc.hInstance, NULL);

    RegistHWND(wnd);
#ifdef _NKDEBUG
    InitSubWindow(wc.hInstance, "\\NInterface\\Data2");
#endif

    struct background {
        int r;
        int g;
        int b;
        int a;
    };

    background bg;
    bg.r = 100, bg.g = 100, bg.b = 100, bg.a = 255;

    Initialize(NULL, NULL, 1280, 960, 0, "\\NInterface\\Data2");

    device = (IDirect3DDevice7*)GetDevice();
    primary = GetNKPrimary();
    backBuffer = GetNKBackBuffer();
    ddraw = GetNKDDraw();

    int cnt = 0;
    while (running)
    {
        POINT pt = { 0, 0 };
        ClientToScreen(wnd, &pt);

        MSG msg;
        NKInputBegin();
        while (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT)
                running = 0;
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }
        NKInputEnd();

        NKUpdate();

        /* Draw */
        {
            int xindent = 0;
            int yindent = 0;

            RECT rect;
            GetClientRect(wnd, &rect);

            if (ClientToScreen(wnd, (POINT*)&rect) == FALSE) return 0;
            if (ClientToScreen(wnd, (POINT*)&rect + 1) == FALSE) return 0;
            

            rect.left += xindent;
            rect.top += yindent;

            rect.right = rect.left + 1280;
            rect.bottom = rect.top + 960;

            RECT srcrect = rect;
            srcrect.right -= srcrect.left;
            srcrect.left = 0;
            srcrect.bottom -= srcrect.top;
            srcrect.top = 0;

            HRESULT hr;
            hr = IDirect3DDevice7_Clear(device, 1, NULL, D3DCLEAR_TARGET, D3DRGBA(0,0,0,1), 1.0f, 0);
            if (FAILED(hr)) {
                printf("Failed IDirect3DDevice7_Clear: 0x%08lx\n", hr);
            }

            BOOL bRender = NKRender(device);

            if (!bRender) {
                backBuffer = GetNKBackBuffer();
                primary = GetNKPrimary();
                device = (IDirect3DDevice7*)GetDevice();
            }

            hr = primary->Blt(&rect, backBuffer, &srcrect, DDBLT_WAIT, NULL);
            if (FAILED(hr)) {
                printf("Failed primary->Blt: 0x%08lx\n", hr);
            }
        }
    }

    Release();
    return 0;
}
```
