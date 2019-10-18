#pragma once
#include "VtParserBase.h"

class VtParserOSC : public VtParserBase
{
public:
	VtParserOSC();
	~VtParserOSC();

public:
	virtual void Parse();

protected:
	void Palette();
	void ResetPalette();
	void WordSet();

	void Args();

	void Title();

	void OscString();
};

