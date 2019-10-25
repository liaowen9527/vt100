#pragma once

class Postion
{
public:
	Postion(int _row = 0, int _col = 0);

public:
	bool lt(const Postion& _Right);
	bool le(const Postion& _Right);
	bool eq(const Postion& _Right);

public:
	int row;
	int col;
};


class Rect
{
public:
	Rect(const Postion& _start, const Postion& _end);

public:
	Postion start;
	Postion end;
};


class Size
{
public:
	Size(int _rows = 0, int _cols = 0);

public:
	bool InsideRow(int row);
	bool InsideCol(int col);

public:
	int rows;
	int cols;
};


class Margin
{
public:
	Margin(int _top = 0, int _bottom = 0);

public:
	int Lines();

	bool IsVaild();

	bool AtUp(int row);
	bool AtBottom(int row);

	bool IsVisibleRow(int row);

public:
	void PosUp(Postion& pos);
	void PosDown(Postion& pos);

public:
	int top;
	int bottom;
};