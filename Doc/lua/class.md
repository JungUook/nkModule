# Base
- 가장 기본이 되는 ui객체
- 이 객체에 기능은 모든 ui객체에서 공통으로 사용할 수 있다.

|함수|설명|
|-------|-------|
|SetActive|ui객체를 활성화 및 비활성화한다.|
|AddChild|ui객체를 자식으로 추가한다.|
|RemoveChild|특정 자식 객체를 제거한다.|
|SizeChild|현재 자식의 수를 반환받는다. 자식 객체의 하위 객체들은 포함되지 않는다.|
|ClearChild|모든 자식을 제거한다. 자식객체의 하위 객체들까지 모두 제거한다.|
|EditPrimaryName|primaryname을 설정한다.|
|EditWindowName|windowname을 설정한다.|
|Find|특정 index위치의 자식을 반환한다. 자식객체의 하위 객체들은 포함되지 않는다. Find + "타입이름"으로 원하는 타입인 객체를 반환받을 수 있다.|
|FindChild|특정 index위치의 자식을 반환한다. 모든 하위 객체까지 포함되며 index는 에디터에서 최상위에서 최하위순이다. FindChild + "타입이름"으로 원하는 타입인 객체를 반환받을 수 있다.|


# Window
- ui객체 중 최상단에 위치한다. 기본적인 윈도우 기능을 수행한다.

|함수|설명|
|-------|-------|
|SetFunctionName|윈도우 창이 닫힐때 호출되는 함수의 이름을 설정하는 함수|
|SetArgsName|윈도우 창이 닫힐때 호출되는 함수의 파라미터로 들어갈 테이블의 이름을 설정하는 함수|

# Space
- layout을 설정하는 객체.
- 혼자서는 화면에 아무것도 표시할 수 없다.

|함수|설명|
|-------|-------|
|SetLayout|layout타입을 설정한다.<br>static은 1, dynamic은 0이다.|
|SetCols|dynamic 타입일때 가로에 배치할 ui의 갯수를 설정하는 함수이다. 매개변수 최소값은 1이며, 자연수값이다.|

# Group
- Window 내부의 window환경을 구성할때 사용하는 객체.

# Popup
- Window에 종속된 팝업 ui 객체.
- 해당 팝업이 활성화된 상태에서는 상위 window를 조작할 수 없게된다.
- 팝업이 종료되어야 이후 상위 window를 조작할 수 있다.

# Combo
- 콤보박스. 라벨을 열어 선택한 객체로 교체할 수 있다.

|함수|설명|
|-------|-------|
|SetComboName|콤보박스에 활성화중인 문자열을 교체하는 함수|
|SetLabelSize|x,y요소로 구성된 테이블을 매개변수로 입력시 해당 값만큼 가로, 세로 길이가 결정되는 함수|
|AddItem|ComboItem을 생성하고 콤보박스의 요소에 추가한다. 이후 반환된 값은 ComboItem 객체.|

# ComboItem
- 콤보박스의 구성요소.

|함수|설명|
|-------|-------|
|SetFunctionName|ComboItem이 선택될때 호출되는 함수의 이름을 설정하는 함수|
|SetArgsName|ComboItem이 선택될때 호출되는 함수의 파라미터 이름을 설정하는 함수|

# Button


# Edit

# Image

# Label

# Checkbox

# Slider

# Progress

# Selectable

# Tree

# Chart

# Tooltip

# Menu

# Scrollbar

# ColorPicker

# SuperStyleObject
