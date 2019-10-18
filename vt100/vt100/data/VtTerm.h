#pragma once
#include "VtCursor.h"

struct VtTerm
{
public:
	VtTerm();
	~VtTerm();

public:
	Size GetSize();
	VtCursor GetCursor();

public:
	void CheckBoundary(int row, int col);

protected:
	VtCursor m_cursor;
	Size m_size;

	VtTermBuffer m_buffer;
};

