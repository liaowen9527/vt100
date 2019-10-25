#include "term_lines.h"

term_lines::term_lines()
{

}

term_lines::~term_lines()
{

}

int term_lines::get_lines()
{
	return m_lines.size()
}

term_line* term_lines::get_line(int row)
{
	term_line& line = m_lines.at(row);

	return &line;
}

int term_lines::find_last_nonempty_line()
{
	return 0;
}

term_line term_lines::front()
{
	return m_lines.front();
}

term_line term_lines::back()
{
	return m_lines.back();
}

void term_lines::push_front(term_line& line)
{
	m_lines.push_front(line);
}

void term_lines::push_back(term_line& line)
{
	m_lines.push_back(line);
}

term_line term_lines::pop_front()
{
	term_line line = m_lines.front();
	m_lines.pop_front();

	return line;
}

term_line term_lines::pop_back()
{
	term_line line = m_lines.back();
	m_lines.pop_back();

	return line;
}

void term_lines::swap(term_lines& lines)
{
	m_lines.swap(lines.m_lines);
}
