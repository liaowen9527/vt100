#include "term_translate.h"

term_translate::term_translate()
{
	
}

term_translate::~term_translate()
{

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

}

unsigned long term_translate::to_sco_acs(unsigned char c)
{
	if (term->sco_acs == 2)
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
	if (term->ucsdata->unitab_ctrl[c] != 0xFF)
	{
		return term->ucsdata->unitab_ctrl[c];
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
	if (term->ucsdata->unitab_ctrl[c] != 0xFF)
	{
		return term->ucsdata->unitab_ctrl[c];
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
	return true;
}

bool term_translate::in__sco_acs(unsigned char c)
{
	if (!term->sco_acs)
	{
		return false;
	}

	return c != '\033' && c != '\012' && c != '\015' && c != '\b';
}


