#include "VtParserBase.h"
#include "../help/WCharWidth.h"

VtParserBase::VtParserBase()
{
	m_term = nullptr;
	c = 0;
}

VtParserBase::~VtParserBase()
{

}

void VtParserBase::SetTerm(VtTerm* p)
{
	term = p;
}

unsigned long VtParserBase::GetChar()
{
	return c;
}

void VtParserBase::Parse(char ch)
{
	c = ch;
}

void VtParserBase::Parse()
{
	
}

bool VtParserBase::CheckCompat(int vttype)
{
	if (!vt_type::IsCompat(OTHER))
	{
		term->termstate = TOPLEVEL;
		return false;
	}

	return true;
}

int VtParserBase::GetCharWidth(char c)
{
	int width = 0;
	if (IsChar(c))
	{
		width = 1;
	}

	if (!width)
	{
		width = (term->cjk_ambig_wide ? mk_wcwidth_cjk((unsigned int)c) : mk_wcwidth((unsigned int)c));
	}

	return width;
}

bool VtParserBase::IsChar(char c)
{
	return (c & 0xFFFFFC00) == 0xD800;
}

bool VtParserBase::IsFont(char c)
{
	return (c & 0xFFFFFE00) == 0xF000;
}
