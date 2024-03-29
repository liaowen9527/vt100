#pragma once
#include <deque>
#include "VtData.h"
#include "term_chars/term_lines.h"

class VtTermChars : public VtData
{
public:
	VtTermChars();
	~VtTermChars();

public:
	void Resize(int rols, int cols);

public:
	int GetLines();
	int GetSbLines();

	term_line* GetLine(int row);
	term_line* GetLine_by_term(int row);

	term_lines* Screen();
	term_lines* AltScreen();
	term_lines* Scrollback();

	int GetCharWidth(unsigned long c);

public:
	bool FindNextTab(int from_col, int& stop_col);
	bool HasTab(int col);

public:
	void SetAltSblines(int count);
	int GetAltSblines();

public:
	void SwapLines();
	void CheckBoundary(int row, int col);

protected:
	term_lines m_screen;
	term_lines m_alt_screen;
	term_lines m_scrollback;

	int m_alt_sblines;

	std::vector<bool> m_tabs;
};