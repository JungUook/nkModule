# Command 개요


## 목차
- [Base](command.md#base)
- [Window](command.md#window)
- [Space](command.md#space)
- [Group](command.md#group)
- [Popup](command.md#popup)
- [Combo](command.md#combo)
- [Button](command.md#button)
- [Edit](command.md#edit)
- [Image](command.md#image)
- [Label](command.md#label)
- [ComboItem](command.md#comboItem)
- [Checkbox](command.md#checkbox)
- [Slider](command.md#slider)
- [Progress](command.md#progress)
- [Selectable](command.md#selectable)
- [Tree](command.md#tree)
- [Chart](command.md#chart)
- [Tooltip](command.md#tooltip)
- [Menu](command.md#menu)
- [Scrollbar](command.md#scrollbar)
- [ColorPicker](command.md#colorPicker)
- [SuperStyleObject](command.md#superStyleObject)

## Base

#### SetActive
```markdown
NKInterface* pNkInterface = /* Initialize */;
bool bActive = true;
pNkInterface->Command("ui_object_primaryName", "SetActive", &bActive);
```

#### CEditPrimaryName
```markdown
NKInterface* pNkInterface = /* Initialize */;
const char* str = "myPrimaryName";
pNkInterface->Command("ui_object_primaryName", "EditPrimaryName", &str);
```

#### CEditPrimaryName
```markdown
NKInterface* pNkInterface = /* Initialize */;
const char* str = "myWindowName";
pNkInterface->Command("ui_object_primaryName", "EditWindowName", &str);
```

## Window

#### None

## Space

#### SetLayout
```markdown
NKInterface* pNkInterface = /* Initialize */;
int iType = 0; // 0: dynamic, 1: static
pNkInterface->Command("ui_object_primaryName", "SetLayout", &iType);
```
#### SetCols
```markdown
NKInterface* pNkInterface = /* Initialize */;
int iCols = 3;
pNkInterface->Command("ui_object_primaryName", "SetCols", &iCols);
```

## Group

#### None

## Popup

#### None

## Combo

#### SetComboName
```markdown
NKInterface* pNkInterface = /* Initialize */;
const char* str = "myComboName";
pNkInterface->Command("ui_object_primaryName", "SetComboName", &str);
```

#### SetLabelSize
```markdown
NKInterface* pNkInterface = /* Initialize */;
float fData[2] = {0.f,};
fData[0] = 300.f; //x
fData[1] = 400.f; //y
float* params = fData;
pNkInterface->Command("ui_object_primaryName", "SetLabelSize", &params);
```

#### AddItem
```markdown
NKInterface* pNkInterface = /* Initialize */;
const char* str = "comboItemName";
pNkInterface->Command("ui_object_primaryName", "AddItem", &str);
```

## Button

#### DisableButton
```markdown
NKInterface* pNkInterface = /* Initialize */;
bool bDisable = true; // true: disable, false: active
pNkInterface->Command("ui_object_primaryName", "DisableButton", &bDisable);
```

## Edit

#### Clear
```markdown
NKInterface* pNkInterface = /* Initialize */;
pNkInterface->Command("ui_object_primaryName", "Clear");
```

#### SetText
```markdown
NKInterface* pNkInterface = /* Initialize */;
const char* str = "inputLabel";
pNkInterface->Command("ui_object_primaryName", "SetText", &str);
```

## Image

#### SetImagePath
```markdown
NKInterface* pNkInterface = /* Initialize */;
const char* str = "path\\imageSprite.spr";
pNkInterface->Command("ui_object_primaryName", "SetImagePath", &str);
```

#### SetSpriteIndex
```markdown
NKInterface* pNkInterface = /* Initialize */;
int iIndex = 1; // start index: 0, end index: sprite end index
pNkInterface->Command("ui_object_primaryName", "SetSpriteIndex", &iIndex);
```

## Label

#### SetLabel
```markdown
NKInterface* pNkInterface = /* Initialize */;
const char* str = "my custom label";
pNkInterface->Command("ui_object_primaryName", "SetLabel", &str);
```

## ComboItem

#### None

## Checkbox

#### SetLabel
```markdown
NKInterface* pNkInterface = /* Initialize */;
const char* str = "my custom label";
pNkInterface->Command("ui_object_primaryName", "SetLabel", &str);
```

#### SetChecked
```markdown
NKInterface* pNkInterface = /* Initialize */;
bool bCheck = true; // true: checked, false: unchecked
pNkInterface->Command("ui_object_primaryName", "SetChecked", &bCheck);
```

#### SetChecked
```markdown
NKInterface* pNkInterface = /* Initialize */;
bool bCheck = false; // true: checked, false: unchecked
bool bSuccess = pNkInterface->Command("ui_object_primaryName", "IsChecked", &bCheck);
if(bSuccess) {
  if(bCheck) {
    //Checked!!
  }
  else {
    //UnChecked!!
  }
}
else {
  //Fail
}
```

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
