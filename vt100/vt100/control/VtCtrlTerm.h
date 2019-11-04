#pragma once
#include "VtCtrlNode.h"

//Some terminals support local printing :
class VtCtrlTerm : public VtCtrlNode
{
public:
	VtCtrlTerm();
	~VtCtrlTerm();

public:
	void ScrollUp();
	void ScrollDown();

	void SetMargins();
	void SetPageSize();

	void SetCompatLevel();

	void PowerOn(bool clear);
	void NoPaste();
	void Deselect();
	void SwapScreen(int which, bool reset, bool keep_cur_pos);

	void Bell();
	void MediaCopy();

	//keyboard
	void LockShift(int index);
	void EnterEsc();
	void KeyCR();
	void KeyLF();
	void KeyTab();

	void FormFeed();
	void LineTab();

public: // from putty
	void move(int row, int col, int marg_clip);
	void scroll(int topline, int botline, int lines, bool sb);
	void erase_lots(bool line_only, bool from_begin, bool to_end);

	void check_selection(Postion from, Postion to);
	void check_boundary(int row, int col);
	void check_trust_status(term_line *line);

	void clear_cc(term_line *line, int col);

	void insch(int width);
	void incpos(Postion& pos);

public:
	void seen_disp_event();
};