#include "term_line.h"

term_line::term_line()
{

}

term_line::~term_line()
{

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

	int oldcols = m_cols;

	/*
	 * This line is the wrong length, which probably means it
	 * hasn't been accessed since a resize. Resize it now.
	 *
	 * First, go through all the characters that will be thrown
	 * out in the resize (if we're shrinking the line) and
	 * return their cc lists to the cc free list.
	 */
	for (int i = cols; i < oldcols; i++)
	{
		clear_cc(i);
	}

	/*
	 * If we're shrinking the line, we now bodily move the
	 * entire cc section from where it started to where it now
	 * needs to be. (We have to do this before the resize, so
	 * that the data we're copying is still there. However, if
	 * we're expanding, we have to wait until _after_ the
	 * resize so that the space we're copying into is there.)
	 */
	if (cols < oldcols)
		memmove(m_chars + cols, m_chars + oldcols,
		(m_size - m_cols) * TSIZE);

	/*
	 * Now do the actual resize, leaving the _same_ amount of
	 * cc space as there was to begin with.
	 */
	m_size += cols - oldcols;
	m_chars = sresize(m_chars, m_size, TTYPE);
	m_cols = cols;

	/*
	 * If we're expanding the line, _now_ we move the cc
	 * section.
	 */
	if (cols > oldcols)
	{
		memmove(m_chars + cols, m_chars + oldcols, (m_size - m_cols) * TSIZE);
	}

	/*
	 * Go through what's left of the original line, and adjust
	 * the first cc_next pointer in each list. (All the
	 * subsequent ones are still valid because they are
	 * relative offsets within the cc block.) Also do the same
	 * to the head of the cc_free list.
	 */
	for (i = 0; i < oldcols && i < cols; i++)
		if (m_chars[i].cc_next)
			m_chars[i].cc_next += cols - oldcols;
	if (m_cc_free)
		m_cc_free += cols - oldcols;

	/*
	 * And finally fill in the new space with erase chars. (We
	 * don't have to worry about cc lists here, because we
	 * _know_ the erase char doesn't have one.)
	 */
	for (i = oldcols; i < cols; i++)
		m_chars[i] = term->basic_erase_char;

#ifdef TERM_CC_DIAGS
	cc_check(line);
#endif
}

void term_line::clear_cc(int col)
{
	assert(0 <= col && col < m_cols);

	int oldfree; 
	int origcol = col;

	if (!m_chars[col].cc_next())
	{
		return;                        /* nothing needs doing */
	}

	oldfree = m_cc_free;
	m_cc_free = col + m_chars[col].cc_next();

	while (m_chars[col].cc_next())
	{
		col += m_chars[col].cc_next;
	}
		
	if (oldfree)
	{
		m_chars[col].cc_next = oldfree - col;
	}
	else
	{
		m_chars[col].cc_next = 0;
	}

	m_chars[origcol].cc_next = 0;

#ifdef TERM_CC_DIAGS
	cc_check();
#endif
}
