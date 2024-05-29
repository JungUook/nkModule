#include "pch.h"
#include "NKWindow.h"

NKWindow::NKWindow() : NKBase()
{
	m_type = eWINDOW;
	m_flags = NK_WINDOW_TITLE;

	m_pivot.x = 0.f;
	m_pivot.y = 0.f;
	m_worldTransform.x = 50.f;
	m_worldTransform.y = 50.f;
	m_worldTransform.w = 300.f;
	m_worldTransform.h = 600.f;

	m_border = 0;
	m_movable = 0;
	m_scalable = 0;
	m_closable = 0;
	m_minimizable = 0;
	m_no_scrollbar = 0;
	m_title = 1;
	m_scroll_auto_hide = 0;
	m_background = 0;
	m_scale_left = 0;
	m_no_input = 0;

	m_bgImagePath = "None";
	m_bgSprIndex = 0;
	m_bgSprSize = 0;
	m_bbgCustom = false;
}

NKWindow::NKWindow(const NKWindow& other) : NKBase()
{
	m_type				= other.m_type;
	m_flags				= other.m_flags;
	m_pivot.x			= other.m_pivot.x;
	m_pivot.y			= other.m_pivot.y;
	m_worldTransform.x	= other.m_worldTransform.x;
	m_worldTransform.y	= other.m_worldTransform.y;
	m_worldTransform.w	= other.m_worldTransform.w;
	m_worldTransform.h	= other.m_worldTransform.h;
	m_border			= other.m_border;
	m_movable			= other.m_movable;
	m_scalable			= other.m_scalable;
	m_closable			= other.m_closable;
	m_minimizable		= other.m_minimizable;
	m_no_scrollbar		= other.m_no_scrollbar;
	m_title				= other.m_title;
	m_scroll_auto_hide	= other.m_scroll_auto_hide;
	m_background		= other.m_background;
	m_scale_left		= other.m_scale_left;
	m_no_input			= other.m_no_input;
	m_titlebgImagePath	= other.m_titlebgImagePath;
	m_titlebgSprIndex	= other.m_titlebgSprIndex;
	m_titlebgSprSize	= other.m_titlebgSprSize;
	m_btitlebgCustom	= other.m_btitlebgCustom;
	m_bgImagePath		= other.m_bgImagePath;
	m_bgSprIndex		= other.m_bgSprIndex;
	m_bgSprSize			= other.m_bgSprSize;
	m_bbgCustom			= other.m_bbgCustom;
}

NKWindow::~NKWindow()
{
}

void NKWindow::Layout(nk_context* ctx)
{
	m_bHovering = false;
	if (nk_begin(ctx, m_primaryName, m_worldTransform, m_flags))
	{
		for (std::list<NKBase*>::iterator it = m_pChildList.begin(); it != m_pChildList.end(); ++it)
		{
			(*it)->Update(ctx);
		}

		m_worldTransform = nk_window_get_bounds(ctx);
		if (nk_input_is_mouse_hovering_rect(&ctx->input, m_worldTransform))
		{
			m_bHovering = true;
		}
		GetPosition();
	}
	nk_end(ctx);
}

void NKWindow::SafeRenderStart()
{
	if (m_btitlebgCustom)
	{
		struct nk_image img;
		m_manager->GetSprite(m_titlebgImagePath.c_str(), m_titlebgSprIndex, img);
		m_style.window.header.normal = nk_style_item_image(img);
	}

	if (m_bbgCustom)
	{
		struct nk_image img;
		m_manager->GetSprite(m_bgImagePath.c_str(), m_bgSprIndex, img);
		m_style.window.fixed_background = nk_style_item_image(img);
	}
}

void NKWindow::SafeRenderEnd()
{
}

void NKWindow::EditInfo()
{
	if (nk_tree_push(m_ctx, NK_TREE_NODE, "Flag", NK_MINIMIZED)) {
		nk_checkbox_label(m_ctx, "BORDER", &m_border);
		nk_checkbox_label(m_ctx, "MOVABLE", &m_movable);
		nk_checkbox_label(m_ctx, "SCALABLE", &m_scalable);
		nk_checkbox_label(m_ctx, "CLOSABLE", &m_closable);
		nk_checkbox_label(m_ctx, "MINIMIZABLE", &m_minimizable);
		nk_checkbox_label(m_ctx, "NO_SCROLLBAR", &m_no_scrollbar);
		nk_checkbox_label(m_ctx, "TITLE", &m_title);
		nk_checkbox_label(m_ctx, "SCROLL_AUTO_HIDE", &m_scroll_auto_hide);
		nk_checkbox_label(m_ctx, "BACKGROUND", &m_background);
		nk_checkbox_label(m_ctx, "SCALE_LEFT", &m_scale_left);
		nk_checkbox_label(m_ctx, "NO_INPUT", &m_no_input);
		nk_tree_pop(m_ctx);
	}

	m_flags = 0;
	if (m_border)
		m_flags |= NK_WINDOW_BORDER;
	if (m_movable)
		m_flags |= NK_WINDOW_MOVABLE;
	if (m_scalable)
		m_flags |= NK_WINDOW_SCALABLE;
	if (m_closable)
		m_flags |= NK_WINDOW_CLOSABLE;
	if (m_minimizable)
		m_flags |= NK_WINDOW_MINIMIZABLE;
	if (m_no_scrollbar)
		m_flags |= NK_WINDOW_NO_SCROLLBAR;
	if (m_title)
		m_flags |= NK_WINDOW_TITLE;
	if (m_scroll_auto_hide)
		m_flags |= NK_WINDOW_SCROLL_AUTO_HIDE;
	if (m_background)
		m_flags |= NK_WINDOW_BACKGROUND;
	if (m_scale_left)
		m_flags |= NK_WINDOW_SCALE_LEFT;
	if (m_no_input)
		m_flags |= NK_WINDOW_NO_INPUT;

	if (nk_tree_push(m_ctx, NK_TREE_NODE, "Create_UI", NK_MINIMIZED)) {
		if (nk_button_label(m_ctx, "Space"))
		{
			CreateUI("NKSpace");
		}
		nk_tree_pop(m_ctx);
	}

	if (nk_tree_push(m_ctx, NK_TREE_NODE, "Select_Title_BG", NK_MINIMIZED)) {
		auto mapSpr = m_manager->GetSprMap();
		int size = mapSpr->size();

		nk_layout_row_dynamic(m_ctx, 22, 1);
		nk_property_int(m_ctx, "#Index:", 0, &m_titlebgSprIndex, m_titlebgSprSize - 1, 1, 1);
		nk_layout_row_dynamic(m_ctx, 22, 2);
		nk_label(m_ctx, "Selected:", NK_TEXT_LEFT);
		std::filesystem::path filePath(m_titlebgImagePath.c_str());
		nk_label(m_ctx, filePath.filename().string().c_str(), NK_TEXT_RIGHT);
		if (nk_button_label(m_ctx, "apply"))
		{
			m_btitlebgCustom = true;
		}
		if (nk_button_label(m_ctx, "clear"))
		{
			m_btitlebgCustom = false;
			m_style.window.header.normal = m_ctx->style.window.header.normal;
		}

		if (size > 0)
		{
			nk_layout_row_dynamic(m_ctx, 150 + 22 * size, 1);
			if (nk_group_begin(m_ctx, "SPR List", NK_WINDOW_TITLE)) {

				float ratio[2] = { 0.8f, 0.2f };
				nk_layout_row(m_ctx, NK_DYNAMIC, 22, 2, ratio);
				int selected = 0;

				for (auto it = mapSpr->begin(); it != mapSpr->end(); ++it) {
					std::filesystem::path filePath((*it).first.c_str());
					nk_label(m_ctx, filePath.filename().string().c_str(), NK_TEXT_LEFT);

					if (nk_button_label(m_ctx, "Load")) {
						m_titlebgImagePath = (*it).first;
						m_titlebgSprSize = ((*it).second)->GetSpr()->GetXCount() * ((*it).second)->GetSpr()->GetYCount();
						if (m_titlebgSprSize <= m_titlebgSprIndex) {
							m_titlebgSprIndex = 0;
						}
					}
				}
				nk_group_end(m_ctx);
			}
		}
		nk_tree_pop(m_ctx);
	}

	if (nk_tree_push(m_ctx, NK_TREE_NODE, "Select_BG", NK_MINIMIZED)) {
		auto mapSpr = m_manager->GetSprMap();
		int size = mapSpr->size();

		nk_layout_row_dynamic(m_ctx, 22, 1);
		nk_property_int(m_ctx, "#Index:", 0, &m_bgSprIndex, m_bgSprSize - 1, 1, 1);
		nk_layout_row_dynamic(m_ctx, 22, 2);
		nk_label(m_ctx, "Selected:", NK_TEXT_LEFT);
		std::filesystem::path filePath(m_bgImagePath.c_str());
		nk_label(m_ctx, filePath.filename().string().c_str(), NK_TEXT_RIGHT);
		if (nk_button_label(m_ctx, "apply"))
		{
			m_bbgCustom = true;
		}
		if (nk_button_label(m_ctx, "clear"))
		{
			m_bbgCustom = false;
			m_style.window.fixed_background = m_ctx->style.window.fixed_background;
		}

		if (size > 0)
		{
			nk_layout_row_dynamic(m_ctx, 150 + 22 * size, 1);
			if (nk_group_begin(m_ctx, "SPR List", NK_WINDOW_TITLE)) {

				float ratio[2] = { 0.8f, 0.2f};
				nk_layout_row(m_ctx, NK_DYNAMIC, 22, 2, ratio);
				int selected = 0;

				for (auto it = mapSpr->begin(); it != mapSpr->end(); ++it) {
					std::filesystem::path filePath((*it).first.c_str());
					nk_label(m_ctx, filePath.filename().string().c_str(), NK_TEXT_LEFT);

					if (nk_button_label(m_ctx, "Load")) {
						m_bgImagePath = (*it).first;
						m_bgSprSize = ((*it).second)->GetSpr()->GetXCount() * ((*it).second)->GetSpr()->GetYCount();
						if (m_bgSprSize <= m_bgSprIndex) {
							m_bgSprIndex = 0;
						}
					}
				}
				nk_group_end(m_ctx);
			}
		}
		nk_tree_pop(m_ctx);
	}
}
