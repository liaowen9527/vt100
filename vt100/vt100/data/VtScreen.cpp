#include "VtScreen.h"

VtScreen::VtScreen()
{

}

VtScreen::~VtScreen()
{

}

Postion VtScreen::GetCursor()
{
	return m_curs;
}

void VtScreen::SetCursor(const Postion& pos)
{
	m_curs = pos;
}

void VtScreen::GetCursor(int& row, int& col)
{
	row = m_curs.row;
	col = m_curs.col;
}

void VtScreen::SetCursor(int row, int col)
{
	m_curs.row = row;
	m_curs.col = col;
}

Margin VtScreen::GetMargin()
{
	return m_margin;
}

void VtScreen::SetMargin(const Margin& marg)
{
	m_margin = marg;
}

bool VtScreen::CanWrap()
{
	return m_wrap;
}

void VtScreen::SetCanWrap(bool val)
{
	m_wrap = val;
}

bool VtScreen::IsWrapNext()
{
	return m_wrapnext;
}

void VtScreen::SetWrapNext(bool val)
{
	m_wrapnext = val;
}

int VtScreen::GetCharset()
{
	return m_cset;
}

void VtScreen::SetCharset(int index)
{
	m_cset = index;
}

void VtScreen::GetMargin(int& top, int& bottom)
{
	top = m_margin.top;
	bottom = m_margin.bottom;
}

void VtScreen::SetMargin(int top, int bottom)
{
	m_margin.top = top;
	m_margin.bottom = bottom;
}

void VtScreen::Swap(VtScreen& _Right, bool keep_cur_pos)
{
	if (keep_cur_pos)
	{
		screen_tag temp = _Right;
		_Right = *this;

		temp.curs = this->curs;
		temp.savecurs = this->savecurs;
		temp.save_wnext = this->save_wnext;
		temp.save_cset = this->save_cset;
		temp.save_utf = this->save_utf;
		temp.save_sco_acs = this->save_sco_acs;
		temp.save_csattr = this->save_csattr;
		temp.save_attr = this->save_attr;

		*this = temp;
	}
	else
	{
		std::swap<screen_tag>(*this, _Right);
	}
}
