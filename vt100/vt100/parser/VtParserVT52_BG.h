#pragma once
#include "VtParserBase.h"

class VtParserVT52_BG : public VtParserBase
{
public:
	VtParserVT52_BG();
	~VtParserVT52_BG();

public:
	virtual void Parse();
};

