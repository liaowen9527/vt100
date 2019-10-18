#pragma once
#include "VtParserBase.h"

class VtParserOSC_String : public VtParserBase
{
public:
	VtParserOSC_String();
	~VtParserOSC_String();

public:
	virtual void Parse();
};

