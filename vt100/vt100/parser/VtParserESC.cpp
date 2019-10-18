/*
https://www2.ccs.neu.edu/research/gpc/VonaUtils/vona/terminal/vtansi.htm
http://ascii-table.com/ansi-escape-sequences-vt-100.php
*/

#include "VtParserESC.h"
#include "../define/VtType.h"

VtParserESC::VtParserESC()
{
	
}

VtParserESC::~VtParserESC()
{

}

void VtParserESC::Parse()
{
	if (ChangeQuery())
	{
		return;
	}

	term->termstate = TOPLEVEL;
	
	char ctrl = ANSI(c, term->esc_query);
	switch (ctrl)
	{
	case '[': /* enter CSI mode */
		EnterCSI();
		break;
	case ']': /* OSC: xterm escape sequences */
		Xterm();
		break;
	case '7':		/* DECSC: save cursor */
		SaveCursor();
		break;
	case '8':	 	/* DECRC: restore cursor */
		RestoreCursor();
		break;
	case '=':		/* DECKPAM: Keypad application mode */
		KeypadAppMode();
		break;
	case '>':		/* DECKPNM: Keypad numeric mode */
		KeypadNumMode();
		break;
	case 'D':	       /* IND: exactly equivalent to LF */
		NextLine();
		break;
	case 'E':	       /* NEL: exactly equivalent to CR-LF */
		CrLf();
		break;
	case 'M':	       /* RI: reverse index - backwards LF */
		ReverseIndex();
		break;
	case 'Z':	       /* DECID: terminal type query */
		TypeQuery();
		break;
	case 'c':	       /* RIS: restore power-on settings */
		RestorePowerOn();
		break;
	case 'H':	       /* HTS: set a tab */
		SetOneTab();
		break;
	case ANSI('8', '#'):	/* DECALN: fills screen with Es :-) */
		FillScreen();
		break;
	case ANSI('3', '#'):	/* DECDHL: 2*height, top */
		DoubleHeightLine();
		break;
	case ANSI('4', '#'):	/* DECDHL: 2*height, bottom */
		DoubleHeightLine(LATTR_TOP);	
		break;
	case ANSI('5', '#'):	/* DECSWL: normal */
		DoubleHeightLine(LATTR_NORM);
		break;
	case ANSI('6', '#'):	/* DECDWL: 2*width */
		DoubleHeightLine(LATTR_WIDE);
		break;
		/* GZD4: G0 designate 94-set */
	case ANSI('A', '('):	
		Charset(CSET_GBCHR, VT100);
		break;
	case ANSI('B', '('):
		Charset(CSET_ASCII, VT100);
		break;
	case ANSI('0', '('):
		Charset(CSET_LINEDRW, VT100);
		break;
		/* G1D4: G1-designate 94-set */
	case ANSI('U', '('):	
		Charset(CSET_SCOACS, OTHER);
		break;
	case ANSI('A', ')'):
		Charset(CSET_GBCHR, VT100);
		break;
	case ANSI('B', ')'):
		Charset(CSET_ASCII, VT100);
		break;
	case ANSI('0', ')'):
		Charset(CSET_LINEDRW, VT100);
		break;
	case ANSI('U', ')'):
		Charset(CSET_SCOACS, OTHER);
		break;
		/* DOCS: Designate other coding system */
	case ANSI('8', '%'):	/* Old Linux code */
	case ANSI('G', '%'):

		break;
	case ANSI('@', '%'):
		break;
	default:
		break;
	}
}

bool VtParserESC::ChangeQuery()
{
	if (c >= ' ' && c <= '/')
	{
		if (term->esc_query)
		{
			term->esc_query = -1;
		}
		else
		{
			term->esc_query = c;
		}

		return true;
	}

	return false;
}

void VtParserESC::SetChar(char ch)
{
	c = ch;
}

void VtParserESC::EnterCSI()
{
	term->termstate = SEEN_CSI;
	term->esc_nargs = 1;
	term->esc_args[0] = ARG_DEFAULT;
	term->esc_query = FALSE;
}

void VtParserESC::Xterm()
{
	/* Compatibility is nasty here, xterm, linux, decterm yuk! */
	if (!CheckCompat(OTHER)) return;
	
	term->termstate = SEEN_OSC;
	term->esc_args[0] = 0;
}

void VtParserESC::SaveCursor()
{
	if (!CheckCompat(VT100)) return;
	
	//save_cursor(term, TRUE);
}

void VtParserESC::RestoreCursor()
{
	if (!CheckCompat(VT100)) return;

// 	save_cursor(term, FALSE);
// 	seen_disp_event(term);
}

void VtParserESC::KeypadAppMode()
{
	if (!CheckCompat(VT100)) return;

	term->app_keypad_keys = 1;
}

void VtParserESC::KeypadNumMode()
{
	if (!CheckCompat(VT100)) return;

	term->app_keypad_keys = 0;
}

void VtParserESC::NextLine()
{
	if (!CheckCompat(VT100)) return;

	
}

void VtParserESC::CrLf()
{
	if (!CheckCompat(VT100)) return;
}

void VtParserESC::ReverseIndex()
{
	if (!CheckCompat(VT100)) return;
}

void VtParserESC::TypeQuery()
{
	if (!CheckCompat(VT100)) return;
}

void VtParserESC::RestorePowerOn()
{
	if (!CheckCompat(VT100)) return;
}

void VtParserESC::SetOneTab()
{
	if (!CheckCompat(VT100)) return;
}

void VtParserESC::FillScreen()
{
	if (!CheckCompat(VT100)) return;
}

void VtParserESC::DoubleHeightLine(int lattr)
{
	if (!CheckCompat(VT100)) return;
}

void VtParserESC::Charset(int attr, int vttype)
{
	if (!CheckCompat(vttype)) return;

	if (!term->no_remote_charset)
	{
		term->cset_attr[1] = attr;
	}
}

void VtParserESC::Utf(bool open)
{
	if (!CheckCompat(OTHER)) return;

	if (!term->no_remote_charset)
	{
		term->utf = open;
	}
}

