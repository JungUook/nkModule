# Class 소개

## 목차
- [Base](class.md#base)
- [Window](class.md#window)
- [Space](class.md#space)
- [Group](class.md#group)
- [Popup](class.md#popup)
- [Combo](class.md#combo)
- [Button](class.md#button)
- [Edit](class.md#edit)
- [Image](class.md#image)
- [Label](class.md#label)
- [ComboItem](class.md#comboItem)
- [Checkbox](class.md#checkbox)
- [Slider](class.md#slider)
- [Progress](class.md#progress)
- [Selectable](class.md#selectable)
- [Tree](class.md#tree)
- [Chart](class.md#chart)
- [Tooltip](class.md#tooltip)
- [Menu](class.md#menu)
- [Scrollbar](class.md#scrollbar)
- [ColorPicker](class.md#colorPicker)
- [SuperStyleObject](class.md#superStyleObject)

## Base
- 가장 기본이 되는 ui객체
- 이 객체에 기능은 모든 ui객체에서 공통으로 사용할 수 있다.

|함수|매개변수|반환값|설명|
|-------|-------|-------|-------|
|SetActive|true or false||ui객체를 활성화 및 비활성화한다.|
|AddChild|object||ui객체를 자식으로 추가한다.|
|RemoveChild|object||특정 자식 객체를 제거한다.|
|EditPrimaryName|string||primaryname을 설정한다.|
|EditWindowName|string||windowname을 설정한다.|

## Window
- ui객체 중 최상단에 위치한다. 기본적인 윈도우 기능을 수행한다.

## Space
- layout을 설정하는 객체.
- 혼자서는 화면에 아무것도 표시할 수 없다.

|함수|매개변수|반환값|설명|
|-------|-------|-------|-------|
|SetLayout|1 or 0||layout타입을 설정한다.<br>static은 1, dynamic은 0이다.|
|SetCols|int||dynamic 타입일때 가로에 배치할 ui의 갯수를 설정하는 함수이다. 매개변수 최소값은 1이며, 자연수값이다.|

## Group
- Window 내부의 window환경을 구성할때 사용하는 객체.

## Popup
- Window에 종속된 팝업 ui 객체.
- 해당 팝업이 활성화된 상태에서는 상위 window를 조작할 수 없게된다.
- 팝업이 종료되어야 이후 상위 window를 조작할 수 있다.

## Combo
- 콤보박스. 라벨을 열어 선택한 객체로 교체할 수 있다.

|함수|매개변수|반환값|설명|
|-------|-------|-------|-------|
|SetComboName|string||콤보박스에 활성화중인 문자열을 교체하는 함수|
|SetLabelSize|table{x,y}||x,y요소로 구성된 테이블을 매개변수로 입력시 해당 값만큼 가로, 세로 길이가 결정되는 함수|
|AddItem|string|object|ComboItem을 생성하고 콤보박스의 요소에 추가한다. 이후 반환된 값은 ComboItem 객체.|

## ComboItem
- 콤보박스의 구성요소.

## Button
- 버튼 기능

|함수|매개변수|반환값|설명|
|-------|-------|-------|-------|
|DisableButton|true or false||버튼을 비활성화할지 설정하는 함수<br>매개변수가 true일때 버튼이 비활성화되며, false라면 버튼이 활성화된다|

## Edit
- inputbox 기능

|함수|매개변수|반환값|설명|
|-------|-------|-------|-------|
|Clear|||inputbox를 비우는 함수|
|SetText|string||inputbox의 문자열을 바꾸는 함수|

## Image
- 이미지 출력 기능

|함수|매개변수|반환값|설명|
|-------|-------|-------|-------|
|SetImagePath|string||출력될 이미지의 경로를 설정하는 함수|
|SetSpriteIndex|int||스프라이트에서 출력된 인덱스를 설정하는 함수|

## Label
- 문자 출력 기능

|함수|매개변수|반환값|설명|
|-------|-------|-------|-------|
|SetLabel|string||라벨에 출력할 문자열을 설정하는 함수|

## Checkbox
- 체크박스 기능

|함수|매개변수|반환값|설명|
|-------|-------|-------|-------|
|SetLabel|string||체크박스에 출력될 문자열을 설정하는 함수|
|SetChecked|true or false||체크박스의 상태를 결정하는 함수.<br>true는 체크, false는 체크하지 않는다|
|IsChecked||true or false|체크박스의 상태를 반환받는 함수.<br>true는 체크된 상태, false는 체크되지 않은 상태|

## Slider
- 슬라이더 기능

|함수|매개변수|반환값|설명|
|-------|-------|-------|-------|
|SetRange|table{min, max}||슬라이더의 최소, 최대값을 설정하는 함수<br>min, max의 자료형은 float로 실수형이다|
|SetValue|float||슬라이더의 현재 값을 설정하는 함수|
|GetValue||float|슬라이더의 현재 값을 반환받는 함수|

## Progress
- 프로그래스바 기능

|함수|매개변수|반환값|설명|
|-------|-------|-------|-------|
|SetProgress|float||프로그래스바의 현재 값을 설정하는 함수|
|GetProgress||float|프로그래스바의 현재 값을 반환받는 함수|

## Selectable
- 선택요소 기능

|함수|매개변수|반환값|설명|
|-------|-------|-------|-------|
|SetLabel|string||선택요소에 출력될 문자열을 설정하는 함수|
|SetImagePath|string||선택요소에 출력될 이미지의 주소를 설정하는 함수|
|SetSpriteIndex|int||선택요소에 출력될 스프라이트 이미지의 index를 설정하는 함수|
|SetSelected|true or false||선택여부를 설정하는 함수|
|IsSelected||true or false|선택여부를 확인하는 함수|

## Tree
- 트리 기능

|함수|매개변수|반환값|설명|
|-------|-------|-------|-------|
|SetLabel|string||트리의 제목을 설정하는 함수|
|SetState|int||현재 트리 상태를 설정하는 함수<br>0은 최소화, 1은 확장이다|
|GetState||int|현재 트리 상태를 반환받는 함수<br>0은 최소화, 1은 확장이다|

## Chart
- 미구현

## Tooltip
- 툴팁 기능

|함수|매개변수|반환값|설명|
|-------|-------|-------|-------|
|SetLabel|string||툴팁에 표시할 문자열을 설정하는 함수|

## Menu
- 메뉴 기능

|함수|매개변수|반환값|설명|
|-------|-------|-------|-------|
|SetLabel|string||메뉴의 제목을 설정하는 함수|

## Scrollbar
- 미구현

## ColorPicker
- 미구현

## SuperStyleObject
- 스타일 설정 전용 객체
