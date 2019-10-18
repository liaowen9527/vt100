#pragma once
#include "VtParserBase.h"

class VtParserOSC_W : public VtParserBase
{
public:
	VtParserOSC_W();
	~VtParserOSC_W();

public:
	virtual void Parse();

protected:
	void Args();
	void OscString();
};

