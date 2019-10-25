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
};