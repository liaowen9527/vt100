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
	if (c >= ' ' && c <= '/') 
	{
		term->esc_query = term->esc_query ? -1 : c;
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
		EnterOSC();
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

void VtParserESC::EnterCSI()
{
	term->termstate = SEEN_CSI;
	term->esc_nargs = 1;
	term->esc_args[0] = ARG_DEFAULT;
	term->esc_query = 0;
}

void VtParserESC::EnterOSC()
{
	/* Compatibility is nasty here, xterm, linux, decterm yuk! */
	if (!CheckCompat(OTHER)) return;
	
	term->termstate = SEEN_OSC;
	term->esc_args[0] = 0;
}

void VtParserESC::SaveCursor()
{
	if (!CheckCompat(VT100)) return;
	
	term->SaveCursor();
}

void VtParserESC::RestoreCursor()
{
	if (!CheckCompat(VT100)) return;

	term->RestoreCursor();
	
	seen_disp_event(term);
}

void VtParserESC::KeypadAppMode()
{
	if (!CheckCompat(VT100)) return;

	term->app_keypad_keys = true;
}

void VtParserESC::KeypadNumMode()
{
	if (!CheckCompat(VT100)) return;

	term->app_keypad_keys = false;
}

void VtParserESC::NextLine()
{
	if (!CheckCompat(VT100)) return;
	
	term->m_cursor.NextLine();
	
	seen_disp_event(term);
}

void VtParserESC::CrLf()
{
	if (!CheckCompat(VT100)) return;

	term->curs.x = 0;
	if (term->curs.y == term->marg_b)
		scroll(term, term->marg_t, term->marg_b, 1, true);
	else if (term->curs.y < term->rows - 1)
		term->curs.y++;
	term->wrapnext = false;
	seen_disp_event(term);
}

void VtParserESC::ReverseIndex()
{
	if (!CheckCompat(VT100)) return;

	if (term->curs.y == term->marg_t)
		scroll(term, term->marg_t, term->marg_b, -1, true);
	else if (term->curs.y > 0)
		term->curs.y--;
	term->wrapnext = false;
	seen_disp_event(term);
}

void VtParserESC::TypeQuery()
{
	if (!CheckCompat(VT100)) return;

	if (term->ldisc && term->id_string[0])
	{
		ldisc_send(term->ldisc, term->id_string,
			strlen(term->id_string), false);
	}
}

void VtParserESC::RestorePowerOn()
{
	if (!CheckCompat(VT100)) return;

	power_on(term, true);
	if (term->ldisc)   /* cause ldisc to notice changes */
		ldisc_echoedit_update(term->ldisc);
	if (term->reset_132) {
		if (!term->no_remote_resize)
			win_request_resize(term->win, 80, term->rows);
		term->reset_132 = false;
	}
	if (term->scroll_on_disp)
		term->disptop = 0;
	seen_disp_event(term);
}

void VtParserESC::SetOneTab()
{
	if (!CheckCompat(VT100)) return;

	term->tabs[term->curs.x] = true;
}

void VtParserESC::FillScreen()
{
	if (!CheckCompat(VT100)) return;

	termline *ldata;
	int i, j;
	pos scrtop, scrbot;

	for (i = 0; i < term->rows; i++) {
		ldata = scrlineptr(i);
		check_line_size(term, ldata);
		for (j = 0; j < term->cols; j++) {
			copy_termchar(ldata, j,
				&term->basic_erase_char);
			ldata->chars[j].chr = 'E';
		}
		ldata->lattr = LATTR_NORM;
	}
	if (term->scroll_on_disp)
		term->disptop = 0;
	seen_disp_event(term);
	scrtop.x = scrtop.y = 0;
	scrbot.x = 0;
	scrbot.y = term->rows;
	check_selection(term, scrtop, scrbot);
}

void VtParserESC::DoubleHeightLine(int lattr)
{
	if (!CheckCompat(VT100)) return;

	int nlattr;
	termline *ldata;

	ldata = scrlineptr(term->curs.y);
	check_line_size(term, ldata);
	check_trust_status(term, ldata);
	ldata->lattr = lattr;
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

