#pragma once
#include "VtCursor.h"

struct VtTerm
{
public:
	VtTerm();
	~VtTerm();

public:
	Size GetSize();
	VtCursor GetCursor();

	void SaveCursor();
	void RestoreCursor();

	/*
	* Scroll the screen. (`lines' is +ve for scrolling forward, -ve
	* for backward.) `sb' is true if the scrolling is permitted to
	* affect the scrollback buffer.
	*/
	void scroll(Margin margin, int lines, bool sb);
	void ScrollUp(Margin margin, int lines);
	void ScrollDown(Margin margin, int lines);

public:
	void CheckBoundary(int row, int col);

	int ToInsideRow(int row);
	int ToInsideCol(int col);

	bool IsVisibleRow(int row);
	int ToVisibleRow(int row);

protected:
	VtCursor m_cursor;
	Size m_size;
	Margin m_margin;
	Rect m_selection;
	Postion m_selanchor;

	VtTermBuffer m_buffer;

	Termstate termstate;
	SelState m_selstate;

	char esc_query;
	bool printing;
	bool vt52_mode;

	int esc_nargs;
	unsigned esc_args[ARGS_MAX];

	int app_keypad_keys;

	bool no_remote_charset;
	int cset_attr[2];	//charset attr

	bool cjk_ambig_wide;
	bool utf;
	bool osc_w;

	int osc_strlen;

	int marg_t, marg_b;		       /* scroll margins */
	int dec_om;			       /* DEC origin mode flag */
	int wrap, wrapnext;		       /* wrap flags */
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
	int utf;			       /* Are we in toggleable UTF-8 mode? */
	int utf_state;		       /* Is there a pending UTF-8 character */
	int utf_char;		       /* and what is it so far. */
	int utf_size;		       /* The size of the UTF character. */
	int printing, only_printing;       /* Are we doing ANSI printing? */
	int print_state;		       /* state of print-end-sequence scan */

	int rows, cols, savelines;
	int has_focus;
	int in_vbell;
	long vbell_end;
	int app_cursor_keys, app_keypad_keys, vt52_mode;
	int repeat_off, cr_lf_return;
	int seen_disp_event;
	int big_cursor;
};

