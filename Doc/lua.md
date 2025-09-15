# Lua 스크립팅 가이드

`nkModule`은 Lua 스크립트를 통해 UI의 동작을 동적으로 제어하고, 게임 로직과 쉽게 연동할 수 있는 강력한 스크립팅 환경을 제공합니다.

## 특징

- **LuaBridge 기반**: [LuaBridge](https://github.com/vinniefalco/LuaBridge) 라이브러리를 사용하여 C++ 클래스와 함수를 손쉽게 Lua 스크립트에 노출(binding)합니다. 이를 통해 스크립트에서 C++ 객체의 멤버 함수를 직접 호출하는 등 유연한 상호작용이 가능합니다.
- **Lua 5.4 버전**: 최신 버전의 Lua를 사용하여 안정성과 성능을 확보했습니다.
- **동적 UI 제어**: 스크립트를 통해 UI 요소의 속성을 변경하거나, 특정 조건에 따라 UI를 동적으로 생성하고 제거할 수 있습니다.
- **이벤트 핸들링**: 버튼 클릭, 텍스트 입력 등 UI에서 발생하는 이벤트를 Lua 함수와 연결하여 처리할 수 있습니다.

## 핵심 전역 객체

Lua 스크립트 환경에는 세 가지 핵심 전역 객체가 기본적으로 제공됩니다.

- `system`: `NuklearUI`의 메인 인스턴스로, UI를 전역적으로 관리하는 함수들을 제공합니다. (예: `system:Find("Button_OK")`)
- `interface`: C++과 Lua 사이의 데이터 통신 및 이벤트 처리를 담당하는 인터페이스 객체입니다.
- `nkio`: 파일 입출력, 특히 프리팹(`.prefab`) 파일을 로드하는 기능을 담당합니다.

> 📄 **상세 설명:** [전역 객체 API 가이드](./lua/global.md)

## 기본 사용 예제

아래는 Lua 스크립트를 사용하여 특정 버튼을 찾아 비활성화하고, 레이블의 텍스트를 변경하는 예제입니다.

```lua
-- 'OK_Button'이라는 이름의 버튼 객체를 찾습니다.
local okButton = system:FindButton("OK_Button")

if okButton ~= nil then
    -- 버튼을 비활성화합니다.
    okButton:DisableButton(true)
end

-- 'Info_Label'이라는 이름의 레이블 객체를 찾습니다.
local infoLabel = system:FindLabel("Info_Label")

if infoLabel ~= nil then
    -- 레이블의 텍스트를 변경합니다.
    infoLabel:SetLabel("상태가 변경되었습니다.")
end
```

## UI 클래스 API

`nkModule`에서 사용할 수 있는 모든 UI 요소들은 Lua 클래스로 노출되어 있습니다. 각 클래스의 상세한 API 문서는 아래 링크에서 확인할 수 있습니다.

> 📄 **상세 설명:** [UI 클래스 API 가이드](./lua/class.md)

### 지원되는 클래스 목록
- Window
- Space
- Group
- Popup
- Combo
- Button
- Edit
- Image
- Label
- ComboItem
- Checkbox
- Slider
- Progress
- Selectable
- Tree
- Chart
- Tooltip
- Menu
- Scrollbar
- ColorPicker
- SuperStyleObject
