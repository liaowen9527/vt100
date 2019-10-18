#include "Postion.h"

Postion::Postion(int _row /*= 0*/, int _col /*= 0*/)
	: row(_row)
	, col(_col)
{

}

bool Postion::lt(const Postion& _Right)
{
	if (row == _Right.row)
	{
		return col < _Right.col;
	}

	return row < _Right.row;
}

bool Postion::le(const Postion& _Right)
{
	if (row == _Right.row)
	{
		return col <= _Right.col;
	}

	return row < _Right.row;
}

bool Postion::eq(const Postion& _Right)
{
	return row == _Right.row && col == _Right.col;
}


Size::Size(int _rows /*= 0*/, int _cols /*= 0*/)
	: rows(_rows)
	, cols(_cols)
{

}

bool Size::InsideRow(int row)
{
	return 0 <= row && row < rows;
}

bool Size::InsideCol(int col)
{
	return 0 <= col && col < rows;
}
