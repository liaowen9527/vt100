#include "VtParserTopLevel.h"



VtParserTopLevel::VtParserTopLevel()
{
	
}

VtParserTopLevel::~VtParserTopLevel()
{

}

/* 
*  Only graphic characters get this far;
*  ctrls are stripped above 
*/
void VtParserTopLevel::Parse()
{
	m_buffer->DisplayChar();

	termline *cline = scrlineptr(term->curs.y);
	int width = GetCharWidth(c);

	if (term->wrapnext && term->wrap && width > 0) 
	{
		cline->lattr |= LATTR_WRAPPED;
		if (term->curs.y == term->marg_b)
		{
			scroll(term, term->marg_t, term->marg_b, 1, TRUE);
		}
		else if (term->curs.y < term->rows - 1)
		{
			term->curs.y++;
		}
		term->curs.x = 0;
		term->wrapnext = FALSE;
		cline = scrlineptr(term->curs.y);
	}

	if (term->insert && width > 0)
	{
		insch(term, width);
	}
		
	if (term->selstate != NO_SELECTION) 
	{
		pos cursplus = term->curs;
		incpos(cursplus);
		check_selection(term, term->curs, cursplus);
	}
	if (((c & CSET_MASK) == CSET_ASCII ||
		(c & CSET_MASK) == 0) &&
		term->logctx)
	{
		logtraffic(term->logctx, (unsigned char)c, LGTYP_ASCII);
	}

	switch (width) 
	{
	case 2:
		/*
		 * If we're about to display a double-width
		 * character starting in the rightmost
		 * column, then we do something special
		 * instead. We must print a space in the
		 * last column of the screen, then wrap;
		 * and we also set LATTR_WRAPPED2 which
		 * instructs subsequent cut-and-pasting not
		 * only to splice this line to the one
		 * after it, but to ignore the space in the
		 * last character position as well.
		 * (Because what was actually output to the
		 * terminal was presumably just a sequence
		 * of CJK characters, and we don't want a
		 * space to be pasted in the middle of
		 * those just because they had the
		 * misfortune to start in the wrong parity
		 * column. xterm concurs.)
		 */
		check_boundary(term, term->curs.x, term->curs.y);
		check_boundary(term, term->curs.x + 2, term->curs.y);
		if (term->curs.x == term->cols - 1) {
			copy_termchar(cline, term->curs.x,
				&term->erase_char);
			cline->lattr |= LATTR_WRAPPED | LATTR_WRAPPED2;
			if (term->curs.y == term->marg_b)
				scroll(term, term->marg_t, term->marg_b,
					1, TRUE);
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

		term->curs.x++;

		/* FULL-TERMCHAR */
		clear_cc(cline, term->curs.x);
		cline->chars[term->curs.x].chr = UCSWIDE;
		cline->chars[term->curs.x].attr = term->curr_attr;

		break;
	case 1:
		check_boundary(term, term->curs.x, term->curs.y);
		check_boundary(term, term->curs.x + 1, term->curs.y);

		/* FULL-TERMCHAR */
		clear_cc(cline, term->curs.x);
		cline->chars[term->curs.x].chr = c;
		cline->chars[term->curs.x].attr = term->curr_attr;

		break;
	case 0:
		if (term->curs.x > 0) {
			int x = term->curs.x - 1;

			/* If we're in wrapnext state, the character
			 * to combine with is _here_, not to our left. */
			if (term->wrapnext)
				x++;

			/*
			 * If the previous character is
			 * UCSWIDE, back up another one.
			 */
			if (cline->chars[x].chr == UCSWIDE) {
				assert(x > 0);
				x--;
			}

			add_cc(cline, x, c);
			seen_disp_event(term);
		}
		continue;
	default:
		continue;
	}

	term->curs.x++;
	if (term->curs.x == term->cols) 
	{
		term->curs.x--;
		term->wrapnext = TRUE;
		if (term->wrap && term->vt52_mode) 
		{
			cline->lattr |= LATTR_WRAPPED;
			if (term->curs.y == term->marg_b)
				scroll(term, term->marg_t, term->marg_b, 1, TRUE);
			else if (term->curs.y < term->rows - 1)
				term->curs.y++;
			term->curs.x = 0;
			term->wrapnext = FALSE;
		}
	}
	seen_disp_event(term);
}