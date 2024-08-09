# nkModule 소개

Nuklear라는 라이브러리를 사용하여 DX7환경에서 에디터 형식으로 사용할 수 있도록 제작된 dll.
기본적으로 루아스크립트를 지원하며, 메인 시스템에 command를 사용할 수 있는 interface를 열어놨다.

## 특징

- 마우스 조작만으로 구현가능한 C기반 UI
- 루아스크립트를 통한 여러 이벤트 구현가능
- 메인 시스템에서는 dll로 콜백함수 및 여러 ui 커멘드 요청 가능

## Building
- C++20
- 전처리기 정의 (NKMOD_EXPORTS, FVF_XYZRHW, _DX7, _NKDEBUG)
- 추가 종속성 (nkmod_d.lib, nkmod.lib)
- 32bit

## nkmod

- exturn "C"

|함수|설명|
|-------|-------|
|RegistHWND|UI시스템을 초기화하기 전 메인 시스템에서 반드시 HWND를 등록시켜야한다.|
|InitSubWindow|에디터용 서브 윈도우를 사용하기 위한 함수이다.|
|CreateD3D7DeviceNew|DX7의 UI를 랜더링할때 사용하는 3D Device를 생성해주는 함수이다.<br>메인 시스템에서 미리 생성해둔 IDirectDraw7와 IDirectDrawSurface7자료형의 primary와 backbuffer가 있어야한다.|
|Initialize|실질적으로 dll의 시스템을 메모리 할당하는 작업이다.<br>앞에서 만들어둔 device와 메인시스템의 IDirectDraw7을 사용한다.|
|NKInputBegin|입력된 키들을 UI input system에 등록하기 시작하겠다는 함수다.<br>항상 NKInputEnd 전에 미리 실행해야한다.|
|NKInputEnd|더이상 키를 입력받지 않겠다는 함수다.<br>항상 NKInputBegin 후에 실행해야한다.|
|NKUpdate|좌표, 색상, 이미지 등 모든 UI 정보가 업데이트된다.<br>Update에서 모든 draw command가 갱신된다|
|NKRender|3D Device로 실질적으로 backbuffer에 UI를 그리는 작업을 한다.<br>메인시스템의 DC영역을 미리 backbuffer에 그린 후에 실행하면 된다.|
|HandleEvent|UI에 대한 모든 이벤트 키를 처리하는 함수다.|
|IsHovering|마우스가 UI위로 향했을때 True가 되는 함수다.|

- NKInterface

|함수|설명|
|-------|-------|
|Command|매개변수 순서대로 각각 ui오브젝트의 이름, ui오브젝트의 함수이름, 입력할&반환받을 포인터를 의미한다.|
|InitializeHandler|콜백받을 함수 구조체를 생성해주는 함수다. |


## 문서
- [nkmod](./Doc/nkmod.md)
- [lua](./Doc/lua.md)
- [handler](./Doc/handler.md)
- [editor](./Doc/editor.md)
