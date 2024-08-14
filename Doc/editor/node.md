# node 개요

## 예시

#### window 생성
<img src="./img/01.png">

- 마우스 우클릭으로 윈도우를 생성한다.

#### window가 생성된 후 선택된 결과
<img src="./img/02.png">

- Node 옆에 window453722544가 적혀있는 부분은 objectInfo라는 탭이다.


#### 실제 클라이언트의 결과
<img src="./img/03.png">


#### 설명

##### node
- ui system이 관리하는 모든 node가 보여지는 곳이다.
- select, copy, remove, move에 따라 하위 버튼이 달라지며, 노드를 펼쳐놓거나 축소할 수 있다.

##### objectInfo
- select된 node의 모든 정보를 표시해준다.
- objectInfo는 선택된 노드의 WindowName으로 변경된다.

##### Active
- Active로 ui를 활성화&비활성화 할 수 있다.

##### follow_parent_style
- follow_parent_style로 부모 노드의 스타일에 종속되게하거나 체크를 해제하여 독자적인 스타일을 구성할 수 있다.

##### ViewportInfo

<img src="./img/04.png">

- ViewportInfo로 현재 프로세스 해상도 정보를 확인할 수 있다.

##### DefaultInfo

<img src="./img/05.png">

- DefaultInfo로 PrimaryName, WindowName, NodeName 등을 수정할 수 있다. 이때, PrimaryName은 해당 ui노드를 검색할때 사용되며 중복될 수 없다.

##### Transform

<img src="./img/06.png">

- Transform 현재 ui노드의 좌표 x,y와 크기인 width, hegiht를 확인하고 조정할 수 있다.

##### ObjectName(NKWindow, NKSpace ...)

<img src="./img/07.png">

- ObjectName(NKWindow) 각 노드에 해당하는 이름이 위치하며 해당 노드에서만 사용할 수 있는 기능이 내장되어있다.

##### Style

<img src="./img/08.png">

- Style ui노드에 여러 수치 및 color, image등을 제어할 수 있는 기능이다.


##### Prefab

<img src="./img/09.png">

- Prefab 해당 노드의 자식노드로 prefab을 생성할 수 있는 기능이다.

## node list
## 목차
- [Window](node.md#window)
- [Space](node.md#space)
- [Group](node.md#group)
- [Popup](node.md#popup)
- [Combo](node.md#combo)
- [Button](node.md#button)
- [Edit](node.md#edit)
- [Image](node.md#image)
- [Label](node.md#label)
- [ComboItem](node.md#comboItem)
- [Checkbox](node.md#checkbox)
- [Slider](node.md#slider)
- [Progress](node.md#progress)
- [Selectable](node.md#selectable)
- [Tree](node.md#tree)
- [Chart](node.md#chart)
- [Tooltip](node.md#tooltip)
- [Menu](node.md#menu)
- [Scrollbar](node.md#scrollbar)
- [ColorPicker](node.md#colorPicker)
- [SuperStyleObject](node.md#superStyleObject)



## Window
### flag

|Flag|설명|
|-------|-------|
|BORDER|window에 border를 추가해주는 기능|
|MOVABLE|window 상단바를 마우스로 드래그하여 이동시킬 수 있는 기능, 해당 기능이 켜지면 크기 및 좌표를 에디터로 수정할 수 없다|
|SCALABEL|window 우측 하단에 크기를 조절해줄 수 있는 기능이 추가된다|
|MINIMIZABLE|window를 최소화하여 내용을 볼 수 없게 한다|
|CLOSABLE|window를 닫는 기능|
|NO_SCROLLBAR|window 내부 공간이 커져도 scrollbar가 나오지 않게 하는 기능|
|TITLE|window 상단바를 추가하는 기능|
|SCROLL_AUTO_HIDE|...|
|BACKGROUND|...|
|SCALE_LEFT|...|
|NO_INPUT|...|

### Properties
- window 상단바의 크기를 조절해주는 기능
### Create UI
- space라는 레이아웃을 제어하는 노드와 superstyle이라는 모든 스타일을 가진 노드를 생성할 수 있다.
### Data
- 루아 함수 이름을 입력하고 enter를 누르면 윈도우를 닫을때마다 해당 함수가 호출된다
- 루아 테이블이나 변수 이름을 입력하고 enter를 누르면 윈도우를 닫을때마다 미리 등록된 함수에 매개변수로 입력된다 ex) myfunction(args)

## Space
### Space_Type
- STATIC: space영역을 기준으로 상대좌표로 ui를 배치하는 설정
- DYNAMIC: count 수만큼 가로 영역에 자식노드들을 정렬하는 기능. 해당 count를 초과하는 자식노드는 바로 아랫줄에 그려진다.
### Create_UI
- 여러 ui를 생성하는 버튼. 자식 노드로 생성된다.

## Group

### flag
|Flag|설명|
|-------|-------|
|BORDER|window에 border를 추가해주는 기능|
|CLOSABLE|window를 닫는 기능|
|NO_SCROLLBAR|window 내부 공간이 커져도 scrollbar가 나오지 않게 하는 기능|
|TITLE|window 상단바를 추가하는 기능|
|SCROLL_AUTO_HIDE|...|
|BACKGROUND|...|
|SCALE_LEFT|...|
|NO_INPUT|...|

### Properties
- 미구현

### Create UI
- group은 layout인 Space만 자식노드로 생성할 수 있다.

## Popup

### flag
|Flag|설명|
|-------|-------|
|BORDER|window에 border를 추가해주는 기능|
|CLOSABLE|window를 닫는 기능|
|NO_SCROLLBAR|window 내부 공간이 커져도 scrollbar가 나오지 않게 하는 기능|
|TITLE|window 상단바를 추가하는 기능|
|SCROLL_AUTO_HIDE|...|
|BACKGROUND|...|
|SCALE_LEFT|...|
|NO_INPUT|...|

### Properties
- 미구현

### Create UI
- popup은 layout인 Space만 자식노드로 생성할 수 있다.

## Combo

### Create UI
- combo는 자식노드로 comboItem만 생성할 수 있다. 생성된 comboItem은 combo박스에서 자식노드로 설정되며, combo를 펼칠시 선택할 수 있는 버튼으로 구현된다.

### combo type
|Flag|설명|
|-------|-------|
|dynamic|transform의 너비에 맞게 라벨 및 아이템의 크기를 맞춰주는 플래그|
|static|라벨 및 아이템을 수동으로 조정할 수 있게 해주는 플래그|

### alignment
|Flag|설명|
|-------|-------|
|left|아이템들의 문자열을 좌측으로 정렬해주는 플래그|
|center|아이템들의 문자열을 중앙으로 정렬해주는 플래그|
|right|아이템들의 문자열을 우측으로 정렬해주는 플래그|

### Label Size
- Combo box를 펼쳤을때 나오는 아이템들의 배경이 되는 영역의 사이즈. x는 너비, y는 높이 값을 의미한다(dynamic에서는 x를 제어할 수 없다.)

### item Size
- ComboItem들의 크기를 조정해주는 기능. dynamic 전용 기능

### Combo Item List
- combo노드의 자식 노드들이 모두 표시된다
- 표시할 문자를 입력할 수 있고, 해당 아이템을 선택했을때 제어할 수 있는 함수나 매개변수를 설정할 수 있다.

### ComboItem
- combo 전용 자식노드. 

## Button
### Disabled
- 버튼을 비활성화한다. 비활성한 버튼은 등록한 함수를 실행할 수 없다.
### Text
- 버튼에 출력할 문자열을 입력하는 inputbox이다.
### Font size
- 버튼에 출력되는 문자열의 크기를 조정할 수 있다.
- 폰트의 크기는 정해져있기때문에 너무 크거나 작으면 글자의 해상도가 낮아져보이는 문제가 발생할 수 있다.
### Data
- 버튼을 눌렀을때 실행할 함수와 매개변수를 설정할 수 있다.

## Edit
### Data

#### luascript 예제
```markdown

editTable = {
  NK_EDIT_ACTIVE = ""
  NK_EDIT_COMMITED = ""
}

function editFunction(args)
  local editActive = args["NK_EDIT_ACTIVE"]
  local editCommited = args["NK_EDIT_COMMITED"]
end
```

#### 설명
- Edit는 inputbox이며, Data에 있는 function과 

## Image
## Label
## Checkbox
## Slider
## Progress
## Selectable
## Tree
## Chart
## Tooltip
## Menu
## Scrollbar
## ColorPicker
## SuperStyleObject
