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
	VtTermChars* chars = m_term->Chars();
	VtCursor* cursor = m_term->Cursor();
	VtScreen* screen = m_term->Screen();

	term_line* cline = chars->GetLine(cursor->Row());
	int width = chars->GetCharWidth(c);

	if (screen->IsWrapNext() && screen->CanWrap() && width > 0)
	{
		cline->append_attr(LATTR_WRAPPED);
		
		m_cursor->Down();

		cline = chars->GetLine(cursor->Row());
	}

	if (term->insert && width > 0)
	{
		m_ctrlTerm->insch(width);
	}
		
	if (m_term->GetSelState() != NO_SELECTION)
	{
		Postion cursplus = screen->GetCursor();
		m_ctrlTerm->incpos(cursplus);
		m_ctrlTerm->check_selection(screen->GetCursor(), cursplus);
	}

	/*m_ctrlTerm->check_trust_status(term, cline);

	int linecols = term->cols;
	if (cline->trusted)
	{
		linecols -= TRUST_SIGIL_WIDTH;
	}*/

	switch (width) 
	{
	case 2:
		Display_W2();
		break;
	case 1:
		Display_W1();
		break;
	case 0:
		Display_W0();
		return;
	default:
		return;
	}

	After_Display();
	
	seen_disp_event(term);
}

void VtParserTopLevel::Display_W0()
{
	VtScreen* screen = m_term->Screen();
	VtCursor* cursor = m_term->Cursor();
	if (cursor->Col() <= 0)
	{
		return;
	}

	int x = cursor->Col() - 1;

	/* If we're in wrapnext state, the character to combine
	 * with is _here_, not to our left. */
	if (screen->IsWrapNext())
	{
		x++;
	}

	/*
	 * If the previous character is UCSWIDE, back up another
	 * one.
	 */
	if (cline->chars[x].chr == UCSWIDE) 
	{
		assert(x > 0);
		x--;
	}

	m_ctrlTerm->add_cc(cline, x, c);
	m_ctrlTerm->seen_disp_event(term);
}

void VtParserTopLevel::Display_W1()
{
	m_ctrlTerm->check_boundary(term->curs.x, term->curs.y);
	m_ctrlTerm->check_boundary(term->curs.x + 1, term->curs.y);

	VtTermChars* chars = m_term->Chars();
	VtCursor* cursor = m_term->Cursor();

	int row = cursor->Row();
	int col = cursor->Col();

	term_line* cline = chars->GetLine(row);
	cline->clear_cc(col);
	term_char tc = m_cur_chr;
	tc.set_chr(c);
	
	cline->set_char(col, m_cur_chr);
}

void VtParserTopLevel::Display_W2()
{
	VtTermChars* chars = m_term->Chars();
	VtCursor* cursor = m_term->Cursor();

	m_ctrlTerm->check_boundary(term, term->curs.x, term->curs.y);
	m_ctrlTerm->check_boundary(term, term->curs.x + 2, term->curs.y);

	if (term->curs.x >= linecols - 1) 
	{
		copy_termchar(cline, term->curs.x, &term->erase_char);
		cline->lattr |= LATTR_WRAPPED | LATTR_WRAPPED2;
		if (term->curs.y == term->marg_b)
		{
			scroll(term, term->marg_t, term->marg_b, 1, true);
		}
		else if (term->curs.y < term->rows - 1)
		{
			term->curs.y++;
		}
			
		term->curs.x = 0;
		cline = scrlineptr(term->curs.y);
		/* Now we must check_boundary again, of course. */
		check_boundary(term, term->curs.x, term->curs.y);
		check_boundary(term, term->curs.x + 2, term->curs.y);
	}

	

	int row = cursor->Row();
	int col = cursor->Col();

	term_line* cline = chars->GetLine(row);
	/* FULL-TERMCHAR */
	term_char tc = m_cur_chr;
	tc.set_chr(c);
	cline->set_char(col, m_cur_chr);

	col++;

	/* FULL-TERMCHAR */
	cline->clear_cc(col);

	term_char tc = m_cur_chr;
	tc.set_chr(UCSWIDE);
	cline->set_char(col, m_cur_chr);

	cursor->SetCol(col);
}

void VtParserTopLevel::After_Display()
{
	VtTermChars* chars = m_term->Chars();
	VtCursor* cursor = m_term->Cursor();

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
}
