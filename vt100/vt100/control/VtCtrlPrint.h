#pragma once
#include "../VtCtxNode.h"

//Some terminals support local printing :
class VtCtrlPrint : public VtCtxNode
{
public:
	VtCtrlPrint();
	~VtCtrlPrint();

	void Init();

public:
	/* Esc[i	Print Screen	Print the current screen */
	void Screen();

	/* Esc[1i	Print Line	Print the current line */
	void Line();

	/* Esc[4i	Stop Print Log	Disable log */
	void DisableLog();

	/* Esc[5i	Start Print Log	Start log;all received text is echoed to a printer */
	void PrintLog();
};