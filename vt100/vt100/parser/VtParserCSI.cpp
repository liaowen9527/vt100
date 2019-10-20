/*
https://www2.ccs.neu.edu/research/gpc/VonaUtils/vona/terminal/vtansi.htm
http://ascii-table.com/ansi-escape-sequences-vt-100.php
*/

#include "VtParserESC.h"
#include "../define/VtType.h"

VtParserCSI::VtParserCSI()
{
	
}

VtParserCSI::~VtParserCSI()
{

}

void VtParserCSI::Parse()
{
	term->termstate = TOPLEVEL;  /* default */

	if (isdigit(c)) 
	{
		Digit();
	}
	else if (c == ';') 
	{
		Semicolon();
	}
	else if (c < '@') 
	{
		Email();
	}
	else
	{
		Control();
	}
}

void VtParserCSI::Digit()
{
	if (term->esc_nargs <= ARGS_MAX) {
		if (term->esc_args[term->esc_nargs - 1] == ARG_DEFAULT)
			term->esc_args[term->esc_nargs - 1] = 0;
		if (term->esc_args[term->esc_nargs - 1] <=
			UINT_MAX / 10 &&
			term->esc_args[term->esc_nargs - 1] * 10 <=
			UINT_MAX - c - '0')
			term->esc_args[term->esc_nargs - 1] =
			10 * term->esc_args[term->esc_nargs - 1] +
			c - '0';
		else
			term->esc_args[term->esc_nargs - 1] = UINT_MAX;
	}
	term->termstate = SEEN_CSI;
}

void VtParserCSI::Semicolon()
{
	if (term->esc_nargs < ARGS_MAX)
		term->esc_args[term->esc_nargs++] = ARG_DEFAULT;
	term->termstate = SEEN_CSI;
}

void VtParserCSI::Email()
{
	if (term->esc_query)
		term->esc_query = -1;
	else if (c == '?')
		term->esc_query = TRUE;
	else
		term->esc_query = c;
	term->termstate = SEEN_CSI;
}

void VtParserCSI::Control()
{
	char ctrl = ANSI(c, term->esc_query);
	switch (ctrl)
	{
	case 'A':       /* CUU: move up N lines */
		MoveUp();
		break;
	case 'e':		/* VPR: move down N lines */
		ModeDown();
		break;
		/* FALLTHROUGH */
	case 'B':		/* CUD: Cursor down */
		CursorDown();
		break;
	case 'b':        /* REP: repeat previous grap */
		RepeateGrap();
		break;
	case ANSI('c', '>'):	/* DA: report xterm version */
		ReportXtermVersion();
		break;
	case 'a':		/* HPR: move right N cols */
		MoveRight();
		break;
		/* FALLTHROUGH */
	case 'C':		/* CUF: Cursor right */
		CursorRight();
		break;
	case 'D':       /* CUB: move left N cols */
		MoveLeft();
		break;
	case 'E':       /* CNL: move down N lines and CR */
		DownAndCR();
		break;
	case 'F':       /* CPL: move up N lines and CR */
		UpAndCR();
		break;
	case 'G':	      /* CHA */
	case '`':       /* HPA: set horizontal posn */
		Horizontal();
		break;
	case 'd':       /* VPA: set vertical posn */
		Vertical();
		break;
	case 'H':	     /* CUP */
	case 'f':      /* HVP: set horz and vert posns at once */
		Postion();
		break;
	case 'J':       /* ED: erase screen or parts of it */
		EraseParts();
		break;
	case 'K':       /* EL: erase line or parts of it */
		EraseLineOrParts();
		break;
	case 'L':       /* IL: insert lines */
		InsertLines();
		break;
	case 'M':       /* DL: delete lines */
		DeleteLines();
		break;
	case '@':       /* ICH: insert chars */
  /* XXX VTTEST says this is vt220, vt510 manual says vt102 */
		InsertChars();
		break;
	case 'P':       /* DCH: delete chars */
		DeleteChars();
		break;
	case 'c':       /* DA: terminal type query */
		TypeQuery();
		break;
	case 'n':       /* DSR: cursor position query */
		CursorPosQuery();
		break;
	case 'h':       /* SM: toggle modes to high */
	case ANSI_QUE('h'):
		ToggleModeH();
		break;
	case 'i':		/* MC: Media copy */
	case ANSI_QUE('i'):
		MediaCopy();
		break;
	case 'l':       /* RM: toggle modes to low */
	case ANSI_QUE('l'):
		ToggleModeL();
		break;
	case 'g':       /* TBC: clear tabs */
		ClearTabs();
		break;
	case 'r':       /* DECSTBM: set scroll margins */
		SetScollMargin();
		break;
	case 'm':       /* SGR: set graphics rendition */
		SetGraphics();
		break;
	case 's':       /* save cursor */
		SaveCursor();
		break;
	case 'u':       /* restore cursor */
		RestorCursor();
		break;
	case 't': /* DECSLPP: set page size - ie window height */
		SetPageSize();
		break;
	case 'S':		/* SU: Scroll up */
		ScrollUp();
		break;
	case 'T':		/* SD: Scroll down */
		ScrollDown();
		break;
	case ANSI('|', '*'): /* DECSNLS */
		SetLines();
		break;
	case ANSI('|', '$'): /* DECSCPP */
		SetColumns();
		break;
	case 'X':     /* ECH: write N spaces w/o moving cursor */
		WriteSpace();
		break;
	case 'x':       /* DECREQTPARM: report terminal characteristics */
		ReportCharacter();
		break;
	case 'Z':		/* CBT */
		CheckSelection();
		break;
	case ANSI('c', '='):      /* Hide or Show Cursor */
		ShowCursor();
		break;
	case ANSI('C', '='):
		SetCursorOn();
		break;
	case ANSI('D', '='):
		SetBlink();
		break;
	case ANSI('E', '='):
		BlinkIsReal();
		break;
	case ANSI('F', '='):      /* set normal foreground */
		Foreground();
		break;
	case ANSI('G', '='):      /* set normal background */
		Background();
		break;
	case ANSI('L', '='):
		SetEraseChar();
		break;
	case ANSI('p', '"'): /* DECSCL: set compat level */
		SetCompatLevel();
		break;
	}
}

void VtParserCSI::MoveUp()
{
	CLAMP(term->esc_args[0], term->rows);
	move(term, term->curs.x,
		term->curs.y - def(term->esc_args[0], 1), 1);
	seen_disp_event(term);
}

void VtParserCSI::ModeDown()
{
	if (!CheckCompat(ANSI)) return;

	CursorDown();
}

void VtParserCSI::MoveRight()
{
	if (!CheckCompat(ANSI)) return;

	CursorRight();
}

void VtParserCSI::MoveLeft()
{
	CLAMP(term->esc_args[0], term->cols);
	move(term, term->curs.x - def(term->esc_args[0], 1),
		term->curs.y, 1);
	seen_disp_event(term);
}

void VtParserCSI::CursorDown()
{
	CLAMP(term->esc_args[0], term->rows);
	move(term, term->curs.x,
		term->curs.y + def(term->esc_args[0], 1), 1);
	seen_disp_event(term);
}

void VtParserCSI::CursorRight()
{
	CLAMP(term->esc_args[0], term->cols);
	move(term, term->curs.x + def(term->esc_args[0], 1),
		term->curs.y, 1);
	seen_disp_event(term);
}

void VtParserCSI::DownAndCR()
{
	if (!CheckCompat(ANSI)) return;

	CLAMP(term->esc_args[0], term->rows);
	move(term, 0,
		term->curs.y + def(term->esc_args[0], 1), 1);
	seen_disp_event(term);
}

void VtParserCSI::UpAndCR()
{
	if (!CheckCompat(ANSI)) return;

	CLAMP(term->esc_args[0], term->rows);
	move(term, 0,
		term->curs.y - def(term->esc_args[0], 1), 1);
	seen_disp_event(term);
}

void VtParserCSI::Horizontal()
{
	if (!CheckCompat(ANSI)) return;

	CLAMP(term->esc_args[0], term->cols);
	move(term, def(term->esc_args[0], 1) - 1,
		term->curs.y, 0);
	seen_disp_event(term);
}

void VtParserCSI::Vertical()
{
	if (!CheckCompat(ANSI)) return;

	CLAMP(term->esc_args[0], term->rows);
	move(term, term->curs.x,
		((term->dec_om ? term->marg_t : 0) +
			def(term->esc_args[0], 1) - 1),
			(term->dec_om ? 2 : 0));
	seen_disp_event(term);
}

void VtParserCSI::Postion()
{
	if (term->esc_nargs < 2)
		term->esc_args[1] = ARG_DEFAULT;
	CLAMP(term->esc_args[0], term->rows);
	CLAMP(term->esc_args[1], term->cols);
	move(term, def(term->esc_args[1], 1) - 1,
		((term->dec_om ? term->marg_t : 0) +
			def(term->esc_args[0], 1) - 1),
			(term->dec_om ? 2 : 0));
	seen_disp_event(term);
}

void VtParserCSI::RepeateGrap()
{
	CLAMP(term->esc_args[0], term->rows * term->cols);
	if (term->last_graphic_char) {
		unsigned i;
		for (i = 0; i < term->esc_args[0]; i++)
			term_display_graphic_char(
				term, term->last_graphic_char);
	}
}

void VtParserCSI::ReportXtermVersion()
{
	if (!CheckCompat(OTHER)) return;

	/* this reports xterm version 136 so that VIM can
	use the drag messages from the mouse reporting */
	if (term->ldisc)
	{
		ldisc_send(term->ldisc, "\033[>0;136;0c", 11,
			false);
	}
}

void VtParserCSI::EraseParts()
{
	unsigned int i = def(term->esc_args[0], 0);
	if (i == 3) {
		/* Erase Saved Lines (xterm)
		* This follows Thomas Dickey's xterm. */
		if (!term->no_remote_clearscroll)
			term_clrsb(term);
	}
	else {
		i++;
		if (i > 3)
			i = 0;
		erase_lots(term, false, !!(i & 2), !!(i & 1));
	}

	if (term->scroll_on_disp)
		term->disptop = 0;
	seen_disp_event(term);
}

void VtParserCSI::EraseLineOrParts()
{
	{
		unsigned int i = def(term->esc_args[0], 0) + 1;
		if (i > 3)
			i = 0;
		erase_lots(term, true, !!(i & 2), !!(i & 1));
	}
	seen_disp_event(term);
}

void VtParserCSI::InsertLines()
{
	if (!CheckCompat(VT102)) return;

	CLAMP(term->esc_args[0], term->rows);
	if (term->curs.y <= term->marg_b)
		scroll(term, term->curs.y, term->marg_b,
			-def(term->esc_args[0], 1), false);
	seen_disp_event(term);
}

void VtParserCSI::DeleteLines()
{
	if (!CheckCompat(VT102)) return;

	CLAMP(term->esc_args[0], term->rows);
	if (term->curs.y <= term->marg_b)
		scroll(term, term->curs.y, term->marg_b,
			def(term->esc_args[0], 1),
			true);
	seen_disp_event(term);
}

void VtParserCSI::InsertChars()
{
	if (!CheckCompat(VT102)) return;

	CLAMP(term->esc_args[0], term->cols);
	insch(term, def(term->esc_args[0], 1));
	seen_disp_event(term);
}

void VtParserCSI::DeleteChars()
{
	if (!CheckCompat(VT102)) return;

	CLAMP(term->esc_args[0], term->cols);
	insch(term, -def(term->esc_args[0], 1));
	seen_disp_event(term);
}

void VtParserCSI::TypeQuery()
{
	if (!CheckCompat(VT100)) return;

	/* This is the response for a VT102 */
	if (term->ldisc && term->id_string[0])
		ldisc_send(term->ldisc, term->id_string,
			strlen(term->id_string), false);
}

void VtParserCSI::CursorPosQuery()
{
	if (term->ldisc) {
		if (term->esc_args[0] == 6) {
			char buf[32];
			sprintf(buf, "\033[%d;%dR", term->curs.y + 1,
				term->curs.x + 1);
			ldisc_send(term->ldisc, buf, strlen(buf),
				false);
		}
		else if (term->esc_args[0] == 5) {
			ldisc_send(term->ldisc, "\033[0n", 4, false);
		}
	}
}

void VtParserCSI::ToggleModeH()
{
	if (!CheckCompat(VT100)) return;

	{
		int i;
		for (i = 0; i < term->esc_nargs; i++)
			toggle_mode(term, term->esc_args[i],
				term->esc_query, true);
	}
}

void VtParserCSI::ToggleModeL()
{
	if (!CheckCompat(VT100)) return;

	{
		int i;
		for (i = 0; i < term->esc_nargs; i++)
			toggle_mode(term, term->esc_args[i],
				term->esc_query, false);
	}
}

void VtParserCSI::MediaCopy()
{
	if (!CheckCompat(VT100)) return;

	{
		char *printer;
		if (term->esc_nargs != 1) break;
		if (term->esc_args[0] == 5 &&
			(printer = conf_get_str(term->conf,
				CONF_printer))[0]) {
			term->printing = true;
			term->only_printing = !term->esc_query;
			term->print_state = 0;
			term_print_setup(term, printer);
		}
		else if (term->esc_args[0] == 4 &&
			term->printing) {
			term_print_finish(term);
		}
	}
}

void VtParserCSI::ClearTabs()
{
	if (!CheckCompat(VT100)) return;

	if (term->esc_nargs == 1) {
		if (term->esc_args[0] == 0) {
			term->tabs[term->curs.x] = false;
		}
		else if (term->esc_args[0] == 3) {
			int i;
			for (i = 0; i < term->cols; i++)
				term->tabs[i] = false;
		}
	}
}

void VtParserCSI::SetScollMargin()
{
	if (!CheckCompat(VT100)) return;

	if (term->esc_nargs <= 2) {
		int top, bot;
		CLAMP(term->esc_args[0], term->rows);
		CLAMP(term->esc_args[1], term->rows);
		top = def(term->esc_args[0], 1) - 1;
		bot = (term->esc_nargs <= 1
			|| term->esc_args[1] == 0 ?
			term->rows :
			def(term->esc_args[1], term->rows)) - 1;
		if (bot >= term->rows)
			bot = term->rows - 1;
		/* VTTEST Bug 9 - if region is less than 2 lines
		* don't change region.
		*/
		if (bot - top > 0) {
			term->marg_t = top;
			term->marg_b = bot;
			term->curs.x = 0;
			/*
			* I used to think the cursor should be
			* placed at the top of the newly marginned
			* area. Apparently not: VMS TPU falls over
			* if so.
			*
			* Well actually it should for
			* Origin mode - RDB
			*/
			term->curs.y = (term->dec_om ?
				term->marg_t : 0);
			seen_disp_event(term);
		}
	}
}

void VtParserCSI::SetGraphics()
{

}

void VtParserCSI::SaveCursor()
{

}

void VtParserCSI::RestorCursor()
{

}

void VtParserCSI::SetPageSize()
{

}

void VtParserCSI::ScrollUp()
{
	if (!CheckCompat(SCOANSI)) return;
}

void VtParserCSI::ScrollDown()
{
	if (!CheckCompat(SCOANSI)) return;
}

void VtParserCSI::SetLines()
{
	if (!CheckCompat(VT420)) return;
}

void VtParserCSI::SetColumns()
{
	if (!CheckCompat(VT340TEXT)) return;
}

void VtParserCSI::WriteSpace()
{
	if (!CheckCompat(ANSIMIN)) return;
}

void VtParserCSI::ReportCharacter()
{
	if (!CheckCompat(VT100)) return;
}

void VtParserCSI::CheckSelection()
{
	if (!CheckCompat(OTHER)) return;
}

void VtParserCSI::ShowCursor()
{
	if (!CheckCompat(SCOANSI)) return;
}

void VtParserCSI::SetCursorOn()
{
	if (!CheckCompat(SCOANSI)) return;
}

void VtParserCSI::SetBlink()
{
	if (!CheckCompat(SCOANSI)) return;
}

void VtParserCSI::BlinkIsReal()
{
	if (!CheckCompat(SCOANSI)) return;
}

void VtParserCSI::Foreground()
{
	if (!CheckCompat(SCOANSI)) return;
}

void VtParserCSI::Background()
{
	if (!CheckCompat(SCOANSI)) return;
}

void VtParserCSI::SetEraseChar()
{
	if (!CheckCompat(SCOANSI)) return;
}

void VtParserCSI::SetCompatLevel()
{

}

