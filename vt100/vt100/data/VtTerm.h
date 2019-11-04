#pragma once
#include "VtCursor.h"
#include "VtArgument.h"


class VtTerm : VtData
{
public:
	VtTerm();
	~VtTerm();

public:
	VtScreen* Screen();
	VtCursor* Cursor();
	VtArgument* Argument();
	VtLdisc* Ldisc();
	VtTermChars* Chars();
	VtCharset* Charset();

public:
	Termstate GetTermState();
	void SetTermState(Termstate state);

	bool IsVt52Mode();
	void SetVt52Mode(bool val);

	int GetEscQuery();
	void SetEscQuery(int val);

	int GetDispTop();
	void SetDispTop(int val);

	SelState GetSelState();

public:
	bool CrHasLf();
	void SetCrHasLf(bool val);

	bool IsScrollOnDisp();
	void SetScrollOnDisp(bool val);

public:
	int Rows();
	int Cols();

	bool HasAltScreen();

public:
	void SaveCursor();
	void RestoreCursor();

public:
	void ScrollUp(Margin margin, int lines);
	void ScrollDown(Margin margin, int lines);
	void ScrollDown_sb(Margin margin, int lines);

	void PowerOn(bool clear);
	void NoPaste();
	void Deselect();
	void SwapScreen(int which, bool& reset, bool keep_cur_pos);

public:
	void CheckSelection(Postion from, Postion to);

	int ToInsideRow(int row);
	int ToInsideCol(int col);

	bool IsVisibleRow(int row);
	int ToVisibleRow(int row);

protected:
	VtLdisc m_ldisc;

	//screen and alternate screen
	VtScreen m_screen;
	VtScreen m_alt_screen;

	VtTermChars m_chars;

	int m_witch;
	int m_alt_which;
	int m_alt_sblines;	/* # of lines on alternate screen that should be used for scrollback. */

	bool m_bEraseToSb;

	//size and selection
	Size m_size;

	SelState m_selstate;
	Rect m_selection;
	Postion m_selanchor;

	//scroll back chars and arguments
	VtTermChars m_scrollback;
	VtArgument m_args;
	VtCharset m_charset;

protected:	//temporary variable
	Termstate m_termstate;
	int m_unget;
	bool m_vt52_mode;
	int m_esc_query;
	int m_disptop;	/* distance scrolled back (0 or -ve) */

	term_char m_cur_chr;

protected: //setting
	bool m_crhaslf;
	bool m_scroll_on_disp;
};

