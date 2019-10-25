#include "VtCtrlTerm.h"
#include "../VtContext.h"

VtCtrlTerm::VtCtrlTerm()
{
	
}

VtCtrlTerm::~VtCtrlTerm()
{

}

void VtCtrlTerm::ScrollUp()
{
	if (!CheckCompat(SCOANSI))
	{
		return;
	}

	int lines = m_args->GetArg(0, m_term->Rows(), 1);
	m_term->ScrollUp(m_term->m_margin, lines);

	seen_disp_event(term);
}

void VtCtrlTerm::ScrollDown()
{
	if (!CheckCompat(SCOANSI))
	{
		return;
	}

	int lines = m_args->GetArg(0, m_term->Rows(), 1);
	m_term->ScrollDown_sb(m_term->m_margin, lines);

	seen_disp_event(term);
}

void VtCtrlTerm::SetMargins()
{
	int nArgs = m_args->Count();
	if (m_args->Count() > 2)
	{
		return;
	}

	int rows = m_term->Rows();

	int top = m_args->GetArg(0, rows, 1) - 1;
	int bot = rows;

	if (nArgs > 1 && m_args->GetArg(1) != 0)
	{
		bot = m_args->GetArg(1, m_term->Rows(), m_term->Rows()) - 1;
	}
	bot = std::min<int>(bot, rows - 1);

	/* VTTEST Bug 9 - if region is less than 2 lines
	* don't change region.
	*/
	if (bot - top > 0) 
	{
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
		term->curs.y = (term->dec_om ? term->marg_t : 0);
		seen_disp_event(term);
	}
}

void VtCtrlTerm::SetPageSize()
{
	/*
	* VT340/VT420 sequence DECSLPP, DEC only allows values
	*  24/25/36/48/72/144 other emulators (eg dtterm) use
	* illegal values (eg first arg 1..9) for window changing
	* and reports.
	*/
	int nArgs = m_args->Count();
	unsigned int arg_0 = m_args->GetArg(0);

	if (term->esc_nargs <= 1 && (arg_0 < 1 || arg_0 >= 24))
	{
		if (!CheckCompat(VT340TEXT))
		{
			return;
		}
		
		if (!term->no_remote_resize)
		{
			win_request_resize(term->win, term->cols, def(term->esc_args[0], 24));
		}
		
		Deselect();
	}
	else if (term->esc_nargs >= 1 && arg_0 >= 1 && arg_0 < 24)
	{
		if (!CheckCompat(OTHER))
		{
			return;
		}

		switch (arg_0)
		{
			int x, y, len;
			char buf[80];
			const char *p;
		case 1:
			m_ctrl->win
			win_set_minimised(term->win, false);
			break;
		case 2:
			win_set_minimised(term->win, true);
			break;
		case 3:
			if (term->esc_nargs >= 3) 
			{
				if (!term->no_remote_resize)
				{
					win_move(term->win, def(term->esc_args[1], 0), def(term->esc_args[2], 0));
				}
			}
			break;
		case 4:
			/* We should resize the window to a given
			 * size in pixels here, but currently our
			 * resizing code isn't healthy enough to
			 * manage it. */
			break;
		case 5:
			/* move to top */
			win_set_zorder(term->win, true);
			break;
		case 6:
			/* move to bottom */
			win_set_zorder(term->win, false);
			break;
		case 7:
			win_refresh(term->win);
			break;
		case 8:
			if (term->esc_nargs >= 3) 
			{
				if (!term->no_remote_resize)
				{
					win_request_resize(term->win, def(term->esc_args[2], term->conf_width), def(term->esc_args[1], term->conf_height));
				}
			}
			break;
		case 9:
			if (term->esc_nargs >= 2)
			{
				win_set_maximised(term->win, term->esc_args[1] ? true : false);
			}
			break;
		case 11:
			if (term->ldisc)
			{
				ldisc_send(term->ldisc, win_is_minimised(term->win) ? "\033[2t" : "\033[1t", 4, false);
			}
			break;
		case 13:
			if (term->ldisc) 
			{
				win_get_pos(term->win, &x, &y);
				len = sprintf(buf, "\033[3;%u;%ut", (unsigned)x, (unsigned)y);
				ldisc_send(term->ldisc, buf, len, false);
			}
			break;
		case 14:
			if (term->ldisc) 
			{
				win_get_pixels(term->win, &x, &y);
				len = sprintf(buf, "\033[4;%d;%dt", y, x);
				ldisc_send(term->ldisc, buf, len, false);
			}
			break;
		case 18:
			if (term->ldisc) 
			{
				len = sprintf(buf, "\033[8;%d;%dt", term->rows, term->cols);
				ldisc_send(term->ldisc, buf, len, false);
			}
			break;
		case 19:
			/*
			 * Hmmm. Strictly speaking we
			 * should return `the size of the
			 * screen in characters', but
			 * that's not easy: (a) window
			 * furniture being what it is it's
			 * hard to compute, and (b) in
			 * resize-font mode maximising the
			 * window wouldn't change the
			 * number of characters. *shrug*. I
			 * think we'll ignore it for the
			 * moment and see if anyone
			 * complains, and then ask them
			 * what they would like it to do.
			 */
			break;
		case 20:
			if (term->ldisc && term->remote_qtitle_action != TITLE_NONE) 
			{
				if (term->remote_qtitle_action == TITLE_REAL)
				{
					p = win_get_title(term->win, true);
				}
				else
				{
					p = EMPTY_WINDOW_TITLE;
				}
				len = strlen(p);
				ldisc_send(term->ldisc, "\033]L", 3, false);
				if (len > 0)
				{
					ldisc_send(term->ldisc, p, len, false);
				}

				ldisc_send(term->ldisc, "\033\\", 2, false);
			}
			break;
		case 21:
			if (term->ldisc && term->remote_qtitle_action != TITLE_NONE) 
			{
				if (term->remote_qtitle_action == TITLE_REAL)
				{
					p = win_get_title(term->win, false);
				}
				else
				{
					p = EMPTY_WINDOW_TITLE;
				}
				len = strlen(p);
				ldisc_send(term->ldisc, "\033]l", 3, false);
				if (len > 0)
				{
					ldisc_send(term->ldisc, p, len, false);
				}
				ldisc_send(term->ldisc, "\033\\", 2, false);
			}
			break;
		}
	}
}

void VtCtrlTerm::SetCompatLevel()
{
	/*
	* Allow the host to make this emulator a
	* 'perfect' VT102. This first appeared in
	* the VT220, but we do need to get back to
	* PuTTY mode so I won't check it.
	*
	* The arg in 40..42,50 are a PuTTY extension.
	* The 2nd arg, 8bit vs 7bit is not checked.
	*
	* Setting VT102 mode should also change
	* the Fkeys to generate PF* codes as a
	* real VT102 has no Fkeys. The VT220 does
	* this, F11..F13 become ESC,BS,LF other
	* Fkeys send nothing.
	*
	* Note ESC c will NOT change this!
	*/

	int arg = m_args->GetArg(0);
	switch (arg)
	{
	case ARG_DEFAULT:
		term->compatibility_level = TM_PUTTY;
		break;
	case 40:
		term->compatibility_level &= TM_VTXXX;
		break;
	case 41:
		term->compatibility_level = TM_PUTTY;
		break;
	case 42:
		term->compatibility_level = TM_SCOANSI;
		break;
	case 50:
		break;
	case 61:
		term->compatibility_level &= ~TM_VTXXX;
		term->compatibility_level |= TM_VT102;
		break;
	case 62:
		term->compatibility_level &= ~TM_VTXXX;
		term->compatibility_level |= TM_VT220;
		break;
	default:
		if (60 <arg && arg < 70)
		{
			term->compatibility_level |= TM_VTXXX;
		}
		break;
	}

	/* Change the response to CSI c */
	if (term->esc_args[0] == 50) 
	{
		int i;
		char lbuf[64];
		strcpy(term->id_string, "\033[?");
		for (i = 1; i < term->esc_nargs; i++) 
		{
			if (i != 1)
			{
				strcat(term->id_string, ";");
			}
				
			sprintf(lbuf, "%u", term->esc_args[i]);
			strcat(term->id_string, lbuf);
		}
		strcat(term->id_string, "c");
	}

}

void VtCtrlTerm::PowerOn(bool clear)
{

}

void VtCtrlTerm::NoPaste()
{

}

void VtCtrlTerm::Deselect()
{
	m_term->Deselect();
}

void VtCtrlTerm::SwapScreen(int which, bool reset, bool keep_cur_pos)
{
	m_term->SwapScreen(which, reset, keep_cur_pos);

	if (reset)
	{
		/*
		 * Yes, this _is_ supposed to honour background-colour-erase.
		 */
		m_ctrl->erase.EraseLots(false, true, true);
	}
}

void VtCtrlTerm::Bell()
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
	if (!term->bellovl || !term->beep_overloaded) 
	{
		win_bell(term->win, term->beep);

		if (term->beep == BELL_VISUAL) {
			term_schedule_vbell(term, false, 0);
		}
	}
	seen_disp_event(term);
}

void VtCtrlTerm::MediaCopy()
{
	if (!CheckCompat(VT100))
	{
		return;
	}
	
	char *printer;
	if (term->esc_nargs != 1)
	{
		return;
	}

	if (term->esc_args[0] == 5 &&
		(printer = conf_get_str(term->conf, CONF_printer))[0]) 
	{
		term->printing = true;
		term->only_printing = !term->esc_query;
		term->print_state = 0;
		term_print_setup(term, printer);
	}
	else if (term->esc_args[0] == 4 && term->printing) 
	{
		term_print_finish(term);
	}
}

void VtCtrlTerm::LockShift(int index)
{
	if (CheckCompat(VT100))
	{
		return;
	}
	
	term->cset = index;
}

void VtCtrlTerm::EnterEsc()
{
	if (term->vt52_mode)
	{
		term->termstate = VT52_ESC;
	}
	else 
	{
		if (CheckCompat(ANSIMIN))
		{
			return;
		}

		term->termstate = SEEN_ESC;
		term->esc_query = 0;
	}
}

void VtCtrlTerm::KeyCR()
{
	term->curs.x = 0;
	term->wrapnext = false;
	seen_disp_event(term);

	if (term->crhaslf) 
	{
		if (term->curs.y == term->marg_b)
		{
			scroll(term, term->marg_t, term->marg_b, 1, true);
		}
		else if (term->curs.y < term->rows - 1)
		{
			term->curs.y++;
		}
	}

	if (term->logctx)
		logtraffic(term->logctx, (unsigned char)c, LGTYP_ASCII);
}

void VtCtrlTerm::KeyLF()
{
	if (term->curs.y == term->marg_b)
	{
		scroll(term, term->marg_t, term->marg_b, 1, true);
	}
	else if (term->curs.y < term->rows - 1)
	{
		term->curs.y++;
	}
		
	if (term->lfhascr)
	{
		term->curs.x = 0;
	}

	term->wrapnext = false;
	seen_disp_event(term);

	if (term->logctx)
		logtraffic(term->logctx, (unsigned char)c, LGTYP_ASCII);
}

void VtCtrlTerm::KeyTab()
{
	pos old_curs = term->curs;
	termline *ldata = scrlineptr(term->curs.y);

	do 
	{
		term->curs.x++;
	} while (term->curs.x < term->cols - 1 && !term->tabs[term->curs.x]);

	if ((ldata->lattr & LATTR_MODE) != LATTR_NORM) 
	{
		if (term->curs.x >= term->cols / 2)
		{
			term->curs.x = term->cols / 2 - 1;
		}
	}
	else 
	{
		if (term->curs.x >= term->cols)
		{
			term->curs.x = term->cols - 1;
		}
	}

	check_selection(term, old_curs, term->curs);
	seen_disp_event(term);
}

void VtCtrlTerm::FormFeed()
{
	if (HasCompat(SCOANSI))
	{
		move(term, 0, 0, 0);
		erase_lots(term, false, false, true);
		if (term->scroll_on_disp)
			term->disptop = 0;
		term->wrapnext = false;
		seen_disp_event(term);
		return;
	}

	LineTab();
}

void VtCtrlTerm::LineTab()
{
	if (!CheckCompat(VT100))
	{
		return;
	}

	KeyLF();
}

