#include "VtCtrlCursor.h"
#include "../VtContext.h"

VtCtrlCursor::VtCtrlCursor()
{
 	
}

VtCtrlCursor::~VtCtrlCursor()
{

}

void VtCtrlCursor::Up()
{
	int offset = m_args->GetArg(0, m_term->Rows(), 1);
	m_cursor->Move(-offset, 0);

	seen_disp_event(term);
}

void VtCtrlCursor::Down()
{
	int offset = m_args->GetArg(0, m_term->Rows(), 1);
	m_cursor->Move(offset, 0);

	seen_disp_event(term);
}

void VtCtrlCursor::Forward()
{
	int offset = m_args->GetArg(0, m_term->Cols(), 1);
	m_cursor->Move(0, offset);

	seen_disp_event(term);
}

void VtCtrlCursor::Backward()
{
	int offset = m_args->GetArg(0, m_term->Cols(), 1);
	m_cursor->Move(0, -offset);

	seen_disp_event(term);
}

void VtCtrlCursor::Backspace()
{
	if (term->curs.x == 0 && (term->curs.y == 0 || !term->wrap))
		/* do nothing */;
	else if (term->curs.x == 0 && term->curs.y > 0)
		term->curs.x = term->cols - 1, term->curs.y--;
	else if (term->wrapnext)
		term->wrapnext = false;
	else
		term->curs.x--;
	seen_disp_event(term);
}

void VtCtrlCursor::DownAndCR()
{
	int offset = term->m_args.GetArg(0, term->m_size.cols, 1);
	term->m_cursor.Move(0, offset);

	seen_disp_event(term);
}

void VtCtrlCursor::UpAndCR()
{
	int offset = term->m_args.GetArg(0, term->m_size.rows, 1);
	term->m_cursor.MoveTo(term->m_cursor.Row() + offset, 0);

	seen_disp_event(term);
}

void VtCtrlCursor::Horizontal()
{
	int col = term->m_args.GetArg(0, term->m_size.cols, 1) - 1;
	term->m_cursor.MoveTo(term->m_cursor.Row(), col);

	seen_disp_event(term);
}

void VtCtrlCursor::Vertical()
{
	int arg = term->m_args.GetArg(0, term->m_size.rows, 1);
	int top = term->dec_om ? term->marg_t : 0;
	int row = top + arg - 1;

	term->m_cursor.MoveTo(row, term->m_cursor.Col(), term->dec_om);

	seen_disp_event(term);
}

void VtCtrlCursor::SetPosition()
{
	if (term->esc_nargs < 2)
		term->esc_args[1] = ARG_DEFAULT;

	int arg1 = term->m_args.GetArg(0, term->m_size.rows, 1);
	int arg2 = term->m_args.GetArg(1, term->m_size.cols, 1);

	int top = term->dec_om ? term->marg_t : 0;
	int row = top + arg1 - 1;
	int col = arg2 - 1;

	term->m_cursor.MoveTo(row, col, term->dec_om);

	seen_disp_event(term);
}

void VtCtrlCursor::CSI_Z()
{
	if (!CheckCompat(OTHER))
	{
		return;
	}

	int arg = m_args->GetArg(0, m_term->Cols(), 1);
	Postion temp = term->curs;

	for (int i = arg; i > 0 && temp.col > 0; i--)
	{
		do
		{
			temp.col--;
			if (temp.col <= 0)
			{
				break;
			}

			if (term->tabs[temp.col])
			{
				break;
			}
		} while (true);
	}

	term->curs = temp;

	check_selection(term, temp, term->curs);
}

void VtCtrlCursor::NextLine()
{
	VtCursor& cursor = m_context->terminal.Cursor();
	cursor.MoveTo(cursor.Row() + 1, 0, false);
}

void VtCtrlCursor::ShowOrHide()
{
	if (!CheckCompat(SCOANSI))
	{
		return;
	}
	
	int arg = m_args->GetArg(0);
	switch (arg)
	{
	case 0:  /* hide cursor */
		term->cursor_on = false;
		break;
	case 1:  /* restore cursor */
		term->big_cursor = false;
		term->cursor_on = true;
		break;
	case 2:  /* block cursor */
		term->big_cursor = true;
		term->cursor_on = true;
		break;
	}
}

void VtCtrlCursor::SetCursorOn()
{
	/*
	* set cursor start on scanline esc_args[0] and
	* end on scanline esc_args[1].If you set
	* the bottom scan line to a value less than
	* the top scan line, the cursor will disappear.
	*/
	if (!CheckCompat(SCOANSI))
	{
		return;
	}

	if (m_args->Count() >= 2) 
	{
		term->cursor_on = m_args->GetArg(0) <= m_args->GetArg(1);
	}
}

void VtCtrlCursor::Save()
{
	VtCursor& cursor = m_context->terminal.Cursor();
	cursor.Save();
}

void VtCtrlCursor::Restore()
{
	VtCursor& cursor = m_context->terminal.Cursor();
	cursor.Restore();
}
