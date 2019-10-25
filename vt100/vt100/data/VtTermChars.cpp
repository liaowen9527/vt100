#include "VtTermChars.h"
#include "VtLine.h"

VtTermChars::VtTermChars()
{

}

VtTermChars::~VtTermChars()
{

}

int VtTermChars::GetLines()
{
	return GetSbLines() + m_screen.get_lines();
}

int VtTermChars::GetSbLines()
{
	if (GetTerm()->HasAltScreen())
	{
		return m_scrollback.get_lines() + GetAltSblines() + m_screen.get_lines();
	}

	return m_scrollback.get_lines();
}

term_line* VtTermChars::GetLine(int row)
{
	int sblines = Scrollback()->get_lines();
	if (row < sblines)
	{
		return Scrollback()->get_line(row);
	}

	row -= sblines;
	if (HasAltScreen())
	{
		int alt_sblines = GetAltSblines();
		if (row < alt_sblines)
		{
			return AltScreen()->get_line(row);
		}

		row -= alt_sblines;
	}

	return Screen()->get_line(row);
}

term_line* VtTermChars::GetLine_by_term(int row)
{
	if (row >= 0)
	{
		return Screen()->get_line(row);
	}

	if (HasAltScreen())
	{
		// if the row in the alt screen
		row = row + GetAltSblines();
		if (row >= 0)
		{
			return AltScreen()->get_line(row);
		}
	}

	// in the scroll back
	row = Scrollback()->get_lines() + row;
	return Scrollback()->get_line(row);
}

term_lines* VtTermChars::Screen()
{
	return &m_screen;
}

term_lines* VtTermChars::AltScreen()
{
	return &m_alt_screen;
}

term_lines* VtTermChars::Scrollback()
{
	return &m_scrollback;
}

void VtTermChars::SetAltSblines(int count)
{
	m_alt_sblines = count;
}

int VtTermChars::GetAltSblines()
{
	return m_alt_sblines;
}

void VtTermChars::SwapLines()
{
	term_lines* lines = Screen();
	term_lines* alt_lines = AltScreen();

	lines->swap(alt_lines);
	m_alt_sblines = alt_lines->find_last_nonempty_line() + 1;
}

void VtTermChars::CheckBoundary(int row, int col)
{
	int row = 0;
	int col = 0;
	termline *ldata;

	/* Validate input coordinates, just in case. */
	if (!m_size.InsideCol(col))
	{
		return;
	}

	m_buffer.CheckLineSize(row);
	term_line* ldata = GetLine_by_term(row);
	
	check_line_size(term, ldata);
	if (x == term->cols) 
	{
		ldata->lattr &= ~LATTR_WRAPPED2;
	}
	else 
	{
		if (ldata->chars[x].chr == UCSWIDE) {
			clear_cc(ldata, x - 1);
			clear_cc(ldata, x);
			ldata->chars[x - 1].chr = ' ' | CSET_ASCII;
			ldata->chars[x] = ldata->chars[x - 1];
		}
	}
}
