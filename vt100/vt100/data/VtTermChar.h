#pragma once
#include "VtData.h"

class VtTermChar : public VtData
{
public:
	VtTermChar();
	~VtTermChar();

public:
	

protected:
	unsigned long chr;
	unsigned long attr;
};