#pragma once
#include "VtParserBase.h"

class VtParserVT52_FG : public VtParserBase
{
public:
	VtParserVT52_FG();
	~VtParserVT52_FG();

public:
	virtual void Parse();
};

