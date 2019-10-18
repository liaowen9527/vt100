#pragma once
#include "VtData.h"

struct cursor_tag
{
	int row;
	int col;
	int attr;
	int cset;
	int wrapnext;
	int cset_attr;
	int sco_acs;
};

class VtCursor : public VtData
{
public:
	VtCursor();
	~VtCursor();

public:
	void Get(int& row, int& col);
	void Set(int row, int col);

	int Row();
	int Col();

	void Move(int row, int col);
	void MoveTo(int row, int col, bool visible);

	void Save();
	void Restore();

public:
	int ToInsideRow(int row);
	int ToInsideCol(int col);

	bool IsVisibleRow(int row);
	int ToVisibleRow(int row);
	
protected:
	cursor_tag m_tag;
	cursor_tag m_save_tag;
};

