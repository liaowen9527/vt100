#pragma once
#include "attr_line.h"
#include "attr_char.h"

class term_line
{
public:
	term_line();
	~term_line();

public:
	void check_size(int cols);

protected:
	void resize(int cols);

	void clear_cc(int col);
	void cc_check();

protected:
	attr_line m_lattr;
	int m_cols;                          /* number of real columns on the line */
	int m_size;                          /* number of allocated termchars
										* (cc-lists may make this > cols) */
	bool m_temporary;                    /* true if decompressed from scrollback */
	int m_cc_free;                       /* offset to first cc in free list */
	
	term_char* m_chars;
	bool m_trusted;
};