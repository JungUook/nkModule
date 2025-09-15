# nkModule 라이브러리

## 개요

`nkModule`은 [Nuklear](https://github.com/Immediate-Mode-UI/Nuklear) 라이브러리를 기반으로 하여, DirectX 7 및 DirectX 9 환경에서 손쉽게 UI를 생성하고 편집할 수 있는 기능을 제공하는 UI 편집 모듈입니다. 디버그 빌드 시 활성화되는 내장 에디터를 통해 실시간으로 UI를 수정하고, 그 결과를 JSON 또는 바이너리 파일로 저장하고 로드할 수 있습니다. 또한, Lua 스크립팅을 지원하여 UI의 동작을 동적으로 제어할 수 있습니다.

## 주요 기능

- **UI 에디터**: 디버그 모드(`_NKDEBUG` 전처리기 정의)에서 UI를 실시간으로 편집할 수 있는 기능을 제공합니다.
- **Lua 스크립팅**: Lua 스크립트를 사용하여 UI 요소의 동작을 제어하고 게임 로직과 연동할 수 있습니다.
- **직렬화**: 생성된 UI를 JSON 또는 바이너리 형식으로 저장하고 로드할 수 있어, UI 레이아웃의 재사용과 관리가 용이합니다.
- **다양한 UI 요소**: 버튼, 레이블, 에디트 박스, 슬라이더 등 Nuklear에서 제공하는 다양한 UI 요소들을 지원합니다.
- **32비트/64비트 지원**: 32비트와 64비트 아키텍처를 모두 지원하여 다양한 환경에서 사용할 수 있습니다.

## 설정 및 초기화

`nkModule`을 프로젝트에 통합하고 초기화하는 과정은 다음과 같습니다. 아래는 DirectX 7 환경을 기준으로 한 예제입니다.

### 1. 헤더 포함 및 전역 변수 선언
```cpp
#include "nkmod.h"

IDirect3DDevice7* device = nullptr;
IDirectDrawSurface7* primary = nullptr;
IDirectDrawSurface7* backBuffer = nullptr;
IDirectDraw7* ddraw = nullptr;
```

### 2. 창 생성 및 초기화
Win32 창을 생성하고 `nkModule`의 초기화 함수를 호출합니다.
```cpp
int main(void)
{
    // ... (Win32 창 생성 코드)

    // nkModule 초기화
    RegistHWND(wnd);
#ifdef _NKDEBUG
    // 디버그 에디터 창 초기화 (필요 시)
    InitSubWindow(wc.hInstance, "\\NInterface\\Data2");
#endif
    Initialize(NULL, NULL, 1280, 960, 0, "\\NInterface\\Data2");

    // DirectX 객체 가져오기
    device = (IDirect3DDevice7*)GetDevice();
    primary = GetNKPrimary();
    backBuffer = GetNKBackBuffer();
    ddraw = GetNKDDraw();
    
    // ... (메인 루프)
}
```

### 3. 메인 루프 통합
메인 렌더링 루프 안에서 입력 처리, 업데이트, 렌더링 함수를 호출합니다.
```cpp
    while (running)
    {
        // ... (메시지 처리)
        
        // Nuklear 입력 처리
        NKInputBegin();
        while (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE)) {
            // ...
        }
        NKInputEnd();

        // UI 업데이트
        NKUpdate();

        // 렌더링
        IDirect3DDevice7_Clear(device, ...);
        
        BOOL bRender = NKRender(device);

        // ... (화면 버퍼 교체)
    }

    // 리소스 해제
    Release();
    return 0;
```

## 주요 API

- `Initialize(ddraw, device, width, height, lang, fontPath)`: `nkModule`을 초기화합니다.
- `RegistHWND(hwnd)`: 메시지 처리를 위해 메인 윈도우 핸들을 등록합니다.
- `NKInputBegin()` / `NKInputEnd()`: Nuklear 입력 처리를 시작하고 끝냅니다.
- `NKUpdate()`: UI 상태를 업데이트합니다.
- `NKRender(device)`: UI를 렌더링합니다.
- `Release()`: 사용이 끝난 `nkModule`의 리소스를 해제합니다.
- `GetDevice()` / `GetNKPrimary()` / `GetNKBackBuffer()` / `GetNKDDraw()`: `nkModule` 내부의 DirectX 객체에 접근할 수 있습니다.

더 자세한 정보는 `Doc` 폴더의 다른 문서들을 참고해 주십시오.
- [editor.md](./editor/editor.md): UI 에디터 사용법
- [lua.md](./lua/lua.md): Lua 스크립팅 가이드
- [handler.md](./handler/handler.md): 커맨드 핸들러 사용법
