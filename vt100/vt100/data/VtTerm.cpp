#include "VtTerm.h"

VtTerm::VtTerm()
{
	
}

VtTerm::~VtTerm()
{

}

Size VtTerm::GetSize()
{
	return m_size;
}

VtCursor VtTerm::GetCursor()
{
	return m_cursor;
}

void VtTerm::CheckBoundary(int row, int col)
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
	ldata = scrlineptr(y);
	check_line_size(term, ldata);
	if (x == term->cols) {
		ldata->lattr &= ~LATTR_WRAPPED2;
	}
	else {
		if (ldata->chars[x].chr == UCSWIDE) {
			clear_cc(ldata, x - 1);
			clear_cc(ldata, x);
			ldata->chars[x - 1].chr = ' ' | CSET_ASCII;
			ldata->chars[x] = ldata->chars[x - 1];
		}
	}
}
