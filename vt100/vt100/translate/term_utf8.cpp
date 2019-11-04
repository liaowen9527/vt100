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

void term_utf8::set_translate(term_translate* trans)
{
	m_trans = trans;
}

unsigned long term_utf8::append(unsigned char c)
{
	if (m_state == 0)
	{
		return append_first_char(c);
	}

	return append_other_char(c);
}

unsigned long term_utf8::append_first_char(unsigned char c)
{
	int len = get_len_by_first_char(c);
	if (len > 6)
	{
		return UCS_INVALID;
	}

	int chr = trans_first_char(c);
	if (len == 1)
	{
		return chr;
	}

	m_state = 1;
	m_size = len;
	m_chr = chr;

	return UCS_INCOMPLETE;
}

unsigned long term_utf8::append_other_char(unsigned char c)
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

	return check();
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

unsigned long term_utf8::trans_first_char(unsigned char c)
{
	int len = get_len_by_first_char(c);

	if (1 == len)
	{
		return trans_first_char_ascii(c);
	}
	else if (2 == len) 
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

unsigned long term_utf8::trans_first_char_ascii(unsigned char c)
{
	int unitab_ctrl = get_unitab_ctrl(c);
	/* UTF-8 must be stateless so we ignore iso2022. */
	if (unitab_ctrl != 0xFF)
	{
		return unitab_ctrl;
	}
	else if (is_utf8linedraw())
	{
		/* Linedraw characters are explicitly enabled */
		return c | CSET_LINEDRW;
	}
	
	return c | CSET_ASCII;
}

int term_utf8::trans_other_char(unsigned char c)
{
	return c & 0x3f;
}

unsigned long term_utf8::check()
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

unsigned char term_utf8::get_unitab_ctrl(unsigned char c)
{
	return m_trans->get_unitab_ctrl(c);
}

bool term_utf8::is_utf8linedraw()
{
	return m_trans->is_utf8linedraw();
}
