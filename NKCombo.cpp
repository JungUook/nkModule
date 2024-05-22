#include "pch.h"
#include "NKCombo.h"
#include "NKComboItem.h"

NKCombo::NKCombo()
	: m_currentLabel(0)
{
	m_type = eCOMBO;
	m_labelSize.x = 150.f;
	m_labelSize.y = 150.f;

	m_pivot.x = 0.f;
	m_pivot.y = 0.f;
	m_worldTransform.x = 50.f;
	m_worldTransform.y = 50.f;
	m_worldTransform.w = 150.f;
	m_worldTransform.h = 60.f;

	memset(m_content, 0, sizeof(m_content));
}

NKCombo::~NKCombo()
{
}

void NKCombo::Layout(nk_context* ctx)
{
	if (nk_combo_begin_label(ctx, m_content, m_labelSize))
	{
		nk_layout_space_begin(ctx, NK_STATIC, m_labelSize.y, m_pChildList.size());

		int i = 0;
		for (std::list<NKBase*>::iterator it = m_pChildList.begin(); it != m_pChildList.end(); ++it)
		{
			nk_layout_space_push(ctx, nk_rect(0, (m_labelSize.y / m_pChildList.size()) * i++, m_labelSize.x, m_labelSize.y / m_pChildList.size()));
			
			if ((*it)->GetType() == eCOMBO_ITEM)
			{
				NKComboItem* pItem = (NKComboItem*)(*it);
				pItem->SetLabel(i);
			}
			(*it)->Update(ctx);
		}
		nk_layout_space_end(ctx);
		nk_combo_end(ctx);
	}
}

void NKCombo::SetComboName(const char* name)
{
	strcpy_s(m_content, name);
}

void NKCombo::SetLabelSize(float x, float y)
{
	m_labelSize.x = x;
	m_labelSize.y = y;
}

void NKCombo::SetCurrentLabel(int number)
{
	m_currentLabel = number;
}
