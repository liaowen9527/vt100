#pragma once
#include "VtParserBase.h"

class VtParserTopLevel : public VtParserBase
{
public:
	VtParserTopLevel();
	~VtParserTopLevel();

public:
	virtual void Parse();

protected:
	void Display_W0();
	void Display_W1();
	void Display_W2();

	void After_Display();
};

