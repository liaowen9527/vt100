/*
https://www2.ccs.neu.edu/research/gpc/VonaUtils/vona/terminal/vtansi.htm
http://ascii-table.com/ansi-escape-sequences-vt-100.php
*/

#include "VtParserESC.h"


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
		m_cursor->Up();
		break;
	case 'e':		/* VPR: move down N lines */
		if (CheckCompat(ANSI)) m_cursor->Down();
		break;
		/* FALLTHROUGH */
	case 'B':		/* CUD: Cursor down */
		m_cursor->Down();
		break;
	case 'b':        /* REP: repeat previous grap */
		RepeateGrap();
		break;
	case ANSI('c', '>'):	/* DA: report xterm version */
		ReportXtermVersion();
		break;
	case 'a':		/* HPR: move right N cols */
		if (CheckCompat(ANSI)) m_cursor->Forward();
		break;
		/* FALLTHROUGH */
	case 'C':		/* CUF: Cursor right */
		m_cursor->Forward();
		break;
	case 'D':       /* CUB: move left N cols */
		m_cursor->Backward();
		break;
	case 'E':       /* CNL: move down N lines and CR */
		if (CheckCompat(ANSI)) m_cursor->DownAndCR();
		break;
	case 'F':       /* CPL: move up N lines and CR */
		if (CheckCompat(ANSI)) m_cursor->UpAndCR();
		break;
	case 'G':	      /* CHA */
	case '`':       /* HPA: set horizontal posn */
		if (CheckCompat(ANSI)) m_cursor->Horizontal();
		break;
	case 'd':       /* VPA: set vertical posn */
		if (CheckCompat(ANSI)) m_cursor->Vertical();
		break;
	case 'H':	     /* CUP */
	case 'f':      /* HVP: set horz and vert posns at once */
		m_cursor->SetPosition();
		break;
	case 'J':       /* ED: erase screen or parts of it */
		m_erase->Parts();
		break;
	case 'K':       /* EL: erase line or parts of it */
		m_erase->LineOrParts();
		break;
	case 'L':       /* IL: insert lines */
		if (CheckCompat(VT102)) m_buffer->InsertLines();
		break;
	case 'M':       /* DL: delete lines */
		if (CheckCompat(VT102)) m_buffer->DeleteLines();
		break;
	case '@':       /* ICH: insert chars */
  /* XXX VTTEST says this is vt220, vt510 manual says vt102 */
		if (CheckCompat(VT102)) m_buffer->InsertChars();
		break;
	case 'P':       /* DCH: delete chars */
		if (CheckCompat(VT102)) m_buffer->DeleteChars();
		break;
	case 'c':       /* DA: terminal type query */
		if (CheckCompat(VT100)) m_ldisc->QueryType();
		break;
	case 'n':       /* DSR: cursor position query */
		m_ldisc->QueryCursorPos();
		break;
	case 'h':       /* SM: toggle modes to high */
	case ANSI_QUE('h'):
		if (CheckCompat(VT100)) m_setup->ToggleModeH();
		break;
	case 'i':		/* MC: Media copy */
	case ANSI_QUE('i'):
		m_ctrlTerm->MediaCopy();
		break;
	case 'l':       /* RM: toggle modes to low */
	case ANSI_QUE('l'):
		if (CheckCompat(VT100)) m_setup->ToggleModeL();
		break;
	case 'g':       /* TBC: clear tabs */
		if (CheckCompat(VT100)) m_buffer->ClearTabs();
		break;
	case 'r':       /* DECSTBM: set scroll margins */
		if (CheckCompat(VT100)) m_ctrlTerm->SetMargins();
		break;
	case 'm':       /* SGR: set graphics rendition */
		m_render->Render();
		break;
	case 's':       /* save cursor */
		m_cursor->Save();
		break;
	case 'u':       /* restore cursor */
		m_cursor->Restore();
		break;
	case 't': /* DECSLPP: set page size - ie window height */
		m_ctrlTerm->SetPageSize();
		break;
	case 'S':		/* SU: Scroll up */
		m_ctrlTerm->ScrollUp();
		break;
	case 'T':		/* SD: Scroll down */
		m_ctrlTerm->ScrollDown();
		break;
	case ANSI('|', '*'): /* DECSNLS */
		m_win->SetLines();
		break;
	case ANSI('|', '$'): /* DECSCPP */
		m_win->SetColumns();
		break;
	case 'X':     /* ECH: write N spaces w/o moving cursor */
		m_buffer->WriteSpaces();
		break;
	case 'x':       /* DECREQTPARM: report terminal characteristics */
		m_ldisc->ReportCharacter();
		break;
	case 'Z':		/* CBT */
		m_cursor->CSI_Z();
		break;
	case ANSI('c', '='):      /* Hide or Show Cursor */
		m_cursor->ShowOrHide();
		break;
	case ANSI('C', '='):
		m_cursor->SetCursorOn();
		break;
	case ANSI('D', '='):
		m_render->SetBlink();
		break;
	case ANSI('E', '='):
		m_render->SetBlinkIsReal();
		break;
	case ANSI('F', '='):      /* set normal foreground */
		m_render->NormalForeground();
		break;
	case ANSI('G', '='):      /* set normal background */
		m_render->NomalBackground();
		break;
	case ANSI('L', '='):
		m_erase->UseBgcolorErase();
		break;
	case ANSI('p', '"'): /* DECSCL: set compat level */
		m_ctrlTerm->SetCompatLevel();
		break;
	}
}
