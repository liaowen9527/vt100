#pragma once
#include "VtParserBase.h"

class VtParserESC : public VtParserBase
{
public:
	VtParserESC();
	~VtParserESC();

public:
	virtual void Parse();

protected:
	void EnterCSI();
	void EnterOSC();

	void SaveCursor();
	void RestoreCursor();

	void KeypadAppMode();
	void KeypadNumMode();

	void NextLine();
	void CrLf();

	void ReverseIndex();
	void TypeQuery();
	void RestorePowerOn();
	void SetOneTab();

	void FillScreen();
	void DoubleHeightLine(int lattr);

	void Charset(int attr, int vttype);
	void Utf(bool open);
};

