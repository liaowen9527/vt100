#pragma once
#include "VtCursor.h"
#include "VtArgument.h"


class VtTerm : VtData
{
public:
	VtTerm();
	~VtTerm();

public:
	VtCursor* Cursor();
	VtArgument* Argument();
	VtLdisc* Ldisc();
	VtTermChars* Chars();

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
	screen_tag m_screen;
	screen_tag m_alt_screen;

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

	Termstate termstate;
	
};

