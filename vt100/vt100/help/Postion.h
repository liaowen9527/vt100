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