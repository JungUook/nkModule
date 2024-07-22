#include "pch.h"
#include "nuklear_extensions.h"

#ifndef NK_ASSERT
#include <assert.h>
#define NK_ASSERT(expr) assert(expr)
#endif

void nk_render_wrapped_label(nk_context* ctx, const char* text)
{
    const char* line_start = text;
    const char* line_end;
    while (*line_start) {
        // Find the next newline or carriage return character
        line_end = strpbrk(line_start, "\n\r");
        if (line_end == NULL) {
            // No more newline or carriage return characters
            nk_label_wrap(ctx, line_start);
            break;
        }
        else {
            size_t line_length = line_end - line_start;
            char* line = (char*)malloc(line_length + 1);  // Allocate memory dynamically
            if (line) {
                strncpy_s(line, line_length + 1, line_start, line_length);
                line[line_length] = '\0';
                nk_label_wrap(ctx, line);
                free(line);  // Free allocated memory
            }
            // Skip over the newline or carriage return character
            line_start = line_end + 1;
            // If there is a '\r\n' or '\n\r' sequence, skip the second character as well
            if ((*line_end == '\r' && *line_start == '\n') || (*line_end == '\n' && *line_start == '\r')) {
                line_start++;
            }
        }
    }
}

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