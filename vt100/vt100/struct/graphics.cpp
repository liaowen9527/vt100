#include "graphics.h"

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

Rect::Rect(const Postion& _start, const Postion& _end)
{
	start = _start;
	end = _end;
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

Margin::Margin(int _top /*= 0*/, int _bottom /*= 0*/)
	: top(_top)
	, bottom(_bottom)
{

}

int Margin::Lines()
{
	return bottom - top;
}

bool Margin::IsVaild()
{
	return top <= bottom;
}

bool Margin::AtUp(int row)
{
	return row < top;
}

bool Margin::AtBottom(int row)
{
	return row > bottom;
}

bool Margin::IsVisibleRow(int row)
{
	return top <= row && row <= bottom;
}

void Margin::PosUp(Postion& pos)
{
	if (IsVisibleRow(pos.row))
	{
		pos.row--;
		if (AtUp(top))
		{
			pos.row = top;
			pos.col = 0;
		}
	}
}

void Margin::PosDown(Postion& pos)
{
	if (IsVisibleRow(pos.row))
	{
		pos.row++;
		if (AtBottom(pos.row))
		{
			pos.row = bottom + 1;
			pos.col = 0;
		}
	}
}

