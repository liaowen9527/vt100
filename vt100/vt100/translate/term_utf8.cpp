#include "term_utf8.h"

term_utf8::term_utf8()
	: m_state(0)
	, m_size(0)
	, m_chr(0)
{
	
}

term_utf8::~term_utf8()
{

}

int term_utf8::append(unsigned char c)
{
	if (m_state == 0)
	{
		return append_first_char(c);
	}

	return append_other_char(c);
}

int term_utf8::append_first_char(unsigned char c)
{
	int len = get_len_by_first_char(c);
	int chr = trans_first_char(c);

	if (len > 6)
	{
		return UCS_INVALID;
	}

	m_state = 1;
	m_size = len;
	m_chr = chr;

	return UCS_INCOMPLETE;
}

int term_utf8::append_other_char(unsigned char c)
{
	if (!is_other_char(c))
	{
		return UCS_TRUNCATED;   /* caller will then give us the same byte again */
	}

	int old = m_chr << 6;
	m_chr = old | trans_other_char(c);
	m_state++;

	if (!is_finish_by_state())
	{
		return UCS_INCOMPLETE;
	}

	return check(m_chr);
}

bool term_utf8::is_finish_by_state()
{
	return m_state >= m_size;
}

bool term_utf8::is_other_char(unsigned char c)
{
	return (c & 0xC0) == 0x80;
}

int term_utf8::get_len_by_first_char(unsigned char c)
{
	if (c < 0x80)	//[0, 128)
	{
		return 1;
	}
	if ((c & 0xe0) == 0xc0) //[0xc0, 0xe0)
	{
		return 2;
	}
	if ((c & 0xf0) == 0xe0) //[0xe0, 0xf0)
	{
		return 3;
	}
	else if ((c & 0xf8) == 0xf0) //[0xf0, 0xf8)
	{
		return 4;
	}
	else if ((c & 0xfc) == 0xf8) //[0xf8, 0xfc)
	{
		return 5;
	}
	else if ((c & 0xfe) == 0xfc) //[0xfc, 0xfe)
	{
		return 6;
	}

	return -1;
}

int term_utf8::trans_first_char(int size, unsigned char c)
{
	if (2 == size) 
	{
		return c & 0x1f;
	}
	else if ((c & 0xf0) == 0xe0) 
	{
		return c & 0x0f;
	}
	else if ((c & 0xf8) == 0xf0) 
	{
		return c & 0x07;
	}
	else if ((c & 0xfc) == 0xf8) 
	{
		return c & 0x03;
	}
	else if ((c & 0xfe) == 0xfc) 
	{
		return c & 0x01;
	}
	
	return UCS_INVALID;
}

int term_utf8::trans_other_char(unsigned char c)
{
	return c & 0x3f;
}

int term_utf8::check()
{
	int chr = m_chr;
	int size = m_size;

	if (is_bad_chr(chr, size) || is_utf16(chr) || is_iso_10646(chr) || is_0xfffe(chr))
	{
		return UCS_INVALID;
	}

	if (is_tag_phobic_app(chr) || is_0xfeff(chr))
	{
		return UCS_INCOMPLETE;
	}

	/* Unicode line separator and paragraph separator are CR-LF */
	if (is_crlf(chr))
	{
		return 0x85;
	}

	/* High controls are probably a Baaad idea too. */
	if (is_high_control(chr))
	{
		return 0xFFFD;
	}

	return chr;
}

bool term_utf8::is_bad_chr(int chr, int size)
{
	if (chr < 0x80)
	{
		return false;
	}

	if (chr < 0x800 && size >= 2)
	{
		return false;
	}

	if (chr < 0x10000 && size >= 3)
	{
		return false;
	}

	if (chr < 0x200000 && size >= 4)
	{
		return false;
	}

	if (chr < 0x4000000 && size >= 5)
	{
		return false;
	}

	return true;
}

bool term_utf8::is_crlf(int chr)
{
	return chr == 0x2028 || chr == 0x2029;
}

bool term_utf8::is_high_control(int chr)
{
	return chr < 0xA0;
}

bool term_utf8::is_utf16(int chr)
{
	return chr >= 0xD800 && chr < 0xE000;
}

bool term_utf8::is_iso_10646(int chr)
{
	return chr > 0x10FFFF;
}

bool term_utf8::is_tag_phobic_app(int chr)
{
	return chr >= 0xE0000 && chr <= 0xE007F;
}

bool term_utf8::is_0xfeff(int chr)
{
	return chr == 0xFEFF;
}

bool term_utf8::is_0xfffe(int chr)
{
	return chr == 0xFFFE || chr == 0xFFFF;
}
