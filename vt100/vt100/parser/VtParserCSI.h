#pragma once
#include "VtParserBase.h"

class VtParserCSI : public VtParserBase
{
public:
	VtParserCSI();
	~VtParserCSI();

public:
	virtual void Parse();

protected:
	void Digit();
	void Semicolon();
	void Email();
	void Control();
};

