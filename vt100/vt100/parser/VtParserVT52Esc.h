#pragma once
#include "VtParserBase.h"

class VtParserVT52Esc : public VtParserBase
{
public:
	VtParserVT52Esc();
	~VtParserVT52Esc();

public:
	virtual void Parse();

protected:
	void Up();
	void Down();
	void Right();
	void Left();
	void Home();

	void Charset(int attr);

	void ReverseLine();

	void ResetEraseChar();
	void EraseEol();	//to end of line
	void EraseEos();	//to end of screen
	void EraseSol();	//to start of line
	void EraseSos();	//to start if screen
	void EraseLine();	//line

	void Into_VT52_Y1();
	void Into_VT52_FG();
	void Into_VT52_BG();

	void Identify();

	void KeypadApp(bool open);
	
	void ExitVT52();

	void HomeAndEraseEol();

	void ScrollUp();
	void ScrollDown();

	void CursorOn(bool open);

	void SetAttr(int attr);
	void RemoveAttr(int attr);
	void Bold(bool value);

	void Wrap(bool value);

};

