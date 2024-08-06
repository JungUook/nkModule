# Base
- 가장 기본이 되는 ui객체
- 이 객체에 기능은 모든 ui객체에서 공통으로 사용할 수 있다.

|함수|매개변수|반환값|설명|
|-------|-------|-------|-------|
|SetActive|true or false||ui객체를 활성화 및 비활성화한다.|
|AddChild|object||ui객체를 자식으로 추가한다.|
|RemoveChild|object||특정 자식 객체를 제거한다.|
|SizeChild||int|현재 자식의 수를 반환받는다. 자식 객체의 하위 객체들은 포함되지 않는다.|
|ClearChild|||모든 자식을 제거한다. 자식객체의 하위 객체들까지 모두 제거한다.|
|EditPrimaryName|string||primaryname을 설정한다.|
|EditWindowName|string||windowname을 설정한다.|
|Find|int|base|특정 index위치의 자식을 반환한다. 자식객체의 하위 객체들은 포함되지 않는다.<br> Find + "타입이름"으로 원하는 타입인 객체를 반환받을 수 있다.|
|FindChild|int|object|특정 index위치의 자식을 반환한다.<br> 모든 하위 객체까지 포함되며 index는 에디터에서 최상위에서 최하위순이다.<br> FindChild + "타입이름"으로 원하는 타입인 객체를 반환받을 수 있다.|


# Window
- ui객체 중 최상단에 위치한다. 기본적인 윈도우 기능을 수행한다.

|함수|매개변수|반환값|설명|
|-------|-------|-------|-------|
|SetFunctionName|string||윈도우 창이 닫힐때 호출되는 함수의 이름을 설정하는 함수|
|SetArgsName|string, int||윈도우 창이 닫힐때 호출되는 함수의 파라미터로 들어갈 테이블의 이름을 설정하는 함수<br>int값이 0이면 value타입이며, 1이면 string타입이 된다.|

# Space
- layout을 설정하는 객체.
- 혼자서는 화면에 아무것도 표시할 수 없다.

|함수|매개변수|반환값|설명|
|-------|-------|-------|-------|
|SetLayout|1 or 0||layout타입을 설정한다.<br>static은 1, dynamic은 0이다.|
|SetCols|int||dynamic 타입일때 가로에 배치할 ui의 갯수를 설정하는 함수이다. 매개변수 최소값은 1이며, 자연수값이다.|

# Group
- Window 내부의 window환경을 구성할때 사용하는 객체.

# Popup
- Window에 종속된 팝업 ui 객체.
- 해당 팝업이 활성화된 상태에서는 상위 window를 조작할 수 없게된다.
- 팝업이 종료되어야 이후 상위 window를 조작할 수 있다.

# Combo
- 콤보박스. 라벨을 열어 선택한 객체로 교체할 수 있다.

|함수|매개변수|반환값|설명|
|-------|-------|-------|-------|
|SetComboName|string||콤보박스에 활성화중인 문자열을 교체하는 함수|
|SetLabelSize|table{x,y}||x,y요소로 구성된 테이블을 매개변수로 입력시 해당 값만큼 가로, 세로 길이가 결정되는 함수|
|AddItem|string|object|ComboItem을 생성하고 콤보박스의 요소에 추가한다. 이후 반환된 값은 ComboItem 객체.|

# ComboItem
- 콤보박스의 구성요소.

|함수|매개변수|반환값|설명|
|-------|-------|-------|-------|
|SetFunctionName|string||ComboItem이 선택될때 호출되는 함수의 이름을 설정하는 함수|
|SetArgsName|string, int||ComboItem이 선택될때 호출되는 함수의 파라미터 이름을 설정하는 함수<br>int값이 0이면 value타입이며, 1이면 string타입이 된다.|

# Button
- 버튼 기능

|함수|매개변수|반환값|설명|
|-------|-------|-------|-------|
|DisableButton|true or false||버튼을 비활성화할지 설정하는 함수<br>매개변수가 true일때 버튼이 비활성화되며, false라면 버튼이 활성화된다|
|SetFunctionName|string||버튼을 눌렀을때 호출될 함수의 이름을 설정하는 함수|
|SetArgsName|string, int||버튼을 눌렀을때 호출된 함수의 매개변수 이름을 설정하는 함수<br>int값이 0이면 value타입이며, 1이면 string타입이 된다.|

# Edit
- inputbox 기능

|함수|매개변수|반환값|설명|
|-------|-------|-------|-------|
|Clear|||inputbox를 비우는 함수|
|SetText|string||inputbox의 문자열을 바꾸는 함수|
|SetFunctionName|string||enter를 입력했을때 호출한 함수의 이름을 설정하는 함수|
|SetArgsName|string, int||enter를 입력했을때 호출한 함수의 파라미터 이름을 설정하는 함수<br>int값이 0이면 value타입이며, 1이면 string타입이 된다.|

# Image
- 이미지 출력 기능

|함수|매개변수|반환값|설명|
|-------|-------|-------|-------|
|SetImagePath|string||출력될 이미지의 경로를 설정하는 함수|
|SetSpriteIndex|int||스프라이트에서 출력된 인덱스를 설정하는 함수|

# Label
- 문자 출력 기능

|함수|매개변수|반환값|설명|
|-------|-------|-------|-------|
|SetLabel|string||라벨에 출력할 문자열을 설정하는 함수|

# Checkbox
- 체크박스 기능

|함수|매개변수|반환값|설명|
|-------|-------|-------|-------|
|SetLabel|string||체크박스에 출력될 문자열을 설정하는 함수|
|SetChecked|true or false||체크박스의 상태를 결정하는 함수.<br>true는 체크, false는 체크하지 않는다|
|IsChecked||true or false|체크박스의 상태를 반환받는 함수.<br>true는 체크된 상태, false는 체크되지 않은 상태|

# Slider
- 슬라이더 기능

|함수|매개변수|반환값|설명|
|-------|-------|-------|-------|
|SetRange|table{min, max}||슬라이더의 최소, 최대값을 설정하는 함수<br>min, max의 자료형은 float로 실수형이다|
|SetValue|float||슬라이더의 현재 값을 설정하는 함수|
|GetValue||float|슬라이더의 현재 값을 반환받는 함수|

# Progress
- 프로그래스바 기능

|함수|매개변수|반환값|설명|
|-------|-------|-------|-------|
|SetProgress|float||프로그래스바의 현재 값을 설정하는 함수|
|GetProgress||float|프로그래스바의 현재 값을 반환받는 함수|

# Selectable
- 선택요소 기능

|함수|매개변수|반환값|설명|
|-------|-------|-------|-------|
|SetLabel|string||선택요소에 출력될 문자열을 설정하는 함수|
|SetImagePath|string||선택요소에 출력될 이미지의 주소를 설정하는 함수|
|SetSpriteIndex|int||선택요소에 출력될 스프라이트 이미지의 index를 설정하는 함수|
|SetSelected|true or false||선택여부를 설정하는 함수|
|IsSelected||true or false|선택여부를 확인하는 함수|
|SetFunctionName|string||클릭 이벤트가 발생할 경우 호출되는 함수의 이름을 설정하는 함수|
|SetArgsName|string,int||클릭 이벤트가 발생할 경우 호출되는 함수의 파라미터 이름을 설정하는 함수<br>int값이 0이면 value타입이며, 1이면 string타입이 된다.|

# Tree
- 트리 기능

|함수|매개변수|반환값|설명|
|-------|-------|-------|-------|
|SetLabel|string||트리의 제목을 설정하는 함수|
|SetState|int||현재 트리 상태를 설정하는 함수<br>0은 최소화, 1은 확장이다|
|GetState||int|현재 트리 상태를 반환받는 함수<br>0은 최소화, 1은 확장이다|

# Chart
- 미구현

# Tooltip
- 툴팁 기능

|함수|매개변수|반환값|설명|
|-------|-------|-------|-------|
|SetLabel|string||툴팁에 표시할 문자열을 설정하는 함수|

# Menu
- 메뉴 기능

|함수|매개변수|반환값|설명|
|-------|-------|-------|-------|
|SetLabel|string||메뉴의 제목을 설정하는 함수|
|SetFunctionName|string||메뉴 이벤트가 발생할 경우 호출되는 함수의 이름을 설정하는 함수|
|SetArgsName|string,int||메뉴 이벤트가 발생할 경우 호출되는 함수의 파라미터 이름을 설정하는 함수<br>int값이 0이면 value타입이며, 1이면 string타입이 된다.|

# Scrollbar
- 미구현

# ColorPicker
- 미구현

# SuperStyleObject
- 스타일 설정 전용 객체
