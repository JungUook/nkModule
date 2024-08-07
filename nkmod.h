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
	enum ParamType { eNK_NUMBER = 0, eNK_STRING = 1, eNK_BOOLEAN = 2 };
	union LuaData { double numberValue;	std::string* stringValue; bool boolValue; };
	struct LuaParam { ParamType type; LuaData value; };
	struct PackedLuaParam { std::string key; LuaParam data; };


	NKMOD_API void RegistHWND(HWND wnd);

#ifdef _NKDEBUG
	NKMOD_API BOOL InitSubWindow(HINSTANCE hInstance, const char* fontPath = nullptr);
#endif

#ifdef _DX9
	NKMOD_API void CreateD3D9Device(HWND wnd);
	NKMOD_API void Initialize(IDirect3DDevice9* device, int width, int height, int lang);
#elif _DX7
	NKMOD_API void CreateD3D7Device(HWND wnd, int width, int height);
	NKMOD_API void CreateD3D7DeviceNew(HWND wnd, IDirectDraw7* pdd, IDirectDrawSurface7* primary, int width, int height);
	NKMOD_API void CreateD3D7DeviceWindow(HWND wnd, IDirectDraw7* pdd, IDirectDrawSurface7* primary, int width, int height, int bpp);
	NKMOD_API void Initialize(IDirectDraw7* pdd, void* pdevice, int width, int height, int lang, const char* fontPath = nullptr);
	NKMOD_API void LoadSprFile(const char* filename);
#endif
	NKMOD_API void LoadLuaFile(const char* filePath);
	NKMOD_API void Release();
	NKMOD_API void NKInputBegin();
	NKMOD_API void NKInputEnd();
	NKMOD_API void NKUpdate();
	NKMOD_API void NKFrameSkip();
#ifdef _DX9
	NKMOD_API void NKRender(IDirect3DDevice9* device);
#elif _DX7
	NKMOD_API BOOL NKRender(void* device);
#endif
	NKMOD_API int HandleEvent(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam);
	NKMOD_API BOOL IsHovering();
#ifdef _DX9
	NKMOD_API IDirect3DDevice9* GetDevice();
	NKMOD_API IDirect3DDevice9Ex* GetDeviceEx();
#elif _DX7
	NKMOD_API IDirectDraw7* GetNKDDraw();
	NKMOD_API IDirectDrawSurface7* GetNKPrimary();
	NKMOD_API IDirectDrawSurface7* GetNKBackBuffer();
	NKMOD_API void* GetDevice();
#endif

	NKMOD_API bool AddBindHandler(const char* key, void* callback, void(*func)(void*, void*));
	NKMOD_API bool RemoveBindHandler(const char* key);
	NKMOD_API bool NKCommand(const char* primaryName, const char* command, void* param = nullptr);
	NKMOD_API bool NKLuaCommand(const char* command, const char* tableName = nullptr, void* params = nullptr);

	NKMOD_API void* NKGetData(const char* key);
	NKMOD_API int NKGetDataInt(const char* key);
	NKMOD_API float NKGetDataFloat(const char* key);
	NKMOD_API const char* NKGetDataString(const char* key);
	NKMOD_API bool NKGetDataBool(const char* key);
}


#ifndef NKMOD_EXPORTS
#include <functional>
#include <vector>
#include <string>

#define REGIST_HANDLER(instance, func, handler) \
	NKInterface::InitializeHandler(#func, instance, func, handler)

struct NKHandler {
	std::string key;
	std::function<void(void*)>* handler;
};

class NKInterface
{
public:
	NKInterface() {};
	~NKInterface() {};

	NKInterface& operator+=(const NKHandler& other)
	{
		bool bResult = AddBindHandler(other.key.c_str(), other.handler, CallBindingEvent);
		assert(bResult && "key already exists in the map!");
		if (bResult) {
			handlers.push_back(other);
		}

		return *this;
	}
	NKInterface& operator-=(const NKHandler& other)
	{
		bool bResult = RemoveBindHandler(other.key.c_str());
		assert(bResult && "Failed to insert key-value pair for key!");
		if (bResult) {
			delete(other.handler);
			for (auto it = handlers.begin(); it != handlers.end();) {
				NKHandler& handle = *it;

				if (handle.key == other.key) {
					it = handlers.erase(it);
					break;
				}
				else {
					++it;
				}
			}
		}
		return *this;
	}
	void executeHandlers(void* event)
	{
		for (auto it = handlers.begin(); it != handlers.end(); ++it) {
			auto data = *it;
			if (data.handler) {
				(*data.handler)(event);
			}
		}
	}

	bool Command(const char* primaryName, const char* command, void* param) {
		return NKCommand(primaryName, command, param);
	}

	bool LuaCommand(const char* command, const char* tableName = nullptr, void* params = nullptr) {
		return NKLuaCommand(command, tableName, params);
	}

	template <typename T>
	static void InitializeHandler(const char* key, T* instance, void (T::* method)(void*), NKHandler& out)
	{
		out.key = key;
		out.handler = new std::function<void(void*)>(std::bind(method, instance, std::placeholders::_1));
	}

	static void CallBindingEvent(void* binding, void* params)
	{
		std::function<void(void*)>* fp = (std::function<void(void*)>*)binding;
		(*fp)(params);
	}

private:
	std::vector<NKHandler> handlers;
};
#endif