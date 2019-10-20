#include "VtCharTrans.h"

VtCharTrans::VtCharTrans()
{
	
}

VtCharTrans::~VtCharTrans()
{

}

void VtCharTrans::SetTerm(terminal_tag* p)
{
	term = p;
}

unsigned long VtCharTrans::Translate(unsigned char c)
{
	if (IsUtf())
	{
		return ByUtf();
	}
	else if (IsAcsMode())
	{
		return ByAcs();
	}
	
	return ByCharset();
}

unsigned long VtCharTrans::ByUtf(unsigned char c)
{
	switch (term->utf_state) 
	{
	case 0:
		ByUtf_0(c);
		break;
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
		ByUtf_1(c);
		break;
	default:
		break;
	}
}

unsigned long VtCharTrans::ByUtf_0(unsigned char c)
{
	if (c < 0x80) 
	{
		/* UTF-8 must be stateless so we ignore iso2022. */
		if (term->ucsdata->unitab_ctrl[c] != 0xFF) 
		{
			return term->ucsdata->unitab_ctrl[c];
		}
		else if ((term->utf8linedraw) &&
			(term->cset_attr[term->cset] == CSET_LINEDRW)) 
		{
			/* Linedraw characters are explicitly enabled */
			return c | CSET_LINEDRW;
		}
		else 
		{
			return c | CSET_ASCII;
		}
	}
	else if ((c & 0xe0) == 0xc0) 
	{
		utf8->size = utf8->state = 1;
		utf8->chr = (c & 0x1f);
	}
	else if ((c & 0xf0) == 0xe0) 
	{
		utf8->size = utf8->state = 2;
		utf8->chr = (c & 0x0f);
	}
	else if ((c & 0xf8) == 0xf0) 
	{
		utf8->size = utf8->state = 3;
		utf8->chr = (c & 0x07);
	}
	else if ((c & 0xfc) == 0xf8) 
	{
		utf8->size = utf8->state = 4;
		utf8->chr = (c & 0x03);
	}
	else if ((c & 0xfe) == 0xfc) 
	{
		utf8->size = utf8->state = 5;
		utf8->chr = (c & 0x01);
	}
	else 
	{
		return UCSINVALID;
	}

	return UCSINCOMPLETE;
}

unsigned long VtCharTrans::ByUtf_1(unsigned char c)
{
	if ((c & 0xC0) != 0x80) 
	{
		utf8->state = 0;
		return UCSTRUNCATED;   /* caller will then give us the
							   * same byte again */
	}
	utf8->chr = (utf8->chr << 6) | (c & 0x3f);
	if (--utf8->state)
	{
		return UCSINCOMPLETE;
	}

	unsigned long t = utf8->chr;

	/* Is somebody trying to be evil! */
	if (t < 0x80 ||
		(t < 0x800 && utf8->size >= 2) ||
		(t < 0x10000 && utf8->size >= 3) ||
		(t < 0x200000 && utf8->size >= 4) ||
		(t < 0x4000000 && utf8->size >= 5))
	{
		return UCSINVALID;
	}

	/* Unicode line separator and paragraph separator are CR-LF */
	if (t == 0x2028 || t == 0x2029)
	{
		return 0x85;
	}

	/* High controls are probably a Baaad idea too. */
	if (t < 0xA0)
	{
		return 0xFFFD;
	}

	/* The UTF-16 surrogates are not nice either. */
	/*       The standard give the option of decoding these:
	*       I don't want to! */
	if (t >= 0xD800 && t < 0xE000)
	{
		return UCSINVALID;
	}

	/* ISO 10646 characters now limited to UTF-16 range. */
	if (t > 0x10FFFF)
	{
		return UCSINVALID;
	}

	/* This is currently a TagPhobic application.. */
	if (t >= 0xE0000 && t <= 0xE007F)
	{
		return UCSINCOMPLETE;
	}

	/* U+FEFF is best seen as a null. */
	if (t == 0xFEFF)
	{
		return UCSINCOMPLETE;
	}

	/* But U+FFFE is an error. */
	if (t == 0xFFFE || t == 0xFFFF)
	{
		return UCSINVALID;
	}

	return t;
}

unsigned long VtCharTrans::ByAcs(unsigned char c)
{
	/* Are we in the nasty ACS mode? Note: no sco in utf mode. */
	if (term->sco_acs == 2)
	{
		c |= 0x80;
	}

	return c | CSET_SCOACS;
}

unsigned long VtCharTrans::ByCharset(unsigned char c)
{
	int attr = GetCharset();
	switch (term->cset_attr[term->cset]) 
	{
		/*
		* Linedraw characters are different from 'ESC ( B'
		* only for a small range. For ones outside that
		* range, make sure we use the same font as well as
		* the same encoding.
		*/
	case CSET_LINEDRW:
		if (term->ucsdata->unitab_ctrl[c] != 0xFF)
		{
			return term->ucsdata->unitab_ctrl[c];
		}
		else
		{
			return c | CSET_LINEDRW;
		}
		break;

	case CSET_GBCHR:
		/* If UK-ASCII, make the '#' a LineDraw Pound */
		if (c == '#')
		{
			return '}' | CSET_LINEDRW;
		}
		/* fall through */

	case CSET_ASCII:
		if (term->ucsdata->unitab_ctrl[c] != 0xFF)
		{
			return term->ucsdata->unitab_ctrl[c];
		}			
		else
		{
			return c | CSET_ASCII;
		}
		break;
	case CSET_SCOACS:
		if (c >= ' ')
		{
			return c | CSET_SCOACS;
		}
		break;
	}

	return c;
}

bool VtCharTrans::IsAcsMode()
{
	return term->sco_acs && (c != '\033' && c != '\012' && c != '\015' && c != '\b');
}

bool VtCharTrans::IsUtf()
{
	//return term->utf || term->ucsdata->line_codepage == CP_UTF8;
	return term->utf;
}

int VtCharTrans::GetCharset()
{
	return term->cset_attr[term->cset];
}

