#include "VtCtrlSetup.h"
#include "../VtContext.h"

VtCtrlSetup::VtCtrlSetup()
{
	
}

VtCtrlSetup::~VtCtrlSetup()
{

}

void VtCtrlSetup::ToggleModeH()
{
	for (int i = 0; i < m_args->Count(); i++)
	{
		int mode = m_args->GetArg(i);
		ToggleMode(mode, term->esc_query, true);
	}
}

void VtCtrlSetup::ToggleModeL()
{
	for (int i = 0; i < m_args->Count(); i++)
	{
		int mode = m_args->GetArg(i);
		ToggleMode(mode, term->esc_query, false);
	}
}

void VtCtrlSetup::ToggleMode(int mode, int query, bool state)
{
	if (query == 1) 
	{
		ToggleMode_q0(mode, high);
	}
	else if (query == 0) 
	{
		ToggleMode_q1(mode, high);
	}
}

void VtCtrlSetup::ToggleMode_q0(int code, bool state)
{
	switch (mode) 
	{
	case 4:                      /* IRM: set insert mode */
		compatibility(VT102);
		term->insert = state;
		break;
	case 12:                     /* SRM: set echo mode */
		term->srm_echo = !state;
		break;
	case 20:                     /* LNM: Return sends ... */
		term->cr_lf_return = state;
		break;
	case 34:                     /* WYULCURM: Make cursor BIG */
		compatibility2(OTHER, VT220);
		term->big_cursor = !state;
	}
}

void VtCtrlSetup::ToggleMode_q1(int code, bool state)
{
	switch (mode)
	{
	case 1:		       /* DECCKM: application cursor keys */
		AppCursorKeys(state);
		break;
	case 2:		       /* DECANM: VT52 mode */
		Vt52Mode(state);
		break;
	case 3:		       /* DECCOLM: 80/132 columns */
		Columns(state);
		break;
	case 5:		       /* DECSCNM: reverse video */
		ReverseVideo(state);
		break;
	case 6:		       /* DECOM: DEC origin mode */
		OriginMode(state);
		break;
	case 7:		       /* DECAWM: auto wrap */
		AutoWrap(state);
		break;
	case 8:		       /* DECARM: auto key repeat */
		AutoKeyRepeat(state);
		break;
	case 25:		       /* DECTCEM: enable/disable cursor */
		 EnableCursor(state);
		break;
	case 47:		       /* alternate screen */
		AlternateScreen(term->no_alt_screen ? false : state, false, false);
		break;
	case 1000:		       /* xterm mouse 1 (normal) */
		XtermMouse1(state);
		break;
	case 1002:		       /* xterm mouse 2 (inc. button drags) */
		XtermMouse2(state);
		break;
	case 1006:		       /* xterm extended mouse */
		XtermExtendMouse(state);
		break;
	case 1015:		       /* urxvt extended mouse */
		UrxvtExtendMouse(state);
		break;
	case 1047:                   /* alternate screen */
		AlternateScreen(term->no_alt_screen ? false : state, true, true);
		break;
	case 1048:                   /* save/restore cursor */
		SaveCursor(state);
		break;
	case 1049:                   /* cursor & alternate screen */
		Mode1049(state);
		break;
	case 2004:		       /* xterm bracketed paste */
		XtermBracketedPaste(state);
		break;
	}
}

void VtCtrlSetup::AppCursorKeys(bool state)
{
	term->app_cursor_keys = state;
}

void VtCtrlSetup::Vt52Mode(bool state)
{
	m_term->vt52_mode = !state;
	if (m_term->vt52_mode)
	{
		m_term->blink_is_real = false;
		m_term->vt52_bold = false;
	}
	else 
	{
		m_term->blink_is_real = m_term->blinktext;
	}

	term_schedule_tblink(term);
}

void VtCtrlSetup::Columns(bool state)
{
	m_term->Deselect();
	
	if (!m_term->no_remote_resize)
	{
		win_request_resize(term->win, state ? 132 : 80, term->rows);
	}
		
	m_term->reset_132 = state;
	
	m_term->m_margin.top = 0;
	m_term->m_margin.bottom = m_term->Rows();
	m_term->m_alt_marg = m_term->m_margin;

	m_cursor->MoveTo(0, 0, false);
	m_ctrl->erase.EraseLots(false, true, true);
}

void VtCtrlSetup::ReverseVideo(bool state)
{
	/*
	* Toggle reverse video. If we receive an OFF within the
	* visual bell timeout period after an ON, we trigger an
	* effective visual bell, so that ESC[?5hESC[?5l will
	* always be an actually _visible_ visual bell.
	*/
	if (term->rvideo && !state) 
	{
		/* This is an OFF, so set up a vbell */
		term_schedule_vbell(term, true, term->rvbell_startpoint);
	}
	else if (!term->rvideo && state) {
		/* This is an ON, so we notice the time and save it. */
		term->rvbell_startpoint = GETTICKCOUNT();
	}
	term->rvideo = state;
	seen_disp_event(term);
}

void VtCtrlSetup::OriginMode(bool state)
{
	term->dec_om = state;
}

void VtCtrlSetup::AutoWrap(bool state)
{
	term->wrap = state;
}

void VtCtrlSetup::AutoKeyRepeat(bool state)
{
	term->repeat_off = !state;
}

void VtCtrlSetup::EnableCursor(bool state)
{
	//m_context->cursor.Enable(state);
	term->cursor_on = state;
	seen_disp_event(term);
}

void VtCtrlSetup::AlternateScreen(bool state, bool reset, bool keep_cur_pos)
{
	if (!CheckCompat(OTHER))
	{
		return;
	}

	m_term->Deselect();
	m_term->SwapScreen(term->no_alt_screen ? 0 : state, false, false);
	
	if (term->scroll_on_disp)
	{
		term->disptop = 0;
	}
}

void VtCtrlSetup::XtermMouse1(bool state)
{
	term->xterm_mouse = state ? 1 : 0;
	win_set_raw_mouse_mode(term->win, state);
}

void VtCtrlSetup::XtermMouse2(bool state)
{
	term->xterm_mouse = state ? 2 : 0;
	win_set_raw_mouse_mode(term->win, state);
}

void VtCtrlSetup::XtermExtendMouse(bool state)
{
	term->xterm_extended_mouse = state;
}

void VtCtrlSetup::UrxvtExtendMouse(bool state)
{
	term->urxvt_extended_mouse = state;
}

void VtCtrlSetup::SaveCursor(bool state)
{
	if (!term->no_alt_screen)
	{
		save_cursor(term, state);
	}
		
	if (!state) seen_disp_event(term);
}

void VtCtrlSetup::Mode1049(bool state)
{
	if (state && !term->no_alt_screen)
	{
		save_cursor(term, state);
	}

	if (!state) seen_disp_event(term);

	if (!CheckCompat(OTHER))
	{
		return;
	}
	
	m_term->Deselect();
	m_term->SwapScreen(term->no_alt_screen ? 0 : state, true, false);

	if (!state && !term->no_alt_screen)
	{
		save_cursor(term, state);
	}
		
	if (term->scroll_on_disp)
	{
		term->disptop = 0;
	}
}

void VtCtrlSetup::XtermBracketedPaste(bool state)
{
	term->bracketed_paste = state;
}

