#pragma once
#ifndef nuklear_extensions_h_
#define nuklear_extensions_h_

#define NK_TEXT_BOLD       (1 << 0)
#define NK_TEXT_UNDERLINE  (1 << 1)

void nk_render_wrapped_label(struct nk_context* ctx, const char* text);

void nk_label_bold(struct nk_context* ctx, const char* text, nk_flags alignment);
void nk_label_underline(struct nk_context* ctx, const char* text, nk_flags alignment);
void nk_label_strikethrough(struct nk_context* ctx, const char* text, nk_flags alignment);
#endif // nuklear_extensions_h_