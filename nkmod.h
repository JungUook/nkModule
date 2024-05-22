// 다음 ifdef 블록은 DLL에서 내보내는 작업을 더 간소화하는 매크로를 만드는
// 표준 방법입니다. 이 DLL에 들어 있는 파일은 모두 명령줄에 정의된 NKMOD_EXPORTS 기호로
// 컴파일됩니다. 이 DLL을 사용하는 프로젝트에서는 이 기호를 정의할 수 없습니다.
// 이렇게 하면 소스 파일에 이 파일이 포함된 다른 모든 프로젝트에서는
// NKMOD_API 함수를 DLL에서 가져오는 것으로 표시되는 반면, 이 DLL에서는
// 이 매크로로 정의된 기호가 내보내지는 것으로 표시됩니다.
#ifdef NKMOD_EXPORTS
#define NKMOD_API __declspec(dllexport)
#else
#define NKMOD_API __declspec(dllimport)
#endif


#ifdef _DX9
#include <d3d9.h>
#elif _DX7
#include <ddraw.h>
#endif

extern "C" {
	NKMOD_API void RegistHWND(HWND wnd);
#ifdef _DX9
	NKMOD_API void CreateD3D9Device(HWND wnd);
	NKMOD_API void Initialize(IDirect3DDevice9* device, int width, int height, int lang);
#elif _DX7
	NKMOD_API void CreateD3D7Device(HWND wnd, int width, int height);
	NKMOD_API void CreateD3D7DeviceNew(HWND wnd, IDirectDraw7* pdd, IDirectDrawSurface7* primary, IDirectDrawSurface7* backBuffer, int width, int height);
	NKMOD_API void Initialize(IDirectDraw7* pdd, void* pdevice, int width, int height, int lang);
	NKMOD_API void LoadSprFile(const char* filename);
#endif
	NKMOD_API void LoadLuaFile(const char* filePath);
	NKMOD_API void Release();
	NKMOD_API void NKInputBegin();
	NKMOD_API void NKInputEnd();
	NKMOD_API void Update();
#ifdef _DX9
	NKMOD_API void Render(IDirect3DDevice9* device);
#elif _DX7
	NKMOD_API void Render(void* device);
#endif
	NKMOD_API int HandleEvent(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam);
#ifdef _DX9
	NKMOD_API IDirect3DDevice9* GetDevice();
	NKMOD_API IDirect3DDevice9Ex* GetDeviceEx();
#elif _DX7
	NKMOD_API IDirectDraw7* GetDDraw();
	NKMOD_API IDirectDrawSurface7* GetPrimary();
	NKMOD_API IDirectDrawSurface7* GetBackBuffer();
	NKMOD_API void* GetDevice();
#endif
}