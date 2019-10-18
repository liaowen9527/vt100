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

void VtCharTrans::Translate(char c)
{
	m_orgin = c;
	SetResult(c);
	m_break = false;

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

bool VtCharTrans::IsBreak()
{
	return m_break;
}

void VtCharTrans::Break()
{
	m_break = true;
}

void VtCharTrans::SetResult(unsigned long result)
{
	m_result = result;
}

void VtCharTrans::ByUtf()
{
	switch (term->utf_state) 
	{
	case 0:
		ByUtf_0();
		break;
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
		ByUtf_1();
		break;
	default:
		break;
	}
}

void VtCharTrans::ByUtf_0()
{
	if (m_orgin < 0x80) 
	{
		/* UTF-8 must be stateless so we ignore iso2022. */
		if (term->ucsdata->unitab_ctrl[m_orgin] != 0xFF)
		{
			SetResult(term->ucsdata->unitab_ctrl[m_orgin]);
		}
		else
		{
			unsigned long result = ((unsigned char)m_orgin) | CSET_ASCII;
			SetResult(result);
		}
		
		return;
	}
	else if ((m_orgin & 0xe0) == 0xc0) 
	{
		term->utf_size = term->utf_state = 1;
		term->utf_char = (m_orgin & 0x1f);
	}
	else if ((m_orgin & 0xf0) == 0xe0) 
	{
		term->utf_size = term->utf_state = 2;
		term->utf_char = (m_orgin & 0x0f);
	}
	else if ((m_orgin & 0xf8) == 0xf0) 
	{
		term->utf_size = term->utf_state = 3;
		term->utf_char = (m_orgin & 0x07);
	}
	else if ((m_orgin & 0xfc) == 0xf8)
	{
		term->utf_size = term->utf_state = 4;
		term->utf_char = (m_orgin & 0x03);
	}
	else if ((m_orgin & 0xfe) == 0xfc)
	{
		term->utf_size = term->utf_state = 5;
		term->utf_char = (m_orgin & 0x01);
	}
	else 
	{
		m_orgin = UCSERR;
		return;
	}

	Break();
}

void VtCharTrans::ByUtf_1()
{
	if ((c & 0xC0) != 0x80)
	{
		unget = c;
		c = UCSERR;
		term->utf_state = 0;
		return;
	}

	term->utf_char = (term->utf_char << 6) | (c & 0x3f);
	if (--term->utf_state)
	{
		Break();
		return;
	}

	c = term->utf_char;

	/* Is somebody trying to be evil! */
	if (c < 0x80 ||
		(c < 0x800 && term->utf_size >= 2) ||
		(c < 0x10000 && term->utf_size >= 3) ||
		(c < 0x200000 && term->utf_size >= 4) ||
		(c < 0x4000000 && term->utf_size >= 5))
		c = UCSERR;

	/* Unicode line separator and paragraph separator are CR-LF */
	if (c == 0x2028 || c == 0x2029)
		c = 0x85;

	/* High controls are probably a Baaad idea too. */
	if (c < 0xA0)
		c = 0xFFFD;

	/* The UTF-16 surrogates are not nice either. */
	/*       The standard give the option of decoding these:
	 *       I don't want to! */
	if (c >= 0xD800 && c < 0xE000)
		c = UCSERR;

	/* ISO 10646 characters now limited to UTF-16 range. */
	if (c > 0x10FFFF)
		c = UCSERR;

	/* This is currently a TagPhobic application.. */
	if (c >= 0xE0000 && c <= 0xE007F)
	{
		Break();
		return;
	}

	/* U+FEFF is best seen as a null. */
	if (c == 0xFEFF)
	{
		Break();
		return;
	}

	/* But U+FFFE is an error. */
	if (c == 0xFFFE || c == 0xFFFF)
		c = UCSERR;
}

void VtCharTrans::ByAcs()
{
	if (term->sco_acs == 2)
	{
		c |= 0x80;
	}

	c |= CSET_SCOACS;
}

void VtCharTrans::ByCharset()
{
	int attr = GetCharset();
	switch (attr)
	{
		/*
		 * Linedraw characters are different from 'ESC ( B'
		 * only for a small range. For ones outside that
		 * range, make sure we use the same font as well as
		 * the same encoding.
		 */
	case CSET_LINEDRW:
		if (term->ucsdata->unitab_ctrl[c] != 0xFF)
			c = term->ucsdata->unitab_ctrl[c];
		else
			c = ((unsigned char)c) | CSET_LINEDRW;
		break;

	case CSET_GBCHR:
		/* If UK-ASCII, make the '#' a LineDraw Pound */
		if (c == '#') {
			c = '}' | CSET_LINEDRW;
			break;
		}
	/*FALLTHROUGH*/ case CSET_ASCII:
		if (term->ucsdata->unitab_ctrl[c] != 0xFF)
			c = term->ucsdata->unitab_ctrl[c];
		else
			c = ((unsigned char)c) | CSET_ASCII;
		break;
	case CSET_SCOACS:
		if (c >= ' ') c = ((unsigned char)c) | CSET_SCOACS;
		break;
	}
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

