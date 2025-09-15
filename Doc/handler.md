# C++ 커맨드 핸들러 가이드

`nkModule`은 C++ 환경에서 UI 요소를 안전하고 효율적으로 제어할 수 있도록 커맨드 핸들러 인터페이스를 제공합니다. 이 인터페이스는 DLL로 빌드된 `nkModule`의 내부 기능을 외부 애플리케이션에 안정적으로 노출하는 역할을 합니다.

## 특징

- **안정적인 인터페이스**: 내부 객체 포인터를 직접 노출하는 대신, 문자열 기반의 커맨드와 `void*` 타입의 파라미터를 사용하여 DLL 경계를 넘어 안전하게 함수를 호출합니다.
- **단순화된 API**: `NKCommand`라는 단일 함수를 통해 모든 UI 요소의 조작을 수행하므로, API 사용법이 간단하고 일관됩니다.
- **C++20 표준 준수**: 최신 C++ 표준을 사용하여 작성되었습니다.

## 핵심 API: `NKCommand`

UI 요소를 제어하기 위한 핵심 함수는 `NKCommand`입니다.

```cpp
bool NKCommand(const char* primaryName, const char* command, void* param);
```

- `primaryName`: 제어하고자 하는 UI 요소의 고유 이름(Primary Name)입니다. 이 이름은 UI 에디터에서 설정할 수 있습니다.
- `command`: 실행할 명령을 나타내는 문자열입니다. (예: "SetLabel", "SetChecked")
- `param`: 명령에 필요한 데이터를 전달하는 포인터입니다. 데이터의 타입은 각 명령에 따라 다릅니다.

> 📄 **상세 설명:** [지원 커맨드 목록](./handler/command.md)

## 기본 사용 예제

아래는 `NKCommand` 함수를 사용하여 특정 버튼을 찾아 비활성화하고, 레이블의 텍스트를 변경하는 예제입니다.

```cpp
// 'OK_Button'이라는 이름의 버튼을 찾아 비활성화합니다.
bool bDisable = true;
NKCommand("OK_Button", "DisableButton", &bDisable);

// 'Info_Label'이라는 이름의 레이블을 찾아 텍스트를 변경합니다.
// C-Style 문자열로 전달해야 합니다.
const char* newText = "상태가 변경되었습니다.";
NKCommand("Info_Label", "SetLabel", (void*)newText);

// 'Active_Toggle' 체크박스의 상태를 가져옵니다.
bool bIsChecked = false;
NKCommand("Active_Toggle", "IsChecked", &bIsChecked);
// 이제 bIsChecked 변수에 체크박스의 현재 상태가 저장됩니다.
```

## UI 클래스별 지원 커맨드

각 UI 요소(클래스)가 지원하는 커맨드의 종류와 필요한 파라미터 타입은 아래의 상세 문서에서 확인할 수 있습니다.

> 📄 **상세 설명:** [UI 클래스별 커맨드 가이드](./handler/class.md)

### 지원되는 클래스 목록
- NKWindow
- NKSpace
- NKGroup
- NKPopup
- NKCombo
- NKButton
- NKEdit
- NKImage
- NKLabel
- NKComboItem
- NKCheckbox
- NKSlider
- NKProgress
- NKSelectable
- NKTree
- NKChart
- NKTooltip
- NKMenu
- NKScrollbar
- NKColorPicker
- NKSuperStyleObject
