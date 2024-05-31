#pragma once
#ifndef NKBaseObject_h__
#define NKBaseObject_h__

#define NK_INCLUDE_FIXED_TYPES
//#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_DEFAULT_FONT
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_STANDARD_VARARGS_h__
#define NK_INCLUDE_DEFAULT_ALLOCATOR_h__
#define NK_BUTTON_TRIGGER_ON_RELEASE
#include <nuklear.h>
#include <list>
#include "LuaLibrary.h"
#include "LuaBridge/LuaBridge.h"
#include "NuklearUI.h"
#include "Constants.h"

class NuklearUI;
class NKBase
{
public:
	NKBase();
	NKBase(const NKBase& other);
	~NKBase();

	virtual std::string getClassName() const;

	//기본함수
public:
	virtual void Initialize(NuklearUI* pManager);
	virtual void Initialize(NKBase* pParent);
	virtual void Initialize();
	virtual void Update(nk_context* ctx);
	virtual void Layout(nk_context* ctx);
	virtual void SafeRenderStart();
	virtual void SafeRenderEnd();
	virtual void Release();

	//제어함수
public:
	virtual bool IsHovering();
	virtual void SetActive(bool bActive);
	virtual void SetHovering(bool bHovering);
	virtual void SetEdit(bool bEdit);
	virtual bool IsEditActive();

	virtual void SetContext(nk_context* ctx);
	virtual void SetStyle(nk_style* style);
	virtual void Setfont(nk_font* font);
	virtual void SetParent(NKBase* nkBase);
	virtual NKBase* GetParent();
	virtual std::list<NKBase*>* GetChildList();
	virtual void AddChild(NKBase* nkBase);
	virtual void LAddChild(luabridge::LuaRef ref);
	virtual void RemoveChildDisConnect(NKBase* nkBase);
	virtual void RemoveChild(NKBase* nkBase);
	virtual void LRemoveChild(luabridge::LuaRef ref);

	//속성관련 함수
public:
	virtual void SetPivot(float x, float y);
	virtual void SetPosition(float x, float y);
	virtual void SetSize(float width, float heigth);
	virtual void SetBackground(int SID);
	virtual struct nk_vec2 GetPivot();
	virtual struct nk_vec2 GetPosition();
	virtual struct nk_rect GetTransform();
	virtual float GetWidth();
	virtual float GetHeight();

	//각 객체의 기본값
public:
	virtual unsigned int GetPrimaryID();
	virtual const char* GetPrimaryName();
	virtual int GetNuklearIndex();
	virtual eTypeUI GetType();

	virtual void SetManager(NuklearUI* manager);
	virtual void SetPrimaryID(unsigned int id);
	virtual void SetPrimaryName(const char* name);
	virtual void SetNuklearIndex(int index);

	// ui 편집용 함수
public:
	virtual void LayoutEditor();
	virtual void EditInfo();
	virtual void EditStyle();

	virtual struct nk_vec2* EditPivot();
	virtual struct nk_rect* EditTransform();
	virtual void EditBaseName(const char* name);
	virtual const char* GetBaseName();

	virtual void CreateUI(const char* classname);
protected:
	NuklearUI* m_manager;

	unsigned int m_primaryID;
	char m_primaryName[64];

	char m_baseName[64];
	char m_editName[64];
	int m_editName_len;

	int m_nkIndex;
	nk_flags m_flags;

	nk_context* m_ctx;
	nk_font* m_font;

	eTypeUI m_type;
	struct nk_vec2 m_pivot;
	struct nk_vec2 m_position;
	struct nk_rect m_worldTransform;

	bool m_bActive;
	bool m_bHovering;
	bool m_bEditActive;

	NKBase* m_pParent;
	std::list<NKBase*> m_pChildList;

#pragma region Style Setup

protected:
	//header
	void HeaderEditor();
	void HeaderCloseButtonEditor();
	void HeaderMinimizeButtonEditor();

	//window
	void WindowEditor();
	void FixedBackgroundEditor();
	void BackgroundEditor();
	void ScalerEditor();
	void PropertiesEditor();
	void PopupEditor();
	void ComboEditor();
	void ContextualEditor();
	void MenuEditor();
	void GroupEditor();
	void TooltipEditor();

	//component
	void ComponentEditor();
	void TextComponentEditor();
	void ButtonComponentEditor();
	void ContextualButtonComponentEditor();
	void MenuButtonComponentEditor();
	void OptionComponentEditor();
	void CheckboxComponentEditor();
	void SelectableComponentEditor();
	void SliderComponentEditor();
	void ProgressComponentEditor();
	void PropertyComponentEditor();
	void EditComponentEditor();
	void ChartComponentEditor();
	void ScrollhComponentEditor();
	void ScrollvComponentEditor();
	void TabComponentEditor();
	void ComboComponentEditor();


	//make editor tool
	void CustomComponentsEditor(ComponentButton& tpi, struct nk_style_button& button, int id);
	void CustomComponentsEditor(ComponentToggle& tpi, struct nk_style_toggle& toggle, int id);
	void CustomComponentsEditor(ComponentSelectable& tpi, struct nk_style_selectable& selectable, int id);
	void CustomComponentsEditor(ComponentSlider& tpi, struct nk_style_slider& slider, int id);
	void CustomComponentsEditor(ComponentProgress& tpi, struct nk_style_progress& progress, int id);
	void CustomComponentsEditor(ComponentProperty& tpi, struct nk_style_property& property, int id);
	void CustomComponentsEditor(ComponentEdit& tpi, struct nk_style_edit& edit, int id);
	void CustomComponentsEditor(NKStyleItem& nsi, struct nk_style_chart& chart, int id);
	void CustomComponentsEditor(ComponentScrollbar& tpi, struct nk_style_scrollbar& scrollbar, int id);
	void CustomComponentsEditor(ComponentTab& tpi, struct nk_style_tab& tab, int id);
	void CustomComponentsEditor(ComponentCombo& tpi, struct nk_style_combo& combo, int id);
	void ItemEditor(NKStyleItem& sItem);
	void PropertyVector2(const char* name, struct nk_vec2& vec, float max, float min, float step, float inc_per_pixel);
	void ColorPicker(struct nk_color& color);

public:
	nk_style m_style;
	nk_style* m_pParentStyle;
	nk_bool m_followParentStyle;

	ComponentItem m_sHeader;
	ComponentButton m_sButton_close;
	ComponentButton m_sButton_minimize;

	NKStyleItem m_sBackground;
	NKStyleItem m_sScaler;

	ComponentButton m_sButton_default;
	ComponentButton m_sButton_contextual;
	ComponentButton m_sButton_menu;

	ComponentToggle m_sToggle_option;
	ComponentToggle m_sToggle_checkbox;
	ComponentSelectable m_sSelectable;
	ComponentSlider m_sSlider;
	ComponentProgress m_sProgress;
	ComponentProperty m_sProperty;
	ComponentEdit m_sEdit;
	NKStyleItem m_sChart;
	ComponentScrollbar m_sScrollbarh;
	ComponentScrollbar m_sScrollbarv;
	ComponentTab m_sTab;
	ComponentCombo m_sCombo;


#pragma endregion // Style Setup
};
#endif //NKBaseObject_h__