#pragma once
#ifndef Constants_h__
#define Constants_h__

#include <cereal/types/vector.hpp>
#include <cereal/types/array.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/types/polymorphic.hpp>

#ifdef _NKDEBUG
#define CHECK_PTR(ptr) \
    if ((ptr) == nullptr) { \
        std::cerr << "Error: Null " << __func__ <<" pointer passed to processPointer" << std::endl; \
        return; \
    }

#define CHECK_LUA_REF(ref) \
    if ((ref).isNil()) { \
        lua_State* L = (ref).state(); \
        lua_Debug ar; \
        if (lua_getstack(L, 1, &ar) && lua_getinfo(L, "Sl", &ar)) { \
            std::cerr << "Error: Invalid reference passed at " \
                      << ar.short_src << ":" << ar.currentline << std::endl; \
        } \
        return; \
    }

#define CHECK_LUA_REF_RETURN(ref) \
    if ((ref).isNil()) { \
        lua_State* L = (ref).state(); \
        lua_Debug ar; \
        if (lua_getstack(L, 1, &ar) && lua_getinfo(L, "Sl", &ar)) { \
            std::cerr << "Error: Invalid reference passed at " \
                      << ar.short_src << ":" << ar.currentline << std::endl; \
        } \
        return nullptr; \
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
	eNONE = -1

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
	, eSUPERSTYLE
};

static void PropertyVector2(nk_context* ctx, const char* name, struct nk_vec2& vec, float max, float min, float step, float inc_per_pixel)
{
	nk_label(ctx, name, NK_TEXT_LEFT);
	nk_layout_row_dynamic(ctx, 22, 2);
	nk_property_float(ctx, "#X:", max, &vec.x, min, step, inc_per_pixel);
	nk_property_float(ctx, "#Y:", max, &vec.y, min, step, inc_per_pixel);
}
static void PropertyTransform2(nk_context* ctx, const char* name, struct nk_rect& rect, float max, float min, float step, float inc_per_pixel)
{
	nk_layout_row_dynamic(ctx, 22, 1);
	nk_label(ctx, name, NK_TEXT_LEFT);
	nk_layout_row_dynamic(ctx, 22, 2);
	nk_property_float(ctx, "#X:", max, &rect.x, min, step, inc_per_pixel);
	nk_property_float(ctx, "#Y:", max, &rect.y, min, step, inc_per_pixel);
	nk_property_float(ctx, "#W:", 0.f, &rect.w, min, step, inc_per_pixel);
	nk_property_float(ctx, "#H:", 0.f, &rect.h, min, step, inc_per_pixel);
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

namespace cereal {

	template <class Archive>
	void serialize(Archive& ar, struct nk_vec2& vec2, const unsigned int version) {
		ar(CEREAL_NVP(vec2.x), CEREAL_NVP(vec2.y));
	}

	template <class Archive>
	void serialize(Archive& ar, struct nk_rect& rect, const unsigned int version) {
		ar(CEREAL_NVP(rect.x), CEREAL_NVP(rect.y), CEREAL_NVP(rect.w), CEREAL_NVP(rect.h));
	}

	template <class Archive>
	void serialize(Archive& ar, struct nk_color& color, const unsigned int version) {
		ar(CEREAL_NVP(color.r), CEREAL_NVP(color.g), CEREAL_NVP(color.b), CEREAL_NVP(color.a));
	}

	template <class Archive>
	void serialize(Archive& ar, struct nk_colorf& color, const unsigned int version) {
		ar(CEREAL_NVP(color.r), CEREAL_NVP(color.g), CEREAL_NVP(color.b), CEREAL_NVP(color.a));
	}

	template <class Archive>
	void serialize(Archive& ar, struct nk_image& img, const unsigned int version) {
		if (version >= 3) {
			ar(CEREAL_NVP(img.w)
				, CEREAL_NVP(img.h)
				, CEREAL_NVP(img.region)
				, CEREAL_NVP(img.color));
		}
		else {
			ar(CEREAL_NVP(img.w)
				, CEREAL_NVP(img.h)
				, CEREAL_NVP(img.region));
		}
	}

	template <class Archive>
	void serialize(Archive& ar, struct nk_nine_slice& nslice, const unsigned int version) {
		ar(CEREAL_NVP(nslice.img)
			, CEREAL_NVP(nslice.l)
			, CEREAL_NVP(nslice.t)
			, CEREAL_NVP(nslice.r)
			, CEREAL_NVP(nslice.b));
	}

	template <class Archive>
	void serialize(Archive& ar, union nk_style_item_data& data, const unsigned int version) {
		ar(CEREAL_NVP(data.color)
			, CEREAL_NVP(data.image)
			, CEREAL_NVP(data.slice));
	}

	template <class Archive>
	void serialize(Archive& ar, struct nk_style_item& item, const unsigned int version) {
		ar(CEREAL_NVP(item.type)
			, CEREAL_NVP(item.data));
	}


	template <class Archive>
	void serialize(Archive& ar, struct nk_style_toggle& toggle, const unsigned int version) {
		ar(
			CEREAL_NVP(toggle.normal),
			CEREAL_NVP(toggle.hover),
			CEREAL_NVP(toggle.active),
			CEREAL_NVP(toggle.border_color),
			CEREAL_NVP(toggle.cursor_normal),
			CEREAL_NVP(toggle.cursor_hover),
			CEREAL_NVP(toggle.text_normal),
			CEREAL_NVP(toggle.text_hover),
			CEREAL_NVP(toggle.text_active),
			CEREAL_NVP(toggle.text_background),
			CEREAL_NVP(toggle.text_alignment),
			CEREAL_NVP(toggle.padding),
			CEREAL_NVP(toggle.touch_padding),
			CEREAL_NVP(toggle.spacing),
			CEREAL_NVP(toggle.border),
			CEREAL_NVP(toggle.color_factor),
			CEREAL_NVP(toggle.disabled_factor)
		);
	}

	template <class Archive>
	void serialize(Archive& ar, struct nk_style_button& button, const unsigned int version) {
		ar(
			CEREAL_NVP(button.normal),
			CEREAL_NVP(button.hover),
			CEREAL_NVP(button.active),
			CEREAL_NVP(button.border_color),
			CEREAL_NVP(button.color_factor_background),
			CEREAL_NVP(button.text_background),
			CEREAL_NVP(button.text_normal),
			CEREAL_NVP(button.text_hover),
			CEREAL_NVP(button.text_active),
			CEREAL_NVP(button.text_alignment),
			CEREAL_NVP(button.color_factor_text),
			CEREAL_NVP(button.border),
			CEREAL_NVP(button.rounding),
			CEREAL_NVP(button.padding),
			CEREAL_NVP(button.image_padding),
			CEREAL_NVP(button.touch_padding),
			CEREAL_NVP(button.disabled_factor)
		);
	}

	template <class Archive>
	void serialize(Archive& ar, struct nk_style_tab& tab, const unsigned int version) {
		ar(
			CEREAL_NVP(tab.background),
			CEREAL_NVP(tab.border_color),
			CEREAL_NVP(tab.text),
			CEREAL_NVP(tab.tab_maximize_button),
			CEREAL_NVP(tab.tab_minimize_button),
			CEREAL_NVP(tab.node_maximize_button),
			CEREAL_NVP(tab.node_minimize_button),
			CEREAL_NVP(tab.sym_minimize),
			CEREAL_NVP(tab.sym_maximize),
			CEREAL_NVP(tab.border),
			CEREAL_NVP(tab.rounding),
			CEREAL_NVP(tab.indent),
			CEREAL_NVP(tab.padding),
			CEREAL_NVP(tab.spacing),
			CEREAL_NVP(tab.color_factor),
			CEREAL_NVP(tab.disabled_factor)
			);
	}

	template <class Archive>
	void serialize(Archive& ar, struct nk_style_slider& slider, const unsigned int version) {
		ar(
			CEREAL_NVP(slider.normal),
			CEREAL_NVP(slider.hover),
			CEREAL_NVP(slider.active),
			CEREAL_NVP(slider.border_color),
			CEREAL_NVP(slider.bar_normal),
			CEREAL_NVP(slider.bar_hover),
			CEREAL_NVP(slider.bar_active),
			CEREAL_NVP(slider.bar_filled),
			CEREAL_NVP(slider.cursor_normal),
			CEREAL_NVP(slider.cursor_hover),
			CEREAL_NVP(slider.cursor_active),
			CEREAL_NVP(slider.border),
			CEREAL_NVP(slider.rounding),
			CEREAL_NVP(slider.bar_height),
			CEREAL_NVP(slider.padding),
			CEREAL_NVP(slider.spacing),
			CEREAL_NVP(slider.cursor_size),
			CEREAL_NVP(slider.color_factor),
			CEREAL_NVP(slider.disabled_factor),
			CEREAL_NVP(slider.show_buttons),
			CEREAL_NVP(slider.inc_button),
			CEREAL_NVP(slider.dec_button),
			CEREAL_NVP(slider.inc_symbol),
			CEREAL_NVP(slider.dec_symbol)
		);
	}

	template <class Archive>
	void serialize(Archive& ar, struct nk_style_selectable& selectable, const unsigned int version) {
		ar(
			CEREAL_NVP(selectable.normal),
			CEREAL_NVP(selectable.hover),
			CEREAL_NVP(selectable.pressed),
			CEREAL_NVP(selectable.normal_active),
			CEREAL_NVP(selectable.hover_active),
			CEREAL_NVP(selectable.pressed_active),
			CEREAL_NVP(selectable.text_normal),
			CEREAL_NVP(selectable.text_hover),
			CEREAL_NVP(selectable.text_pressed),
			CEREAL_NVP(selectable.text_normal_active),
			CEREAL_NVP(selectable.text_hover_active),
			CEREAL_NVP(selectable.text_pressed_active),
			CEREAL_NVP(selectable.text_background),
			CEREAL_NVP(selectable.text_alignment),
			CEREAL_NVP(selectable.rounding),
			CEREAL_NVP(selectable.padding),
			CEREAL_NVP(selectable.touch_padding),
			CEREAL_NVP(selectable.image_padding),
			CEREAL_NVP(selectable.color_factor),
			CEREAL_NVP(selectable.disabled_factor)
		);
	}

	template <class Archive>
	void serialize(Archive& ar, struct nk_style_scrollbar& scrollbar, const unsigned int version) {
		ar(
			CEREAL_NVP(scrollbar.normal),
			CEREAL_NVP(scrollbar.hover),
			CEREAL_NVP(scrollbar.active),
			CEREAL_NVP(scrollbar.border_color),
			CEREAL_NVP(scrollbar.cursor_normal),
			CEREAL_NVP(scrollbar.cursor_hover),
			CEREAL_NVP(scrollbar.cursor_active),
			CEREAL_NVP(scrollbar.cursor_border_color),
			CEREAL_NVP(scrollbar.border),
			CEREAL_NVP(scrollbar.rounding),
			CEREAL_NVP(scrollbar.border_cursor),
			CEREAL_NVP(scrollbar.rounding_cursor),
			CEREAL_NVP(scrollbar.padding),
			CEREAL_NVP(scrollbar.color_factor),
			CEREAL_NVP(scrollbar.disabled_factor),
			CEREAL_NVP(scrollbar.show_buttons),
			CEREAL_NVP(scrollbar.inc_button),
			CEREAL_NVP(scrollbar.dec_button),
			CEREAL_NVP(scrollbar.inc_symbol),
			CEREAL_NVP(scrollbar.dec_symbol)
		);
	}

	template <class Archive>
	void serialize(Archive& ar, struct nk_style_property& prop, const unsigned int version) {
		ar(
			CEREAL_NVP(prop.normal),
			CEREAL_NVP(prop.hover),
			CEREAL_NVP(prop.active),
			CEREAL_NVP(prop.border_color),
			CEREAL_NVP(prop.label_normal),
			CEREAL_NVP(prop.label_hover),
			CEREAL_NVP(prop.label_active),
			CEREAL_NVP(prop.sym_left),
			CEREAL_NVP(prop.sym_right),
			CEREAL_NVP(prop.border),
			CEREAL_NVP(prop.rounding),
			CEREAL_NVP(prop.padding),
			CEREAL_NVP(prop.color_factor),
			CEREAL_NVP(prop.disabled_factor),
			CEREAL_NVP(prop.edit),
			CEREAL_NVP(prop.inc_button),
			CEREAL_NVP(prop.dec_button)
		);
	}

	template <class Archive>
	void serialize(Archive& ar, struct nk_style_progress& progress, const unsigned int version) {
		ar(
			CEREAL_NVP(progress.normal),
			CEREAL_NVP(progress.hover),
			CEREAL_NVP(progress.active),
			CEREAL_NVP(progress.border_color),
			CEREAL_NVP(progress.cursor_normal),
			CEREAL_NVP(progress.cursor_hover),
			CEREAL_NVP(progress.cursor_active),
			CEREAL_NVP(progress.cursor_border_color),
			CEREAL_NVP(progress.rounding),
			CEREAL_NVP(progress.border),
			CEREAL_NVP(progress.cursor_border),
			CEREAL_NVP(progress.cursor_rounding),
			CEREAL_NVP(progress.padding),
			CEREAL_NVP(progress.color_factor),
			CEREAL_NVP(progress.disabled_factor)
		);
	}

	template <class Archive>
	void serialize(Archive& ar, struct nk_style_window_header& header, const unsigned int version) {
		ar(
			CEREAL_NVP(header.normal),
			CEREAL_NVP(header.hover),
			CEREAL_NVP(header.active),
			CEREAL_NVP(header.close_button),
			CEREAL_NVP(header.minimize_button),
			CEREAL_NVP(header.close_symbol),
			CEREAL_NVP(header.minimize_symbol),
			CEREAL_NVP(header.maximize_symbol),
			CEREAL_NVP(header.label_normal),
			CEREAL_NVP(header.label_hover),
			CEREAL_NVP(header.label_active),
			CEREAL_NVP(header.align),
			CEREAL_NVP(header.padding),
			CEREAL_NVP(header.label_padding),
			CEREAL_NVP(header.spacing)
		);
	}

	template <class Archive>
	void serialize(Archive& ar, struct nk_style_edit& edit, const unsigned int version) {
		ar(
			CEREAL_NVP(edit.normal),
			CEREAL_NVP(edit.hover),
			CEREAL_NVP(edit.active),
			CEREAL_NVP(edit.border_color),
			CEREAL_NVP(edit.scrollbar),
			CEREAL_NVP(edit.cursor_normal),
			CEREAL_NVP(edit.cursor_hover),
			CEREAL_NVP(edit.cursor_text_normal),
			CEREAL_NVP(edit.cursor_text_hover),
			CEREAL_NVP(edit.text_normal),
			CEREAL_NVP(edit.text_hover),
			CEREAL_NVP(edit.text_active),
			CEREAL_NVP(edit.selected_normal),
			CEREAL_NVP(edit.selected_hover),
			CEREAL_NVP(edit.selected_text_normal),
			CEREAL_NVP(edit.selected_text_hover),
			CEREAL_NVP(edit.border),
			CEREAL_NVP(edit.rounding),
			CEREAL_NVP(edit.cursor_size),
			CEREAL_NVP(edit.scrollbar_size),
			CEREAL_NVP(edit.padding),
			CEREAL_NVP(edit.row_padding),
			CEREAL_NVP(edit.color_factor),
			CEREAL_NVP(edit.disabled_factor)
		);
	}

	template <class Archive>
	void serialize(Archive& ar, struct nk_style_combo& combo, const unsigned int version) {
		ar(
			CEREAL_NVP(combo.normal),
			CEREAL_NVP(combo.hover),
			CEREAL_NVP(combo.active),
			CEREAL_NVP(combo.border_color),
			CEREAL_NVP(combo.label_normal),
			CEREAL_NVP(combo.label_hover),
			CEREAL_NVP(combo.label_active),
			CEREAL_NVP(combo.symbol_normal),
			CEREAL_NVP(combo.symbol_hover),
			CEREAL_NVP(combo.symbol_active),
			CEREAL_NVP(combo.button),
			CEREAL_NVP(combo.sym_normal),
			CEREAL_NVP(combo.sym_hover),
			CEREAL_NVP(combo.sym_active),
			CEREAL_NVP(combo.border),
			CEREAL_NVP(combo.rounding),
			CEREAL_NVP(combo.content_padding),
			CEREAL_NVP(combo.button_padding),
			CEREAL_NVP(combo.spacing),
			CEREAL_NVP(combo.color_factor),
			CEREAL_NVP(combo.disabled_factor)
		);
	}
}
#endif //Constants_h__