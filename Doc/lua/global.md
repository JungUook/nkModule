# system
- 메인 시스템(매니저)를 wrapping한 객체
- 주로 UI객체에 접근할때 사용한다.

|함수|설명|
|-------|-------|
|Find|UI클래스를 반환해주는 함수<br>해당 함수로 반환된 객체는 Base이며 모든 UI객체의 부모이다.<br>만약, 원하는 타입의 객체를 반환받고 싶다면 Find + "객체 이름"인 함수를 사용하면 된다.<br>매개변수는 primaryname을 사용하며, 이는 에디터나 루아스크립트로 설정할 수 있다.|

#### LuaScript
```markdown
local primaryname = "base01"
local base = system:Find(primaryKey)

local windowKey = "window01"
local window = system:FindWindow(windowKey)
```

#### 사용 가능한 타입들
- [Window](./class.md#window)
- [Space](./class.md#space)
- [Group](./class.md#group)
- [Popup](./class.md#popup)
- [Combo](./class.md#combo)
- [Button](./class.md#button)
- [Edit](./class.md#edit)
- [Image](./class.md#image)
- [Label](./class.md#label)
- [ComboItem](./class.md#comboItem)
- [Checkbox](./class.md#checkbox)
- [Slider](./class.md#slider)
- [Progress](./class.md#progress)
- [Selectable](./class.md#selectable)
- [Tree](./class.md#tree)
- [Chart](./class.md#chart)
- [Tooltip](./class.md#tooltip)
- [Menu](./class.md#menu)
- [Scrollbar](./class.md#scrollbar)
- [ColorPicker](./class.md#colorPicker)
- [SuperStyleObject](./class.md#superStyleObject)



# interface
- 이 객체를 사용하기 위해서는 사전 작업이 필요하다.
- 루아 스크립트에서 핸들러를 호출하기 위한 객체이다.

|함수|설명|
|-------|-------|
|TriggerEvent|기존 프로젝트에서 구독해놓은 핸들러를 호출한다.<br>자세한 사용법은 아래 예시를 참고.

#### C++
```markdown
//class header
NKInterface* pInterface;
NKHandler mHandler;

//class cpp
REGIST_HANDLER(this, &MYClass::GetEvent, mHandler);
(*pInterface) += mHandler;

void MYClass::GetEvent(void* param) {
  int nEvent = NKGetDataInt("nEvent");
}

```


#### Luascript
```markdown
local data = {
  key = "&MYClass::GetEvent",
  value = {
    nEvent = 2
  }
}
interface:TriggerEvent(data)

```

#### 설명
1. C++에서 NKInterface 포인터에 객체를 할당하고 핸들러 등록을 해둔 후 빌드한다.
2. luascript에서 위 데이터 형식을 맞춰서 함수를 호출한다.
3. C++에서 GetEvent가 호출되고 nEvent의 값을 2인 것을 확인할 수 있다.


# nkio
- 이 객체를 프리팹을 만드는 기능을 담당한다.
- 주로 에디터에서 미리 생성하여 만들어둔 프리팹을 동적으로 생성할때 사용한다.

|함수|설명|
|-------|-------|
|LoadPrefab|사용하고 싶은 프리팹을 생성한다. <br>부모 없이 생성하면 새로운 윈도우가 생성된 후 그 자식으로 설정된다. <br>부모를 설정하면 그 부모의 자식으로 설정된다.<br>반환값은 프리맵 최상단 객체이다.|


#### Luascript
```markdown
//부모없이 일반적인 생성
local data = {
  filename = "path\\myPrefab.json",
  parent = nil
}
local prefab = nkio:LoadPrefab(data)

//부모를 설정한 생성
local parentObj = system:Find("myParent")
local data = {
  filename = "path\\myPrefab.json",
  parent = parentObj
}
local prefab = nkio:LoadPrefab(data)
```

#### 설명
- 매개변수는 filename에 원하는 프리팹의 경로를 설정한다. 경로는 root에서의 상대경로이며, json과 bin 확장자를 지원한다.
- 프리팹 생성시 부모설정에 주의해야한다.
  - Window 타입은 부모를 가질 수 없다.
  - 모든 하위 ui는 space타입을 부모로 가져야한다.
  - 추가 예외사항 작성중...
