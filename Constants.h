#pragma once
#ifndef Constants_h__
#define Constants_h__

#ifdef _NKDEBUG
#define CHECK_PTR(ptr) \
    if ((ptr) == nullptr) { \
        std::cerr << "Error: Null " << __func__ <<" pointer passed to processPointer" << std::endl; \
        return; \
    }

#define CHECK_LUA_REF(ref) \
    if ((ref).isNil() || !(ref).isUserdata()) { \
        lua_State* L = (ref).state(); \
        lua_Debug ar; \
        if (lua_getstack(L, 1, &ar) && lua_getinfo(L, "Sl", &ar)) { \
            std::cerr << "Error: Invalid reference passed at " \
                      << ar.short_src << ":" << ar.currentline << std::endl; \
        } \
        return; \
    }

#else
#define CHECK_PTR(ptr) \
	if ((ptr) == nullptr) { \
		return; \
	}

#define CHECK_LUA_REF(ref) \
	if ((ref).isNil() || !(ref).isUserdata()) { \
        return; \
    }
#endif // _NKDEBUG

enum eTypeUI {
	eBASE = -1

	, eWINDOW = 0
	, eSPACE
	, eGROUP
	, ePOPUP
	, eCOMBO
	, eBUTTON
	, eEDIT
	, eIMAGE
	, eLABEL
	, eCOMBO_ITEM
	, eCHECKBOX
	, eSLIDER
	, ePROGRESS
	, eSELECTABLE
	, eTREE
	, eCHART
	, eCOLOR_PICKER
	, eTOOLTIP
	, eMENU
	, eSCROLLBAR
};

enum eTreeID {
	eTreeHeaderCloseButton = 1000,
	eTreeHeaderMinimizeButton = 2000,
	eTreeDefaultButton = 3000,
	eTreeContextualButton = 4000,
	eTreeMenuButton = 5000,
	eTreeOptionToggle = 6000,
	eTreeCheckboxToggle = 7000,
	eTreeSelectable = 8000,
	eTreeSlider = 9000,

	eTreeProgress = 1000 * 10,
	eTreeProperty = 1100 * 10,
	eTreeEdit = 1200 * 10,
	eTreeChart = 1300 * 10,
	eTreeScrollh = 1400 * 10,
	eTreeScrollv = 1500 * 10,
	eTreeTab = 1600 * 10,
	eTreeCombo = 1700 * 10,
};

//struct NKStyleItem {
//	std::string imagePath;
//	int option;
//	int sprIndex;
//	int sprSize;
//	int nineslice[4];
//	struct nk_style_item* target;
//	struct nk_style_item* restore;
//
//	NKStyleItem() : imagePath("None"), option(0), sprIndex(0), sprSize(0), target(nullptr), restore(nullptr) {
//		for (int i = 0; i < 4; ++i) {
//			nineslice[i] = 0;
//		}
//	}
//	void Init(struct nk_style_item* pTarget, struct nk_style_item* pRestore) {
//		target = pTarget;
//		restore = pRestore;
//	}
//};

//struct ComponentItem {
//	NKStyleItem normal;
//	NKStyleItem hover;
//	NKStyleItem active;
//	void Init(struct nk_style_item* pNormal, struct nk_style_item* pNormal_restore
//		, struct nk_style_item* pHover, struct nk_style_item* pHover_restore
//		, struct nk_style_item* pActive, struct nk_style_item* pActive_restore)
//	{
//		normal.Init(pNormal, pNormal_restore);
//		hover.Init(pHover, pHover_restore);
//		active.Init(pActive, pActive_restore);
//	}
//};

//struct ComponentButton {
//	NKStyleItem normal;
//	NKStyleItem hover;
//	NKStyleItem active;
//	void Init(struct nk_style_button* pTarget, struct nk_style_button* pRestore)
//	{
//		normal.Init(&pTarget->normal, &pRestore->normal);
//		hover.Init(&pTarget->hover, &pRestore->hover);
//		active.Init(&pTarget->active, &pRestore->active);
//	}
//};

//struct ComponentToggle {
//	NKStyleItem normal;
//	NKStyleItem hover;
//	NKStyleItem active;
//
//	NKStyleItem cursor_normal;
//	NKStyleItem cursor_hover;
//	void Init(struct nk_style_toggle* pTarget, struct nk_style_toggle* pRestore)
//	{
//		normal.Init(&pTarget->normal, &pRestore->normal);
//		hover.Init(&pTarget->hover, &pRestore->hover);
//		active.Init(&pTarget->active, &pRestore->active);
//
//		cursor_normal.Init(&pTarget->cursor_normal, &pRestore->cursor_normal);
//		cursor_hover.Init(&pTarget->cursor_hover, &pRestore->cursor_hover);
//	}
//};

//struct ComponentSelectable {
//	NKStyleItem normal;
//	NKStyleItem hover;
//	NKStyleItem pressed;
//
//	NKStyleItem normal_active;
//	NKStyleItem hover_active;
//	NKStyleItem pressed_active;
//	void Init(struct nk_style_selectable* pTarget, struct nk_style_selectable* pRestore)
//	{
//		normal.Init(&pTarget->normal, &pRestore->normal);
//		hover.Init(&pTarget->hover, &pRestore->hover);
//		pressed.Init(&pTarget->pressed, &pRestore->pressed);
//
//		normal_active.Init(&pTarget->normal_active, &pRestore->normal_active);
//		hover_active.Init(&pTarget->hover_active, &pRestore->hover_active);
//		pressed_active.Init(&pTarget->pressed_active, &pRestore->pressed_active);
//	}
//};

//struct ComponentSlider {
//	NKStyleItem normal;
//	NKStyleItem hover;
//	NKStyleItem active;
//
//	NKStyleItem cursor_normal;
//	NKStyleItem cursor_hover;
//	NKStyleItem cursor_active;
//
//	ComponentButton inc_button;
//	ComponentButton dec_button;
//
//	void Init(struct nk_style_slider* pTarget, struct nk_style_slider* pRestore)
//	{
//		normal.Init(&pTarget->normal, &pRestore->normal);
//		hover.Init(&pTarget->hover, &pRestore->hover);
//		active.Init(&pTarget->active, &pRestore->active);
//
//		cursor_normal.Init(&pTarget->cursor_normal, &pRestore->cursor_normal);
//		cursor_hover.Init(&pTarget->cursor_hover, &pRestore->cursor_hover);
//		cursor_active.Init(&pTarget->cursor_active, &pRestore->cursor_active);
//
//		inc_button.Init(&pTarget->inc_button, &pRestore->inc_button);
//		dec_button.Init(&pTarget->dec_button, &pRestore->dec_button);
//	}
//};

//struct ComponentProgress {
//	NKStyleItem normal;
//	NKStyleItem hover;
//	NKStyleItem active;
//
//	NKStyleItem cursor_normal;
//	NKStyleItem cursor_hover;
//	NKStyleItem cursor_active;
//	void Init(struct nk_style_progress* pTarget, struct nk_style_progress* pRestore)
//	{
//		normal.Init(&pTarget->normal, &pRestore->normal);
//		hover.Init(&pTarget->hover, &pRestore->hover);
//		active.Init(&pTarget->active, &pRestore->active);
//
//		cursor_normal.Init(&pTarget->cursor_normal, &pRestore->cursor_normal);
//		cursor_hover.Init(&pTarget->cursor_hover, &pRestore->cursor_hover);
//		cursor_active.Init(&pTarget->cursor_active, &pRestore->cursor_active);
//	}
//};

//struct ComponentScrollbar {
//	NKStyleItem normal;
//	NKStyleItem hover;
//	NKStyleItem active;
//
//	NKStyleItem cursor_normal;
//	NKStyleItem cursor_hover;
//	NKStyleItem cursor_active;
//
//	ComponentButton inc_button;
//	ComponentButton dec_button;
//
//	void Init(struct nk_style_scrollbar* pTarget, struct nk_style_scrollbar* pRestore)
//	{
//		normal.Init(&pTarget->normal, &pRestore->normal);
//		hover.Init(&pTarget->hover, &pRestore->hover);
//		active.Init(&pTarget->active, &pRestore->active);
//
//		cursor_normal.Init(&pTarget->cursor_normal, &pRestore->cursor_normal);
//		cursor_hover.Init(&pTarget->cursor_hover, &pRestore->cursor_hover);
//		cursor_active.Init(&pTarget->cursor_active, &pRestore->cursor_active);
//
//		inc_button.Init(&pTarget->inc_button, &pRestore->inc_button);
//		dec_button.Init(&pTarget->dec_button, &pRestore->dec_button);
//	}
//};

//struct ComponentEdit {
//	NKStyleItem normal;
//	NKStyleItem hover;
//	NKStyleItem active;
//
//	ComponentScrollbar scrollbar;
//	void Init(struct nk_style_edit* pTarget, struct nk_style_edit* pRestore)
//	{
//		normal.Init(&pTarget->normal, &pRestore->normal);
//		hover.Init(&pTarget->hover, &pRestore->hover);
//		active.Init(&pTarget->active, &pRestore->active);
//
//		scrollbar.Init(&pTarget->scrollbar, &pRestore->scrollbar);
//	}
//};

//struct ComponentProperty {
//	NKStyleItem normal;
//	NKStyleItem hover;
//	NKStyleItem active;
//
//	ComponentEdit edit;
//	ComponentButton inc_button;
//	ComponentButton dec_button;
//	void Init(struct nk_style_property* pTarget, struct nk_style_property* pRestore)
//	{
//		normal.Init(&pTarget->normal, &pRestore->normal);
//		hover.Init(&pTarget->hover, &pRestore->hover);
//		active.Init(&pTarget->active, &pRestore->active);
//
//		edit.Init(&pTarget->edit, &pRestore->edit);
//		inc_button.Init(&pTarget->inc_button, &pRestore->inc_button);
//		dec_button.Init(&pTarget->dec_button, &pRestore->dec_button);
//	}
//};

//struct ComponentTab {
//	NKStyleItem background;
//
//	ComponentButton tab_maximize_button;
//	ComponentButton tab_minimize_button;
//	ComponentButton node_maximize_button;
//	ComponentButton node_minimize_button;
//
//	void Init(struct nk_style_tab* pTarget, struct nk_style_tab* pRestore)
//	{
//		background.Init(&pTarget->background, &pRestore->background);
//
//		tab_maximize_button.Init(&pTarget->tab_maximize_button, &pRestore->tab_maximize_button);
//		tab_minimize_button.Init(&pTarget->tab_minimize_button, &pRestore->tab_minimize_button);
//		node_maximize_button.Init(&pTarget->node_maximize_button, &pRestore->node_maximize_button);
//		node_minimize_button.Init(&pTarget->node_minimize_button, &pRestore->node_minimize_button);
//	}
//};

//struct ComponentCombo {
//	NKStyleItem normal;
//	NKStyleItem hover;
//	NKStyleItem active;
//
//	ComponentButton button;
//
//	void Init(struct nk_style_combo* pTarget, struct nk_style_combo* pRestore)
//	{
//		normal.Init(&pTarget->normal, &pRestore->normal);
//		hover.Init(&pTarget->hover, &pRestore->hover);
//		active.Init(&pTarget->active, &pRestore->active);
//
//		button.Init(&pTarget->button, &pRestore->button);
//	}
//};

static void PropertyVector2(nk_context* ctx, const char* name, struct nk_vec2& vec, float max, float min, float step, float inc_per_pixel)
{
	nk_label(ctx, name, NK_TEXT_LEFT);
	nk_layout_row_dynamic(ctx, 22, 2);
	nk_property_float(ctx, "#X:", max, &vec.x, min, step, inc_per_pixel);
	nk_property_float(ctx, "#Y:", max, &vec.y, min, step, inc_per_pixel);
}
static void ColorPicker(nk_context* ctx, struct nk_color& color)
{
	struct nk_colorf colorf;

	colorf.a = ((float)color.a / 255.0f);
	colorf.r = ((float)color.r / 255.0f);
	colorf.g = ((float)color.g / 255.0f);
	colorf.b = ((float)color.b / 255.0f);

	nk_layout_row_dynamic(ctx, 300, 1);
	colorf = nk_color_picker(ctx, colorf, NK_RGBA);

	nk_layout_row_dynamic(ctx, 22, 1);
	nk_property_float(ctx, "#r:", 0.f, &colorf.r, 255, 0.01f, 0.01f);
	nk_property_float(ctx, "#g:", 0.f, &colorf.g, 255, 0.01f, 0.01f);
	nk_property_float(ctx, "#b:", 0.f, &colorf.b, 255, 0.01f, 0.01f);
	nk_property_float(ctx, "#a:", 0.f, &colorf.a, 255, 0.01f, 0.01f);

	color.a = ((nk_byte)(colorf.a * 255.0f));
	color.r = ((nk_byte)(colorf.r * 255.0f));
	color.g = ((nk_byte)(colorf.g * 255.0f));
	color.b = ((nk_byte)(colorf.b * 255.0f));
}
#endif //Constants_h__