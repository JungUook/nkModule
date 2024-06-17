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
#endif //Constants_h__