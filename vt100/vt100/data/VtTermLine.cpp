#include "VtTermChars.h"
#include <assert.h>

VtTermLine::VtTermLine()
{

}

VtTermLine::~VtTermLine()
{

}

int VtTermLine::Cols()
{
	return m_chars.size();
}

void VtTermLine::Resize(int count)
{
	int cols = Cols();
	if (count == cols)
	{
		return;
	}


}

void VtTermLine::Clear()
{

}

void VtTermLine::Clear(int col)
{
	assert(col >=0 && col < Cols());


}



