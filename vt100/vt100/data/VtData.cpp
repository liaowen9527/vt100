#include "VtData.h"

VtData::VtData()
{

}

VtData::~VtData()
{

}

VtTerm* VtData::GetTerm()
{
	return m_vtTerm;
}

void VtData::SetTerm(VtTerm* term)
{
	m_vtTerm = term;
}
