# 메인 시스템 설명

## 메인화면구성
<img src="./img/01.png">

## 버튼 설명
<img src="./img/02.png">

#### Clear
- 프로젝트를 모두 비우는 버튼

#### Reload
- 등록되어있는 루아스크립트를 모두 다시 로드하는 버튼

#### Save
- 현재 상태의 프로젝트를 저장하는 버튼(nkmod.json, nkmod.bin)

#### Load
- 저장되어있던 프로젝트를 로드하는 버튼(nkmod.json, nkmod.bin)

## 옵션 설명
<img src="./img/03.png">

#### [node](./editor/node.md)
<img src="./img/03_00.png">

- ui 노드들을 편집하거나 복사, 삭제, 이동 등을 제어할 수 있는 옵션

#### [file](./editor/file.md)
<img src="./img/04.png">

- spr파일을 리스트화시켜 관리하며, 미리보기를 지원하는 옵션

#### [lua](./editor/lua.md)
<img src="./img/05.png">

- lua script 파일을 읽고, ui노드에 등록된 함수나 변수들을 확인할 수 있는 옵션
- release모드용으로 사용할 수 있도록 lua파일을 luac파일로 바이너리화할 수 있다.

#### [prefab](./editor/prefab.md)
<img src="./img/06.png">

- ui노드들의 프리팹을 리스트화시키는 옵션.
- 리스트에 등록된 프리팹을 생성할 수 있다.
