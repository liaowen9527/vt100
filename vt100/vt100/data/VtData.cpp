#include "VtData.h"

VtData::VtData()
{

}

VtData::~VtData()
{

}

VtTerm* VtData::GetTerm()
{
	return m_term;
}

void VtData::SetTerm(VtTerm* term)
{
	m_term = term;
}
