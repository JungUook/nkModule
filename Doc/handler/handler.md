# Handler 개요

## NKHandler
- 호출될 함수를 등록할 수 있는 핸들러
- 함수와 핸들러 1:1로 매칭해야한다.

|변수|설명|
|-------|-------|
|key|클래스의 함수포인터 이름|
|handler|클래스의 함수포인터|

#### C++
```markdown
//class header
NKHandler mHandler;

//class cpp
REGIST_HANDLER(this, &MYClass::GetEvent, mHandler);

void MYClass::GetEvent(void* param) {
  int nEvent = NKGetDataInt("nEvent");
}

```

## NKInterface
- 핸들러를 관리해주는 클래스

|함수|설명|
|-------|-------|
|operator+=|구독 기능|
|operator-=|구독 해제기능|
|Command|클래스의 함수를 호출하는 기능|
|REGIST_HANDLER|핸들러 초기화 기능|

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

void MYClass::Update() {
  m_pNkInterface->Command("jangyeongsilui", "SetActive", &bActive);
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
4. Command 함수의 사용법은 하위문서 참고.



## Command
- 생성된 Ui 객체의 primaryname을 key로 찾아 그에 맞는 명령어를 사용할 수 있다.


#### C++
```markdown
bool Command(const char* primaryName, const char* command, void* param) {
	return NKCommand(primaryName, command, param);
}
```

#### 설명
1. primaryName은 ui에디터에서 설정해놓은 문자열이다.
2. command는 해당 클래스가 사용할 수 있는 함수의 이름이다.
3. param은 해당 함수에서 사용할 매개변수이다.
4. 반환되는 값은 함수실행의 성공여부이다. false는 실패, true는 성공이다.

#### [해당 문서로 이동](command.md)
