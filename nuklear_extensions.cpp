#include "pch.h"
#include "nuklear_extensions.h"

#ifndef NK_ASSERT
#include <assert.h>
#define NK_ASSERT(expr) assert(expr)
#endif
#define MAX_BUFFER_SIZE 4096

void nk_render_wrapped_label(nk_context* ctx, const char* text)
{
    char text_buffer[MAX_BUFFER_SIZE] = { 0 };
    strncpy_s(text_buffer, MAX_BUFFER_SIZE, text, MAX_BUFFER_SIZE - 1);
    text_buffer[MAX_BUFFER_SIZE - 1] = '\0';

    char* line_start = text_buffer;
    char* line_end;

    while (*line_start) {
        // Find the next newline or carriage return character
        line_end = strpbrk(line_start, "\n\r");
        if (line_end == NULL) {
            // No more newline or carriage return characters
            nk_label_wrap(ctx, line_start);
            break;
        }
        else {
            *line_end = '\0'; // Temporarily null-terminate the current line
            nk_label_wrap(ctx, line_start);
            // Restore the newline or carriage return character
            *line_end = *line_end == '\n' ? '\n' : '\r';
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