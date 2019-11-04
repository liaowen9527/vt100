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

void VtParserBase::SetContext(VtContext* context)
{
	m_context = context;

	m_term = &(m_context->term);

	m_cursor = &(m_context->cursorCtrl);
}

void VtParserBase::SetTerm(VtTerm* p)
{
	term = p;
}

unsigned long VtParserBase::GetChar()
{
	return m_char;
}

void VtParserBase::Parse(unsigned long c)
{
	m_char = c;

	Parse();
}

void VtParserBase::Parse()
{
	
}

bool VtParserBase::CheckCompat(int vttype)
{
	if (!vt_type::IsCompat(vttype))
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
