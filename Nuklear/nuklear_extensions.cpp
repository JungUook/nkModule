#include "pch.h"
#include "nuklear_extensions.h"

#ifndef NK_ASSERT
#include <assert.h>
#define NK_ASSERT(expr) assert(expr)
#endif

void nk_label_bold(nk_context* ctx, std::string& text)
{
	text = "<b>" + text + "</b>";
}

void nk_label_outline(nk_context* ctx, std::string& text)
{
	text = "<o>" + text + "</o>";
}