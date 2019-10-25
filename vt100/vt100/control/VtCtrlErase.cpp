#include "VtCtrlErase.h"
#include "../VtContext.h"

VtCtrlErase::VtCtrlErase()
{
	
}

VtCtrlErase::~VtCtrlErase()
{

}

void VtCtrlErase::Parts()
{
	unsigned int i = m_args->GetArg(0, 1);

	if (i == 3)
	{
		/* Erase Saved Lines (xterm)
		* This follows Thomas Dickey's xterm. */
		if (!term->no_remote_clearscroll)
		{
			ClearSB();
		}
	}
	else
	{
		i++;
		if (i > 3)
		{
			i = 0;
		}

		bool from_start = i & 2;
		bool to_end = i & 1;
		EraseLots(false, from_start, to_end);
	}

	if (term->scroll_on_disp)
	{
		term->disptop = 0;
	}

	seen_disp_event(term);
}

void VtCtrlErase::LineOrParts()
{
	unsigned int i = m_args->GetArg(0, 0) + 1;
	if (i > 3)
	{
		i = 0;
	}

	bool from_start = i & 2;
	bool to_end = i & 1;
	EraseLots(true, from_start, to_end);

	seen_disp_event(term);
}

void VtCtrlErase::EraseLots(bool line_only, bool from_begin, bool to_end)
{
	bool line_only;
	Postion start, end;
	bool erase_lattr;
	bool erasing_lines_from_top = false;

	if (line_only)
	{
		start.row = m_cursor->Row();
		start.col = 0;
		end.row = m_cursor->Row() + 1;
		end.col = 0;
		erase_lattr = false;
	}
	else
	{
		start.row = 0;
		start.col = 0;
		end.row = m_term->Rows();
		end.col = 0;
		erase_lattr = true;
	}

	/* This is the endpoint of the clearing operation that is not
	 * either the start or end of the line / screen. */
	Postion boundary;
	m_cursor->Get(boundary.row, boundary.col);

	if (!from_begin)
	{
		/*
		 * If we're erasing from the current char to the end of
		 * line/screen, then we take account of wrapnext, so as to
		 * maintain the invariant that writing a printing character
		 * followed by ESC[K should not overwrite the character you
		 * _just wrote_. That is, when wrapnext says the cursor is
		 * 'logically' at the very rightmost edge of the screen
		 * instead of just before the last printing char, ESC[K should
		 * do nothing at all, and ESC[J should clear the next line but
		 * leave this one unchanged.
		 *
		 * This adjusted position will also be the position we use for
		 * check_boundary (i.e. the thing we ensure isn't in the
		 * middle of a double-width printing char).
		 */
		if (term->wrapnext)
			incpos(boundary);

		start = boundary;
	}
	if (!to_end)
	{
		/*
		 * If we're erasing from the start of (at least) the line _to_
		 * the current position, then that is taken to mean 'inclusive
		 * of the cell under the cursor', which means we don't
		 * consider wrapnext at all: whether it's set or not, we still
		 * clear the cell under the cursor.
		 *
		 * Again, that incremented boundary position is where we
		 * should be careful of a straddling wide character.
		 */
		incpos(boundary);
		end = boundary;
	}
	if (!from_begin || !to_end)
	{
		check_boundary(term, boundary.x, boundary.y);
	}

	check_selection(term, start, end);

	/* Clear screen also forces a full window redraw, just in case. */
	if (start.y == 0 && start.x == 0 && end.y == term->rows)
	{
		term_invalidate(term);
	}

	/* Lines scrolled away shouldn't be brought back on if the terminal
	 * resizes. */
	if (start.y == 0 && start.x == 0 && end.x == 0 && erase_lattr)
		erasing_lines_from_top = true;

	if (term->erase_to_scrollback && erasing_lines_from_top) {
		/* If it's a whole number of lines, starting at the top, and
		 * we're fully erasing them, erase by scrolling and keep the
		 * lines in the scrollback. */
		int scrolllines = end.y;
		if (end.y == term->rows) {
			/* Shrink until we find a non-empty row.*/
			scrolllines = find_last_nonempty_line(term, term->screen) + 1;
		}
		if (scrolllines > 0)
			scroll(term, 0, scrolllines - 1, scrolllines, true);
	}
	else {
		termline *ldata = scrlineptr(start.y);
		check_trust_status(term, ldata);
		while (poslt(start, end)) {
			check_line_size(term, ldata);
			if (start.x == term->cols) {
				if (!erase_lattr)
					ldata->lattr &= ~(LATTR_WRAPPED | LATTR_WRAPPED2);
				else
					ldata->lattr = LATTR_NORM;
			}
			else {
				copy_termchar(ldata, start.x, &term->erase_char);
			}
			if (incpos(start) && start.y < term->rows) {
				ldata = scrlineptr(start.y);
				check_trust_status(term, ldata);
			}
		}
	}

	/* After an erase of lines from the top of the screen, we shouldn't
	 * bring the lines back again if the terminal enlarges (since the user or
	 * application has explicitly thrown them away). */
	if (erasing_lines_from_top && !(term->alt_which))
		term->tempsblines = 0;
}

void VtCtrlErase::ClearSB()
{

}

void VtCtrlErase::SetEraseChar()
{

}

void VtCtrlErase::UseBgcolorErase()
{

}
