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



## LuaCommand
- 루아스크립트 함수를 실행시켜주는 함수


#### Luascript
```markdown
myTable = {
    number = "None",
    string = 0,
    boolean = false
}

function MyLuaFunction(ref)
	local myNumber = ref["number"]
	local myString = ref["string"]
	local myBoolean = ref["boolean"]
end

```


#### C++
```markdown

NKInterface mInterface;

std::vector<PackedLuaParam> vParams;

PackedLuaParam myNumber;
myNumber.key = "number";
myNumber.data.type = eNK_NUMBER;
myNumber.data.value.numberValue = 500;
vParams.push_back(myNumber);


std::string str = "text 1234 data";

PackedLuaParam myString;
myString.key = "string";
myString.data.type = eNK_STRING;
myString.data.value.stringValue = &str;
vParams.push_back(myString);

PackedLuaParam myBoolean;
myBoolean.key = "boolean";
myBoolean.data.type = eNK_BOOLEAN;
myBoolean.data.value.boolValue = true;
vParams.push_back(myBoolean);

mInterface.LuaCommand("MyLuaFunction", "myTable", &vParams);
```

#### result(luascript)
```markdown

myNumber
//500

myString
//text 1234 data

myBoolean
//true
```

#### 설명
1. luascript에서 선행작업을 수행한다.(테이블, 함수)
2. 메인 클라이언트에서 PackedLuaParam 구조체를 이용하여 숫자(double), 문자열 포인터(std::string*), 판별값(bool) 등 세 종류의 데이터를 매개변수로 사용할 수 있다.
3. 루아 함수로 실행할 매개변수는 vector에 담은 후 NKLuaCommand로 함수이름, 테이블이름, vector포인터 순으로 넣어 실행하면 된다.
4. 만약 매개변수가 하나라면 vector에 담지 않고 PackedLuaParam 구조체의 포인터를 넣으면 된다.
