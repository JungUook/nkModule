#include "pch.h"
#include "NKStyleOption.h"
#include "NuklearUI.h"

NKStyleOption::NKStyleOption(nk_context* ctx, nk_style* style)
{
	m_pComponent = new ComponentToggle(&style->option, &ctx->style.option);
}

NKStyleOption::~NKStyleOption()
{
	delete m_pComponent;
	m_pComponent = nullptr;
}

void NKStyleOption::EditComponentStyle(nk_context* ctx, NuklearUI* pManager)
{
	if (nk_tree_push(ctx, NK_TREE_NODE, "Option", NK_MINIMIZED)) {
		m_pComponent->CustomComponentsEditor(ctx, pManager);
		nk_tree_pop(ctx);
	}
}
