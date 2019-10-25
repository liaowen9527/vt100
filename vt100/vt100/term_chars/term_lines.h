#pragma once
#include <deque>
#include "term_line.h"

class term_lines
{
public:
	term_lines();
	~term_lines();

public:
	int get_lines();
	term_line* get_line(int row);

	int find_last_nonempty_line();

public:
	term_line front();
	term_line back();

	void push_front(term_line& line);
	void push_back(term_line& line);

	term_line pop_front();
	term_line pop_back();

	void swap(term_lines& lines);

public:
	

protected:
	std::deque<term_line> m_lines;
	int m_lines_expect;
};