#pragma once
#include "VtParserBase.h"

class VtParserOSC_MaybeST : public VtParserBase
{
public:
	VtParserOSC_MaybeST();
	~VtParserOSC_MaybeST();

public:
	virtual void Parse();
};

