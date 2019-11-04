#include "term_translate.h"
#include "../data/VtTerm.h"

term_translate::term_translate()
{
	m_term = nullptr;
}

term_translate::~term_translate()
{

}

VtTerm* term_translate::get_term()
{
	return m_term;
}

void term_translate::set_term(VtTerm* term)
{
	m_term = term;
}

term_utf8* term_translate::get_utf8()
{
	return &m_utf8;
}

unicode_data* term_translate::get_ucsdata()
{
	return &m_ucsdata;
}

unsigned long term_translate::translate(unsigned char c)
{
	if (in_utf())
	{
		return m_utf8.append(c);
	}

	if (in__sco_acs(c))
	{
		return to_sco_acs(c);
	}

	return to_charset(c);
}

int term_translate::get_sco_acs()
{
	screen_tag* tag = m_term->Screen();
	return tag->sco_acs;
}

unsigned char term_translate::get_unitab_ctrl(unsigned char c)
{
	return m_ucsdata.get_unitab_ctrl(c);
}

bool term_translate::is_utf8linedraw()
{
	return m_term->Charset()->IsLineDraw();
}

unsigned long term_translate::to_sco_acs(unsigned char c)
{
	int sco_acs = get_sco_acs();
	if (sco_acs == 2)
	{
		c |= 0x80;
	}

	return c | CSET_SCOACS;
}

unsigned long term_translate::to_charset(unsigned char c)
{
	const int charset = 0;

	if (CSET_LINEDRW == charset)
	{
		return to_line_draw(c);
	}
	else if (CSET_GBCHR == charset)
	{
		return to_uk_ascii(c);
	}
	else if (CSET_ASCII == charset)
	{
		return to_ascii(c);
	}
	else if (CSET_SCOACS == charset)
	{
		return to_charset_sco_acs(c);
	}

	return c;
}

unsigned long term_translate::to_line_draw(unsigned char c)
{
	unsigned char unitab_ctrl = get_unitab_ctrl(c);
	if (unitab_ctrl != 0xFF)
	{
		return unitab_ctrl;
	}
		
	return c | CSET_LINEDRW;
}

unsigned long term_translate::to_uk_ascii(unsigned char c)
{
	/* If UK-ASCII, make the '#' a LineDraw Pound */
	if (c == '#')
	{
		return '}' | CSET_LINEDRW;
	}

	return to_ascii(c);
}

unsigned long term_translate::to_ascii(unsigned char c)
{
	unsigned char unitab_ctrl = get_unitab_ctrl(c);
	if (unitab_ctrl != 0xFF)
	{
		return unitab_ctrl;
	}
		
	return c | CSET_ASCII;
}

unsigned long term_translate::to_charset_sco_acs(unsigned char c)
{
	if (c >= ' ')
	{
		return c | CSET_SCOACS;
	}
	
	return c;
}

bool term_translate::in_utf()
{
	screen_tag* tag = m_term->Screen();
	return tag->utf && m_ucsdata.is_utf8();
}

bool term_translate::in__sco_acs(unsigned char c)
{
	int sco_acs = get_sco_acs();
	if (sco_acs == 0)
	{
		return false;
	}

	return c != '\033' && c != '\012' && c != '\015' && c != '\b';
}



