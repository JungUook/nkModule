#pragma once
#ifndef NKStyle_h__
#define NKStyle_h__
#include "Constants.h"

class NuklearUI;

class NKStyle
{
public:
	NKStyle();
	NKStyle(const NKStyle& other);
	~NKStyle();

protected:
	void InitializeStyle(nk_context* ctx, NuklearUI* pManager);
	void InitializeStyle(nk_font* font, nk_style& style, nk_style* parentStyle);
	void InitializeStyle(nk_context* ctx);

	virtual void StyleUpdateStart(nk_context* ctx, nk_style& original, NKStyle* pParent);
	virtual void StyleUpdateEnd(nk_context* ctx, nk_style& original);

	virtual void SetStyle(nk_style* style);
	virtual void Setfont(nk_font* font);
	virtual void SetBackground(NuklearUI* pManager, int SID);


	//ui 편집용 함수
protected:
	void FollowParentStyle(nk_context* ctx, NKStyle* pParent);

protected:	
	nk_style m_style;
	nk_font* m_font;

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

protected:
	//header
	void HeaderEditor(nk_context* ctx, NuklearUI* pManager);
	void HeaderCloseButtonEditor(nk_context* ctx, NuklearUI* pManager);
	void HeaderMinimizeButtonEditor(nk_context* ctx, NuklearUI* pManager);

	//window
	void WindowEditor(nk_context* ctx, NuklearUI* pManager);
	void FixedBackgroundEditor(nk_context* ctx, NuklearUI* pManager);
	void BackgroundEditor(nk_context* ctx);
	void ScalerEditor(nk_context* ctx, NuklearUI* pManager);
	void PropertiesEditor(nk_context* ctx);
	void PopupEditor(nk_context* ctx);
	void ComboEditor(nk_context* ctx);
	void ContextualEditor(nk_context* ctx);
	void MenuEditor(nk_context* ctx);
	void GroupEditor(nk_context* ctx);
	void TooltipEditor(nk_context* ctx);

	//component
	void ComponentEditor(nk_context* ctx, NuklearUI* pManager);
	void TextComponentEditor(nk_context* ctx, NuklearUI* pManager);
	void ButtonComponentEditor(nk_context* ctx, NuklearUI* pManager);
	void ContextualButtonComponentEditor(nk_context* ctx, NuklearUI* pManager);
	void MenuButtonComponentEditor(nk_context* ctx, NuklearUI* pManager);
	void OptionComponentEditor(nk_context* ctx, NuklearUI* pManager);
	void CheckboxComponentEditor(nk_context* ctx, NuklearUI* pManager);
	void SelectableComponentEditor(nk_context* ctx, NuklearUI* pManager);
	void SliderComponentEditor(nk_context* ctx, NuklearUI* pManager);
	void ProgressComponentEditor(nk_context* ctx, NuklearUI* pManager);
	void PropertyComponentEditor(nk_context* ctx, NuklearUI* pManager);
	void EditComponentEditor(nk_context* ctx, NuklearUI* pManager);
	void ChartComponentEditor(nk_context* ctx, NuklearUI* pManager);
	void ScrollhComponentEditor(nk_context* ctx, NuklearUI* pManager);
	void ScrollvComponentEditor(nk_context* ctx, NuklearUI* pManager);
	void TabComponentEditor(nk_context* ctx, NuklearUI* pManager);
	void ComboComponentEditor(nk_context* ctx, NuklearUI* pManager);


	//make editor tool
	void CustomComponentsEditor(nk_context* ctx, NuklearUI* pManager, ComponentButton& tpi, struct nk_style_button& button, int id);
	void CustomComponentsEditor(nk_context* ctx, NuklearUI* pManager, ComponentToggle& tpi, struct nk_style_toggle& toggle, int id);
	void CustomComponentsEditor(nk_context* ctx, NuklearUI* pManager, ComponentSelectable& tpi, struct nk_style_selectable& selectable, int id);
	void CustomComponentsEditor(nk_context* ctx, NuklearUI* pManager, ComponentSlider& tpi, struct nk_style_slider& slider, int id);
	void CustomComponentsEditor(nk_context* ctx, NuklearUI* pManager, ComponentProgress& tpi, struct nk_style_progress& progress, int id);
	void CustomComponentsEditor(nk_context* ctx, NuklearUI* pManager, ComponentProperty& tpi, struct nk_style_property& property, int id);
	void CustomComponentsEditor(nk_context* ctx, NuklearUI* pManager, ComponentEdit& tpi, struct nk_style_edit& edit, int id);
	void CustomComponentsEditor(nk_context* ctx, NuklearUI* pManager, NKStyleItem& nsi, struct nk_style_chart& chart, int id);
	void CustomComponentsEditor(nk_context* ctx, NuklearUI* pManager, ComponentScrollbar& tpi, struct nk_style_scrollbar& scrollbar, int id);
	void CustomComponentsEditor(nk_context* ctx, NuklearUI* pManager, ComponentTab& tpi, struct nk_style_tab& tab, int id);
	void CustomComponentsEditor(nk_context* ctx, NuklearUI* pManager, ComponentCombo& tpi, struct nk_style_combo& combo, int id);

	void ColorPicker(nk_context* ctx, struct nk_color& color);
	void ItemEditor(nk_context* ctx, NuklearUI* pManager, NKStyleItem& sItem);
};
#endif //NKStyle_h__