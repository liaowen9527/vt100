#pragma once
#include "VtParserBase.h"

class VtParserCSI : public VtParserBase
{
public:
	VtParserCSI();
	~VtParserCSI();

public:
	virtual void Parse();

protected:
	void Digit();
	void Semicolon();
	void Email();
	void Control();

protected:
	void MoveUp();
	void ModeDown();
	void MoveRight();
	void MoveLeft();

	void CursorDown();
	void CursorRight();

	void DownAndCR();
	void UpAndCR();

	void Horizontal();
	void Vertical();

	void Postion();

	void ReportXtermVersion();

	void EraseParts();
	void EraseLineOrParts();

	void InsertLines();
	void DeleteLines();

	void InsertChars();
	void DeleteChars();

	void TypeQuery();
	void CursorPosQuery();

	void ToggleModeH();
	void ToggleModeL();

	void MediaCopy();

	void ClearTabs();

	void SetScollMargin();

	void SetGraphics();

	void SaveCursor();
	void RestorCursor();

	void SetPageSize();

	void ScrollUp();
	void ScrollDown();

	void SetLines();
	void SetColumns();

	void WriteSpace();

	void ReportCharacter();

	void CheckSelection();

	void ShowCursor();

	void SetCursorOn();

	void SetBlink();
	void BlinkIsReal();

	void Foreground();
	void Background();

	void SetEraseChar();

	void SetCompatLevel();
};

