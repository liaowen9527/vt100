#include "VtCursor.h"
#include "terminal_tag.h"

VtCursor::VtCursor()
{
	
}

VtCursor::~VtCursor()
{

}

void VtCursor::Get(int& row, int& col)
{
	row = m_tag.row;
	col = m_tag.col;
}

void VtCursor::Set(int row, int col)
{
	m_tag.row = row;
	m_tag.col = col;
}

int VtCursor::Row()
{
	return m_tag.row;
}

int VtCursor::Col()
{
	return m_tag.col;
}

void VtCursor::Move(int row, int col)
{
	row = m_tag.row - row;
	col = m_tag.col - col;

	MoveTo(row, col, false);
}

void VtCursor::MoveTo(int row, int col, bool visible)
{
	col = ToInsideCol(col);
	if (IsVisible() || visible)
	{
		row = ToVisibleRow(row);
	}

	row = ToInsideRow(row);

	m_tag.row = row;
	m_tag.col = col;
	m_tag.wrapnext = false;
}

void VtCursor::Save()
{
	m_save_tag = m_tag;
}

void VtCursor::Restore()
{
	m_tag = m_save_tag;
	int rows = term->rows;
	int cols = term->cols;

	/* Make sure the window hasn't shrunk since the save */
	m_tag.row = std::min<int>(m_tag.row, rows - 1);
	m_tag.col = std::min<int>(m_tag.col, cols - 1);

	/*
	* wrapnext might reset to False if the x position is no
	* longer at the rightmost edge.
	*/
	if (m_tag.wrapnext && m_tag.col < cols - 1)
	{
		m_tag.wrapnext = false;
	}
		
}

bool VtCursor::IsVisible()
{
	return term->IsVisibleRow(Row());
}

int VtCursor::ToInsideRow(int row)
{
	int rows = term->rows;

	row = std::max<int>(0, row);
	return std::min<int>(row, rows - 1);
}

int VtCursor::ToInsideCol(int col)
{
	int cols = term->cols;

	col = std::max<int>(0, col);
	return std::min<int>(col, cols - 1);
}

bool VtCursor::IsVisibleRow(int row)
{
	int top = term->marg_t;
	int bottom = term->marg_b;

	return top <= row && row <= bottom;
}

int VtCursor::ToVisibleRow(int row)
{
	int top = term->marg_t;
	int bottom = term->marg_b;

	row = std::max<int>(0, top);
	return std::min<int>(row, bottom);
}

