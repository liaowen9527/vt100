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

}

void VtParserCSI::CursorDown()
{

}

void VtParserCSI::CursorRight()
{

}

void VtParserCSI::DownAndCR()
{
	if (!CheckCompat(ANSI)) return;

}

void VtParserCSI::UpAndCR()
{
	if (!CheckCompat(ANSI)) return;
}

void VtParserCSI::Horizontal()
{
	if (!CheckCompat(ANSI)) return;
}

void VtParserCSI::Vertical()
{
	if (!CheckCompat(ANSI)) return;
}

void VtParserCSI::Postion()
{

}

void VtParserCSI::ReportXtermVersion()
{
	if (!CheckCompat(OTHER)) return;
}

void VtParserCSI::EraseParts()
{

}

void VtParserCSI::EraseLineOrParts()
{

}

void VtParserCSI::InsertLines()
{
	if (!CheckCompat(VT102)) return;
}

void VtParserCSI::DeleteLines()
{
	if (!CheckCompat(VT102)) return;
}

void VtParserCSI::InsertChars()
{
	if (!CheckCompat(VT102)) return;
}

void VtParserCSI::DeleteChars()
{
	if (!CheckCompat(VT102)) return;
}

void VtParserCSI::TypeQuery()
{
	if (!CheckCompat(VT100)) return;
}

void VtParserCSI::CursorPosQuery()
{

}

void VtParserCSI::ToggleModeH()
{
	if (!CheckCompat(VT100)) return;
}

void VtParserCSI::ToggleModeL()
{
	if (!CheckCompat(VT100)) return;
}

void VtParserCSI::MediaCopy()
{
	if (!CheckCompat(VT100)) return;
}

void VtParserCSI::ClearTabs()
{
	if (!CheckCompat(VT100)) return;
}

void VtParserCSI::SetScollMargin()
{
	if (!CheckCompat(VT100)) return;
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

