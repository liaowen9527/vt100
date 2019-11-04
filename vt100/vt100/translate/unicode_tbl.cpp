#include "unicode_tbl.h"

unicode_tbl::unicode_tbl()
{
	
}

unicode_tbl::~unicode_tbl()
{
	
}

void unicode_tbl::init(int rows, int cols, unicode_table* unitab_line)
{
	for (int i = 0; i < rows; i++) 
	{
		wchar_t temp = unitab_line->get(i);
		if (is_direct_char(temp) || is_direct_font(temp))
		{
			continue;
		}

		if (m_uni_tbl.empty())
		{
			m_uni_tbl.resize(rows);
		}

		int row = ((temp >> 8) & 0xFF);
		std::vector<char>& line = m_uni_tbl[row];
		if (line.empty())
		{
			line = std::vector<char>(cols, 0);
		}

		int col = temp & 0xFF;
		line[col] = i;
	}
}

void unicode_tbl::clear()
{
	m_uni_tbl.clear();
}

int unicode_tbl::get_rows()
{
	return m_uni_tbl.size();
}

int unicode_tbl::get_cols()
{
	if (m_uni_tbl.empty())
	{
		return 0;
	}

	std::vector<char>& line = m_uni_tbl.front();
	return line.size();
}
