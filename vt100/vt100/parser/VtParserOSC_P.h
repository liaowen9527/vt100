#pragma once
#include "VtParserBase.h"

class VtParserOSC_P : public VtParserBase
{
public:
	VtParserOSC_P();
	~VtParserOSC_P();

public:
	virtual void Parse();
};

