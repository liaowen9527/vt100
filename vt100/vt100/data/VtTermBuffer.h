#pragma once
#include <deque>
#include "VtData.h"
#include "VtTermLine.h"

class VtTermBuffer : public VtData
{
public:
	VtTermBuffer();
	~VtTermBuffer();

public:
	void CheckLineSize(int row);
	void InsertChar(int nCount);

protected:
	VtTermLine* GetLine(int row);

protected:
	std::deque<VtTermLine> m_lines;
};