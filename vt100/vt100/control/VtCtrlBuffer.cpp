#include "VtCtrlErase.h"
#include "../VtContext.h"

VtCtrlBuffer::VtCtrlBuffer()
{
	
}

VtCtrlBuffer::~VtCtrlBuffer()
{

}

void VtCtrlBuffer::DisplayChar()
{
	termline *cline = scrlineptr(term->curs.y);
	int width = 0;
	if (DIRECT_CHAR(c))
		width = 1;
	if (!width)
		width = term_char_width(term, c);

	if (term->wrapnext && term->wrap && width > 0) {
		cline->lattr |= LATTR_WRAPPED;
		if (term->curs.y == term->marg_b)
			scroll(term, term->marg_t, term->marg_b, 1, true);
		else if (term->curs.y < term->rows - 1)
			term->curs.y++;
		term->curs.x = 0;
		term->wrapnext = false;
		cline = scrlineptr(term->curs.y);
	}
	if (term->insert && width > 0)
		insch(term, width);
	if (term->selstate != NO_SELECTION) {
		pos cursplus = term->curs;
		incpos(cursplus);
		check_selection(term, term->curs, cursplus);
	}
	if (((c & CSET_MASK) == CSET_ASCII ||
		(c & CSET_MASK) == 0) && term->logctx)
		logtraffic(term->logctx, (unsigned char)c, LGTYP_ASCII);

	check_trust_status(term, cline);

	int linecols = term->cols;
	if (cline->trusted)
		linecols -= TRUST_SIGIL_WIDTH;

	/*
	 * Preliminary check: if the terminal is only one character cell
	 * wide, then we cannot display any double-width character at all.
	 * Substitute single-width REPLACEMENT CHARACTER instead.
	 */
	if (width == 2 && linecols < 2) {
		width = 1;
		c = 0xFFFD;
	}

	switch (width) {
	case 2:
		/*
		 * If we're about to display a double-width character starting
		 * in the rightmost column, then we do something special
		 * instead. We must print a space in the last column of the
		 * screen, then wrap; and we also set LATTR_WRAPPED2 which
		 * instructs subsequent cut-and-pasting not only to splice
		 * this line to the one after it, but to ignore the space in
		 * the last character position as well. (Because what was
		 * actually output to the terminal was presumably just a
		 * sequence of CJK characters, and we don't want a space to be
		 * pasted in the middle of those just because they had the
		 * misfortune to start in the wrong parity column. xterm
		 * concurs.)
		 */
		check_boundary(term, term->curs.x, term->curs.y);
		check_boundary(term, term->curs.x + 2, term->curs.y);
		if (term->curs.x >= linecols - 1) {
			copy_termchar(cline, term->curs.x,
				&term->erase_char);
			cline->lattr |= LATTR_WRAPPED | LATTR_WRAPPED2;
			if (term->curs.y == term->marg_b)
				scroll(term, term->marg_t, term->marg_b,
					1, true);
			else if (term->curs.y < term->rows - 1)
				term->curs.y++;
			term->curs.x = 0;
			cline = scrlineptr(term->curs.y);
			/* Now we must check_boundary again, of course. */
			check_boundary(term, term->curs.x, term->curs.y);
			check_boundary(term, term->curs.x + 2, term->curs.y);
		}

		/* FULL-TERMCHAR */
		clear_cc(cline, term->curs.x);
		cline->chars[term->curs.x].chr = c;
		cline->chars[term->curs.x].attr = term->curr_attr;
		cline->chars[term->curs.x].truecolour =
			term->curr_truecolour;

		term->curs.x++;

		/* FULL-TERMCHAR */
		clear_cc(cline, term->curs.x);
		cline->chars[term->curs.x].chr = UCSWIDE;
		cline->chars[term->curs.x].attr = term->curr_attr;
		cline->chars[term->curs.x].truecolour =
			term->curr_truecolour;

		break;
	case 1:
		check_boundary(term, term->curs.x, term->curs.y);
		check_boundary(term, term->curs.x + 1, term->curs.y);

		/* FULL-TERMCHAR */
		clear_cc(cline, term->curs.x);
		cline->chars[term->curs.x].chr = c;
		cline->chars[term->curs.x].attr = term->curr_attr;
		cline->chars[term->curs.x].truecolour =
			term->curr_truecolour;

		break;
	case 0:
		if (term->curs.x > 0) {
			int x = term->curs.x - 1;

			/* If we're in wrapnext state, the character to combine
			 * with is _here_, not to our left. */
			if (term->wrapnext)
				x++;

			/*
			 * If the previous character is UCSWIDE, back up another
			 * one.
			 */
			if (cline->chars[x].chr == UCSWIDE) {
				assert(x > 0);
				x--;
			}

			add_cc(cline, x, c);
			seen_disp_event(term);
		}
		return;
	default:
		return;
	}
	term->curs.x++;
	if (term->curs.x >= linecols) {
		term->curs.x = linecols - 1;
		term->wrapnext = true;
		if (term->wrap && term->vt52_mode) {
			cline->lattr |= LATTR_WRAPPED;
			if (term->curs.y == term->marg_b)
				scroll(term, term->marg_t, term->marg_b, 1, true);
			else if (term->curs.y < term->rows - 1)
				term->curs.y++;
			term->curs.x = 0;
			term->wrapnext = false;
		}
	}
	seen_disp_event(term);
}

void VtCtrlBuffer::InsertLines()
{
	Margin margin;
	margin.top = m_cursor->Row();
	margin.bottom = m_term->m_margin.bottom;

	if (margin.IsVaild())
	{
		int arg = m_args->GetArg(0, m_term->Rows(), 1);
		m_term->ScrollUp(margin, arg);
	}

	seen_disp_event(term);
}

void VtCtrlBuffer::DeleteLines()
{
	Margin margin;
	margin.top = m_cursor->Row();
	margin.bottom = m_term->m_margin.bottom;

	if (margin.IsVaild())
	{
		int arg = m_args->GetArg(0, m_term->Rows(), 1);
		m_term->ScrollDown_sb(margin, arg);
	}

	seen_disp_event(term);
}

void VtCtrlBuffer::InsertChars()
{
	int arg = m_args->GetArg(0, m_term->Cols(), 1);
	
	insch(term, def(term->esc_args[0], 1));
	seen_disp_event(term);
}

void VtCtrlBuffer::DeleteChars()
{
	int arg = m_args->GetArg(0, m_term->Cols(), 1);
	insch(term, -def(term->esc_args[0], 1));
	seen_disp_event(term);
}

void VtCtrlBuffer::ClearTabs()
{
	if (m_args->Count() == 1)
	{
		int arg = m_args->GetArg(0);
		if (arg == 0)
		{
			term->tabs[term->curs.x] = false;
		}
		else if (arg == 3)
		{
			for (int i = 0; i < term->cols; i++)
			{
				term->tabs[i] = false;
			}
		}
	}
}

void VtCtrlBuffer::WriteSpaces()
{
	/* XXX VTTEST says this is vt220, vt510 manual
	 * says vt100 */
	if (!CheckCompat(ANSIMIN))
	{
		return;
	}
	
	int n = m_args->GetArg(0, m_term->Cols(), 1);
	
	Postion cursplus;
	int p = term->curs.x;
	termline *cline = scrlineptr(term->curs.y);

	check_trust_status(term, cline);
	if (n > term->cols - term->curs.x)
	{
		n = term->cols - term->curs.x;
	}
		
	cursplus = term->curs;
	cursplus.x += n;
	check_boundary(term, term->curs.x, term->curs.y);
	check_boundary(term, term->curs.x + n, term->curs.y);
	check_selection(term, term->curs, cursplus);
	while (n--)
	{
		copy_termchar(cline, p++, &term->erase_char);
	}
		
	seen_disp_event(term);
}
