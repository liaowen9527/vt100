#include "term_line.h"

term_line::term_line()
{

}

term_line::~term_line()
{

}

bool term_line::has_attr(int attr)
{
	return m_lattr.is_enable(attr);
}

void term_line::append_attr(int attr)
{
	m_lattr.enable(attr);
}

term_char term_line::get_char()
{
	return m_chars[col];
}

void term_line::set_char(int col, const term_char& tc)
{
	m_chars[col] = tc;
}

void term_line::check_size(int cols)
{
	if (m_cols != cols)
	{
		resize(cols);
	}
}

void term_line::resize(int cols)
{
	if (m_cols == cols) 
	{
		return;
	}
}

void term_line::add_cc(int col, unsigned long chr)
{
	
}

void term_line::clear_cc(int col)
{
	
}
