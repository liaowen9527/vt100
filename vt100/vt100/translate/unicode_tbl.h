#pragma once
#include <vector>
#include "unicode_table.h"

class unicode_tbl
{
public:
	unicode_tbl();
	~unicode_tbl();

	void init(int rows, int cols, unicode_table* unitab_line);
	void clear();

	int get_rows();
	int get_cols();

protected:
	unicode_table* m_unitab_line;
	std::vector<std::vector<char>> m_uni_tbl;
};