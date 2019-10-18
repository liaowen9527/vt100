#pragma once
#include "VtParserBase.h"

class VtParserTopLevel : public VtParserBase
{
public:
	VtParserTopLevel();
	~VtParserTopLevel();

public:
	virtual void Parse();
};

