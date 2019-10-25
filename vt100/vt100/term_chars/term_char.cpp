#include "char_chr.h"

term_char::term_char()
{

}

term_char::~term_char()
{

}

int term_char::cc_next()
{
	return m_cc_next;
}

bool term_char::is_wide()
{

}

wchar_t term_char::get_wchar()
{
	wchar_t ch = ' ';
	unsigned long cset = get_charset();
	switch (cset)
	{
	case CSET_ASCII:
		break;
	case CSET_LINEDRW:
		break;
	case CSET_SCOACS:
		break;
	}

	return ch;
}

unsigned long term_char::get_charset()
{
	return m_tchar & CSET_MASK;
}

int term_char::get_ucsdata_index()
{
	return m_tchar & 0xFF;
}

bool term_char::is_ascii()
{
	return get_charset() = CSET_ASCII;
}

bool term_char::is_linedraw()
{
	return get_charset() = CSET_LINEDRW;
}

bool term_char::is_scoacs()
{
	return get_charset() = CSET_SCOACS;
}

bool term_char::is_gbchr()
{
	return get_charset() = CSET_GBCHR;
}

bool term_char::is_mask()
{
	return get_charset();
}
