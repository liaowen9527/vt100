#include "VtTermBuffer.h"
#include "VtLine.h"

VtTermBuffer::VtTermBuffer()
{

}

VtTermBuffer::~VtTermBuffer()
{

}

void VtTermBuffer::PushFront(VtTermLine& line)
{
	m_lines.push_front(line);
}

VtTermLine VtTermBuffer::PopFront()
{
	m_lines.pop_front();
}

void VtTermBuffer::PushBack(VtTermLine& line)
{
	m_lines.push_back(line);
}

VtTermLine VtTermBuffer::PopBack()
{
	return m_lines.pop_back();
}

void VtTermBuffer::CheckLineSize(int row)
{
	Size size = m_vtTerm->GetSize();
	VtTermLine* line = GetLine(row);

	if (size.cols == line->Cols())
	{
		line->Resize(size.cols);
	}
}

void VtTermBuffer::InsertChar(int nCount)
{
	nCount = std::min<int>(term->cols - 1, nCount);
	int m = term->cols - 1 - nCount;

	VtPos posCursor;
	VtSelection selection;
	
	VtPos posEol(0, 1);
	int nRow = 0;
	int nEndCol = 1;

	if (posCursor.le(selection.m_to) && posEol.le(selection.m_from))
	{
	}
}

VtTermLine* VtTermBuffer::GetLine(int row)
{
	VtTermLine& line = m_lines.at(row);

	return &line;
}

