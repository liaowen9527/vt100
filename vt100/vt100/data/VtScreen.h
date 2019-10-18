#pragma once
#include "VtCursor.h"

class VtScreen
{
public:
	VtScreen();
	~VtScreen();

public:
	

protected:
	
	
	int marg_t, marg_b;		       /* scroll margins */
	int dec_om;			       /* DEC origin mode flag */
	bool m_wrap;					/* wrap flags */
	bool m_wrapnext;				/* wrap flags */

	int insert;			       /* insert-mode flag */
	int cset;			       /* 0 or 1: which char set */

	int save_cset, save_csattr;	       /* saved with cursor position */
	int save_utf, save_wnext;	       /* saved with cursor position */

	int rvideo;			       /* global reverse video flag */
	unsigned long rvbell_startpoint;   /* for ESC[?5hESC[?5l vbell */

	int cursor_on;		       /* cursor enabled flag */
	int reset_132;		       /* Flag ESC c resets to 80 cols */
	int use_bce;		       /* Use Background coloured erase */
	int cblinker;		       /* When blinking is the cursor on ? */
	int tblinker;		       /* When the blinking text is on */
	int blink_is_real;		       /* Actually blink blinking text */
	int term_echoing;		       /* Does terminal want local echo? */
	int term_editing;		       /* Does terminal want local edit? */
	int sco_acs, save_sco_acs;	       /* CSI 10,11,12m -> OEM charset */
	int vt52_bold;		       /* Force bold on non-bold colours */
	int utf;			       /* Are we in toggle able UTF-8 mode? */
	int utf_state;		       /* Is there a pending UTF-8 character */
	int utf_char;		       /* and what is it so far. */
	int utf_size;		       /* The size of the UTF character. */
	int printing, only_printing;       /* Are we doing ANSI printing? */
	int print_state;		       /* state of print-end-sequence scan */
};

