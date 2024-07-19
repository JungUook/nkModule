#include "pch.h"
#include "nuklear_extensions.h"

#ifndef NK_ASSERT
#include <assert.h>
#define NK_ASSERT(expr) assert(expr)
#endif

void nk_label_bold(nk_context* ctx, const char* text, nk_flags alignment)
{
    struct nk_command_buffer* canvas = nk_window_get_canvas(ctx);
    struct nk_rect bounds = nk_widget_bounds(ctx);

    if (!nk_widget(&bounds, ctx))
        return;

    nk_draw_text(canvas, bounds, text, nk_strlen(text), ctx->style.font, nk_rgb(0, 0, 0), ctx->style.text.color);
}

void nk_label_underline(nk_context* ctx, const char* text, nk_flags alignment)
{
    nk_label(ctx, text, alignment);

    struct nk_command_buffer* canvas = nk_window_get_canvas(ctx);
    struct nk_rect bounds = nk_widget_bounds(ctx);
    struct nk_style* style = &ctx->style;

    float text_width = style->font->width(style->font->userdata, style->font->height, text, nk_strlen(text));
    nk_stroke_line(canvas, bounds.x, bounds.y + bounds.h - 1, bounds.x + text_width, bounds.y + bounds.h - 1, 1.0f, style->text.color);
}

void nk_label_strikethrough(nk_context* ctx, const char* text, nk_flags alignment)
{
    nk_label(ctx, text, alignment);

    struct nk_command_buffer* canvas = nk_window_get_canvas(ctx);
    struct nk_rect bounds = nk_widget_bounds(ctx);
    struct nk_style* style = &ctx->style;

    float text_width = style->font->width(style->font->userdata, style->font->height, text, nk_strlen(text));
    nk_stroke_line(canvas, bounds.x, bounds.y + bounds.h / 2, bounds.x + text_width, bounds.y + bounds.h / 2, 1.0f, style->text.color);
}