#include "VtTerm.h"

VtTerm::VtTerm()
{
	
}

VtTerm::~VtTerm()
{

}

VtScreen* VtTerm::Screen()
{
	return &m_screen;
}

VtCursor* VtTerm::Cursor()
{
	return nullptr;
}

VtArgument* VtTerm::Argument()
{
	return nullptr;
}

VtLdisc* VtTerm::Ldisc()
{
	return &m_ldisc;
}

VtTermChars* VtTerm::Chars()
{
	return &m_chars;
}

VtCharset* VtTerm::Charset()
{
	return &m_charset;
}

Termstate VtTerm::GetTermState()
{
	return m_termstate;
}

void VtTerm::SetTermState(Termstate state)
{
	m_termstate = state;
}

bool VtTerm::IsVt52Mode()
{
	return m_vt52_mode;
}

void VtTerm::SetVt52Mode(bool val)
{
	m_vt52_mode = val;
}

int VtTerm::GetEscQuery()
{
	return m_esc_query;
}

void VtTerm::SetEscQuery(int val)
{
	m_esc_query = val;
}

int VtTerm::GetDispTop()
{
	return m_disptop;
}

void VtTerm::SetDispTop(int val)
{
	m_disptop = val;
}

SelState VtTerm::GetSelState()
{
	return m_selstate;
}

bool VtTerm::CrHasLf()
{
	return m_crhaslf;
}

void VtTerm::SetCrHasLf(bool val)
{
	m_crhaslf = val;
}

bool VtTerm::IsScrollOnDisp()
{
	return m_scroll_on_disp;
}

void VtTerm::SetScrollOnDisp(bool val)
{
	m_scroll_on_disp = val;
}

int VtTerm::Rows()
{
	return m_size.rows;
}

int VtTerm::Cols()
{
	return m_size.cols;
}

bool VtTerm::HasAltScreen()
{
	return m_alt_which != 0;
}

void VtTerm::SaveCursor()
{

}

void VtTerm::RestoreCursor()
{

}

void VtTerm::ScrollUp(Margin margin, int lines)
{
	lines = std::min<int>(lines, margin.Lines() + 1);

	while (lines-- > 0) 
	{
		VtTermLine line = m_buffer.PopBack();
		line.Resize(m_size.cols);
		line.Clear();
		m_buffer.PushFront(line);

		margin.PosDown(m_selection.start);
		margin.PosDown(m_selection.end);
	}
}

void VtTerm::ScrollDown(Margin margin, int lines)
{
	lines = std::min<int>(lines, margin.Lines() + 1);
	
	while (lines-- > 0) 
	{
		VtTermLine line = m_buffer.PopFront();
		line.Resize(m_size.cols);
		line.Clear();
		m_buffer.PushBack(line);

		if (m_selstate != NO_SELECTION)
		{
			margin.PosUp(m_selection.start);
			margin.PosUp(m_selection.end);
			margin.PosUp(m_selanchor);
		}
	}
}

void VtTerm::ScrollDown_sb(Margin margin, int lines)
{
	if (margin.top != 0 || term->alt_which != 0)
	{
		ScrollDown(margin, lines);
		return;
	}

	lines = std::min<int>(lines, margin.Lines() + 1);

	while (lines-- > 0)
	{
		VtTermLine line = m_buffer.PopFront();

		if (term->savelines > 0)
		{
			int sblen = m_scrollback.GetLines();
			if (sblen == term->savelines)
			{
				m_scrollback.PopFront();
			}
			else
			{
				term->tempsblines += 1;
			}

			m_scrollback.PushFront(line);
			if (term->disptop > -term->savelines && term->disptop < 0)
			{
				term->disptop--;
			}
		}
		
		line.Resize(m_size.cols);
		line.Clear();
		m_buffer.PushBack(line);

		Margin selMargin = margin;
		selMargin.top = -term->savelines;

		if (m_selstate != NO_SELECTION)
		{
			selMargin.PosUp(m_selection.start);
			selMargin.PosUp(m_selection.end);
			selMargin.PosUp(m_selanchor);
		}
	}

}

void VtTerm::PowerOn(bool clear)
{
	
}

void VtTerm::NoPaste()
{

}

void VtTerm::Deselect()
{
	 m_selstate = NO_SELECTION;
	 m_selection.start = m_selection.end = Postion(0, 0);
}

void VtTerm::SwapScreen(int which, bool& reset, bool keep_cur_pos)
{
	reset = (which == 0) ? true : reset;

	if (which != m_alt_which)
	{
		m_alt_which = which;

		Chars()->SwapLines();
		m_alt_screen.swap(m_screen);
	}
}

void VtTerm::CheckSelection(Postion from, Postion to)
{
	if (from.lt(m_selection.end) && m_selection.start.lt(to))
	{
		Deselect();
	}
}

int VtTerm::ToInsideRow(int row)
{
	int rows = m_size.rows;

	row = std::max<int>(0, row);
	return std::min<int>(row, rows - 1);
}

int VtTerm::ToInsideCol(int col)
{
	int cols = m_size.cols;

	col = std::max<int>(0, col);
	return std::min<int>(col, cols - 1);
}

bool VtTerm::IsVisibleRow(int row)
{
	return m_margin.IsVisibleRow(row);
}

int VtTerm::ToVisibleRow(int row)
{
	int top = m_margin.top;
	int bottom = m_margin.bottom;

	row = std::max<int>(0, top);
	return std::min<int>(row, bottom);
}
