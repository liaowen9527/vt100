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

void VtTerm::SaveCursor()
{

}

void VtTerm::RestoreCursor()
{

}

void VtTerm::scroll(Terminal *term, int topline, int botline, int lines, bool sb)
{
	termline *line;
	int seltop, scrollwinsize;

	if (topline != 0 || term->alt_which != 0)
		sb = false;

	scrollwinsize = botline - topline + 1;

	if (lines < 0) {
		lines = -lines;
		if (lines > scrollwinsize)
			lines = scrollwinsize;
		while (lines-- > 0) {
			line = delpos234(term->screen, botline);
			resizeline(term, line, term->cols);
			clear_line(term, line);
			addpos234(term->screen, line, topline);

			if (term->selstart.y >= topline && term->selstart.y <= botline) {
				term->selstart.y++;
				if (term->selstart.y > botline) {
					term->selstart.y = botline + 1;
					term->selstart.x = 0;
				}
			}
			if (term->selend.y >= topline && term->selend.y <= botline) {
				term->selend.y++;
				if (term->selend.y > botline) {
					term->selend.y = botline + 1;
					term->selend.x = 0;
				}
			}
		}
	}
	else {
		if (lines > scrollwinsize)
			lines = scrollwinsize;
		while (lines-- > 0) {
			line = delpos234(term->screen, topline);
#ifdef TERM_CC_DIAGS
			cc_check(line);
#endif
			if (sb && term->savelines > 0) {
				int sblen = count234(term->scrollback);
				/*
				* We must add this line to the scrollback. We'll
				* remove a line from the top of the scrollback if
				* the scrollback is full.
				*/
				if (sblen == term->savelines) {
					unsigned char *cline;

					sblen--;
					cline = delpos234(term->scrollback, 0);
					sfree(cline);
				}
				else
					term->tempsblines += 1;

				addpos234(term->scrollback, compressline(line), sblen);

				/* now `line' itself can be reused as the bottom line */

				/*
				* If the user is currently looking at part of the
				* scrollback, and they haven't enabled any options
				* that are going to reset the scrollback as a
				* result of this movement, then the chances are
				* they'd like to keep looking at the same line. So
				* we move their viewpoint at the same rate as the
				* scroll, at least until their viewpoint hits the
				* top end of the scrollback buffer, at which point
				* we don't have the choice any more.
				*
				* Thanks to Jan Holmen Holsten for the idea and
				* initial implementation.
				*/
				if (term->disptop > -term->savelines && term->disptop < 0)
					term->disptop--;
			}
			resizeline(term, line, term->cols);
			clear_line(term, line);
			check_trust_status(term, line);
			addpos234(term->screen, line, botline);

			/*
			* If the selection endpoints move into the scrollback,
			* we keep them moving until they hit the top. However,
			* of course, if the line _hasn't_ moved into the
			* scrollback then we don't do this, and cut them off
			* at the top of the scroll region.
			*
			* This applies to selstart and selend (for an existing
			* selection), and also selanchor (for one being
			* selected as we speak).
			*/
			seltop = sb ? -term->savelines : topline;

			if (term->selstate != NO_SELECTION) {
				if (term->selstart.y >= seltop &&
					term->selstart.y <= botline) {
					term->selstart.y--;
					if (term->selstart.y < seltop) {
						term->selstart.y = seltop;
						term->selstart.x = 0;
					}
				}
				if (term->selend.y >= seltop && term->selend.y <= botline) {
					term->selend.y--;
					if (term->selend.y < seltop) {
						term->selend.y = seltop;
						term->selend.x = 0;
					}
				}
				if (term->selanchor.y >= seltop &&
					term->selanchor.y <= botline) {
					term->selanchor.y--;
					if (term->selanchor.y < seltop) {
						term->selanchor.y = seltop;
						term->selanchor.x = 0;
					}
				}
			}
		}
	}
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
			margin.PosDown(m_selection.start);
			margin.PosDown(m_selection.end);
			margin.PosDown(m_selanchor);
		}
	}
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
