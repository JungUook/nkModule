#pragma once
#ifndef nuklear_extensions_h_
#define nuklear_extensions_h_

#define NK_TEXT_BOLD       (1 << 0)
#define NK_TEXT_UNDERLINE  (1 << 1)

void nk_label_bold(struct nk_context* ctx, std::string& text);
void nk_label_outline(struct nk_context* ctx, std::string& text);
#endif // nuklear_extensions_h_