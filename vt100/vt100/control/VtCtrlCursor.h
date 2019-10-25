#pragma once
#include "VtCtrlNode.h"

class VtCtrlCursor : public VtCtrlNode
{
public:
	VtCtrlCursor();
	~VtCtrlCursor();

public:
	void Up();
	void Down();
	void Forward();	
	void Backward();

	void Backspace();

	void DownAndCR();
	void UpAndCR();

	void Horizontal();
	void Vertical();

	void SetPosition();

	void CSI_Z();
	void NextLine();

	void ShowOrHide();
	void SetCursorOn();

	void Save();
	void Restore();
};