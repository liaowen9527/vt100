/*
https://www2.ccs.neu.edu/research/gpc/VonaUtils/vona/terminal/vtansi.htm
http://ascii-table.com/ansi-escape-sequences-vt-100.php
*/

#include "VtParser.h"
#include "../define/VtType.h"

VtParser::VtParser()
{
	
}

VtParser::~VtParser()
{

}

void VtParser::Parse(const std::string& str)
{
	Parse(str.c_str(), str.length());
}

void VtParser::Parse(const char* str, int len)
{
	const char* str_old = str;
	int len_old = len;
	while (len)
	{
		InputChar(*str);

		str = str + 1;
		--len;
	}

	m_context.buffer.Reset();
}

void VtParser::InputChar(char ch)
{
	if (m_bPrinting)
	{
		DoPainting();
	}
	if (m_term.termstate == TOPLEVEL)
	{
		m_vtTrans.Translate(ch);
		switch (t) 
		{
		case UCSINCOMPLETE:
			return;       /* didn't complete a multibyte char */
		case UCSTRUNCATED:
			unget = c;
			/* fall through */
		case UCSINVALID:
			c = UCSERR;
			break;
		default:
			c = t;
			break;
		}
	}
	DoSpecialCase();

	if (IsC0Control(ch))
	{
		C0Control(ch);
	}
	else
	{
		NormalControl(ch);
	}
}

void VtParser::DoPainting()
{

}

void VtParser::DoSpecialCase(char& c)
{
	/*
		 * How about C1 controls?
		 * Explicitly ignore SCI (0x9a), which we don't translate to DECID.
		 */
	if ((c & -32) == 0x80 && m_term.termstate < DO_CTRLS && !m_term.vt52_mode && vt_type::IsCompat(CL_VT220))
	{
		if (c == 0x9a)
		{
			c = 0;
		}	
		else 
		{
			m_term.termstate = SEEN_ESC;
			m_term.esc_query = FALSE;
			c = '@' + (c & 0x1F);
		}
	}
}

void VtParser::GLControl(char& c)
{
	//if (term->curs.x && !term->wrapnext)
	//{
	//	term->curs.x--;
	//}
	//	
	//term->wrapnext = FALSE;
	///* destructive backspace might be disabled */
	//if (!term->no_dbackspace) 
	//{
	//	check_boundary(term, term->curs.x, term->curs.y);
	//	check_boundary(term, term->curs.x + 1, term->curs.y);
	//	copy_termchar(scrlineptr(term->curs.y), term->curs.x, &term->erase_char);
	//}
}

void VtParser::C0Control(char& c)
{
	if ((c & ~0x1F) == 0 && term->termstate < DO_CTRLS) 
	{
		switch (c) 
		{
		case '\005':             /* ENQ: terminal type query */
								 /*
								 * Strictly speaking this is VT100 but a VT100 defaults to
								 * no response. Other terminals respond at their option.
								 *
								 * Don't put a CR in the default string as this tends to
								 * upset some weird software.
								 */
			compatibility(ANSIMIN);
			if (term->ldisc) {
				strbuf *buf = term_input_data_from_charset(
					term, DEFAULT_CODEPAGE,
					term->answerback, term->answerbacklen);
				ldisc_send(term->ldisc, buf->s, buf->len, false);
				strbuf_free(buf);
			}
			break;
		case '\007':            /* BEL: Bell */
		{
			struct beeptime *newbeep;
			unsigned long ticks;

			ticks = GETTICKCOUNT();

			if (!term->beep_overloaded) 
			{
				newbeep = snew(struct beeptime);
				newbeep->ticks = ticks;
				newbeep->next = NULL;
				if (!term->beephead)
					term->beephead = newbeep;
				else
					term->beeptail->next = newbeep;
				term->beeptail = newbeep;
				term->nbeeps++;
			}

			/*
			* Throw out any beeps that happened more than
			* t seconds ago.
			*/
			while (term->beephead &&
				term->beephead->ticks < ticks - term->bellovl_t) {
				struct beeptime *tmp = term->beephead;
				term->beephead = tmp->next;
				sfree(tmp);
				if (!term->beephead)
					term->beeptail = NULL;
				term->nbeeps--;
			}

			if (term->bellovl && term->beep_overloaded &&
				ticks - term->lastbeep >= (unsigned)term->bellovl_s) {
				/*
				* If we're currently overloaded and the
				* last beep was more than s seconds ago,
				* leave overload mode.
				*/
				term->beep_overloaded = false;
			}
			else if (term->bellovl && !term->beep_overloaded &&
				term->nbeeps >= term->bellovl_n) {
				/*
				* Now, if we have n or more beeps
				* remaining in the queue, go into overload
				* mode.
				*/
				term->beep_overloaded = true;
			}
			term->lastbeep = ticks;

			/*
			* Perform an actual beep if we're not overloaded.
			*/
			if (!term->bellovl || !term->beep_overloaded) {
				win_bell(term->win, term->beep);

				if (term->beep == BELL_VISUAL) {
					term_schedule_vbell(term, false, 0);
				}
			}
			seen_disp_event(term);
		}
		break;
		case '\b':              /* BS: Back space */
			if (term->curs.x == 0 && (term->curs.y == 0 || !term->wrap))
				/* do nothing */;
			else if (term->curs.x == 0 && term->curs.y > 0)
				term->curs.x = term->cols - 1, term->curs.y--;
			else if (term->wrapnext)
				term->wrapnext = false;
			else
				term->curs.x--;
			seen_disp_event(term);
			break;
		case '\016':            /* LS1: Locking-shift one */
			compatibility(VT100);
			term->cset = 1;
			break;
		case '\017':            /* LS0: Locking-shift zero */
			compatibility(VT100);
			term->cset = 0;
			break;
		case '\033':            /* ESC: Escape */
			if (term->vt52_mode)
				term->termstate = VT52_ESC;
			else {
				compatibility(ANSIMIN);
				term->termstate = SEEN_ESC;
				term->esc_query = 0;
			}
			break;
		case '\015':            /* CR: Carriage return */
			term->curs.x = 0;
			term->wrapnext = false;
			seen_disp_event(term);

			if (term->crhaslf) {
				if (term->curs.y == term->marg_b)
					scroll(term, term->marg_t, term->marg_b, 1, true);
				else if (term->curs.y < term->rows - 1)
					term->curs.y++;
			}
			if (term->logctx)
				logtraffic(term->logctx, (unsigned char)c, LGTYP_ASCII);
			break;
		case '\014':            /* FF: Form feed */
			if (has_compat(SCOANSI)) {
				move(term, 0, 0, 0);
				erase_lots(term, false, false, true);
				if (term->scroll_on_disp)
					term->disptop = 0;
				term->wrapnext = false;
				seen_disp_event(term);
				break;
			}
		case '\013':            /* VT: Line tabulation */
			compatibility(VT100);
		case '\012':            /* LF: Line feed */
			if (term->curs.y == term->marg_b)
				scroll(term, term->marg_t, term->marg_b, 1, true);
			else if (term->curs.y < term->rows - 1)
				term->curs.y++;
			if (term->lfhascr)
				term->curs.x = 0;
			term->wrapnext = false;
			seen_disp_event(term);
			if (term->logctx)
				logtraffic(term->logctx, (unsigned char)c, LGTYP_ASCII);
			break;
		case '\t':              /* HT: Character tabulation */
		{
			pos old_curs = term->curs;
			termline *ldata = scrlineptr(term->curs.y);

			do {
				term->curs.x++;
			} while (term->curs.x < term->cols - 1 &&
				!term->tabs[term->curs.x]);

			if ((ldata->lattr & LATTR_MODE) != LATTR_NORM) {
				if (term->curs.x >= term->cols / 2)
					term->curs.x = term->cols / 2 - 1;
			}
			else {
				if (term->curs.x >= term->cols)
					term->curs.x = term->cols - 1;
			}

			check_selection(term, old_curs, term->curs);
		}
		seen_disp_event(term);
		break;
		}
	}
}

void VtParser::NormalControl(char& c)
{
	if (IsC0Control(c))
	{
		C0Control(c);
	}

	switch (m_term.termstate)
	{
	case TOPLEVEL:
		m_vtTopLevel.Parse(c);
		break;
	case SEEN_ESC:
		m_vtEsc.Parse(c);
		break;
	case SEEN_CSI:
		m_vtCsi.Parse(c);
		break;
	case SEEN_OSC:
		m_vtOsc.Parse(c);
		break;
	case SEEN_OSC_W:
		m_vtOscW.Parse(c);
		break;
	case SEEN_OSC_P:
		m_vtOscP.Parse(c);
		break;
	case OSC_STRING:
		m_vtOscStr.Parse(c);
		break;
	case OSC_MAYBE_ST:
		m_vtMaybeST.Parse(c);
		break;
	case VT52_ESC:
		m_vt52Esc.Parse(c);
		break;
	case VT52_Y1:
		m_vt52Y1.Parse(c);
		break;
	case VT52_Y2:
		m_vt52Y1.Parse(c);
		break;
	case VT52_FG:
		m_vt52FG.Parse(c);
		break;
	case VT52_BG:
		m_vt52BG.Parse(c);
		break;
	default:
		break;
	}
}

bool VtParser::IsGLControl(char c)
{
	return c == '\177' && m_term.termstate < DO_CTRLS && vt_type::IsCompat(CL_OTHER);
}

bool VtParser::IsC0Control(char c)
{
	return (c & ~0x1F) == 0 && m_term.termstate < DO_CTRLS;
}