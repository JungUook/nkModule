#include "pch.h"
#include "NKStyleText.h"
#include "NuklearUI.h"

NKStyleText::NKStyleText(nk_context* ctx, nk_style* style)
{
	color			 = &style->text.color;
	padding			 = &style->text.padding;
	color_factor	 = &style->text.color_factor;
	disabled_factor	 = &style->text.disabled_factor;

}

NKStyleText::~NKStyleText()
{
	color			 = nullptr;
	padding			 = nullptr;
	color_factor	 = nullptr;
	disabled_factor	 = nullptr;
}

void NKStyleText::EditComponentStyle(nk_context* ctx, NuklearUI* pManager)
{
	if (nk_tree_push(ctx, NK_TREE_NODE, "Text", NK_MINIMIZED)) {
		ColorPicker(ctx, *color);
		PropertyVector2(ctx, "padding", *padding, 0.f, 100.f, 1.f, 0.1f);
		nk_property_float(ctx, "#color_factor:", 0.f, color_factor, 1.f, 0.01f, 0.01f);
		nk_property_float(ctx, "#disabled_factor:", 0.f, disabled_factor, 1.f, 0.01f, 0.01f);
		nk_tree_pop(ctx);
	}
}
