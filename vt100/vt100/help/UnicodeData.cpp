#include "UnicodeData.h"

UnicodeData::UnicodeData()
{
	m_line_codepage = 0;
	m_font_codepage = 0;
	m_dbcs_screenfont = 0;
}

UnicodeData::~UnicodeData()
{

}

int UnicodeData::GetFontCodePage()
{
	return m_font_codepage;
}

void UnicodeData::SetFontCodePage(int codepage)
{
	m_font_codepage = codepage;
}

int UnicodeData::GetLineCodePage()
{
	return m_line_codepage;
}

void UnicodeData::SetLineCodePage(int codepage)
{
	m_line_codepage = codepage;
}

void UnicodeData::Init()
{
	int i, j;
	int used_dtf = 0;
	int vtmode;

	/* Decide on the Line and Font codepages */
	

	VtMode vtmode = conf_get_int(conf, CONF_vtmode);
	if (vtmode == VT_OEMONLY) 
	{
		m_font_codepage = 437;
		m_dbcs_screenfont = 0;
		if (m_line_codepage <= 0)
		{
			m_line_codepage = GetACP();
		}
	}
	else if (m_line_codepage <= 0)
	{
		m_line_codepage = m_font_codepage;
	}
		

	/* Collect screen font ucs table */
	if (m_dbcs_screenfont || m_font_codepage == 0) 
	{
		get_unitab(m_font_codepage, m_unitab_font, 2);
		for (i = 128; i < 256; i++)
		{
			m_unitab_font[i] = (WCHAR)(CSET_ACP + i);
		}
	}
	else 
	{
		get_unitab(m_font_codepage, m_unitab_font, 1);

		/* CP437 fonts are often broken ... */
		if (m_font_codepage == 437)
			m_unitab_font[0] = m_unitab_font[255] = 0xFFFF;
	}
	if (vtmode == VT_XWINDOWS)
	{
		memcpy(m_unitab_font + 1, unitab_xterm_std, sizeof(unitab_xterm_std));
	}

	/* Collect OEMCP ucs table */
	get_unitab(CP_OEMCP, ucsdata->unitab_oemcp, 1);

	/* Collect CP437 ucs table for SCO acs */
	if (vtmode == VT_OEMANSI || vtmode == VT_XWINDOWS)
	{
		memcpy(ucsdata->unitab_scoacs, ucsdata->unitab_oemcp, sizeof(ucsdata->unitab_scoacs));
	}
	else
	{
		get_unitab(437, ucsdata->unitab_scoacs, 1);
	}

	/* Collect line set ucs table */
	if (m_line_codepage == m_font_codepage && (m_dbcs_screenfont || vtmode == VT_POORMAN || m_font_codepage == 0)) 
	{

		/* For DBCS and POOR fonts force direct to font */
		used_dtf = 1;
		for (i = 0; i < 32; i++)
			ucsdata->unitab_line[i] = (WCHAR)i;
		for (i = 32; i < 256; i++)
			ucsdata->unitab_line[i] = (WCHAR)(CSET_ACP + i);
		ucsdata->unitab_line[127] = (WCHAR)127;
	}
	else 
	{
		get_unitab(m_line_codepage, ucsdata->unitab_line, 0);
	}

	/* VT100 graphics - NB: Broken for non-ascii CP's */
	memcpy(ucsdata->unitab_xterm, ucsdata->unitab_line, sizeof(ucsdata->unitab_xterm));
	memcpy(ucsdata->unitab_xterm + '`', unitab_xterm_std, sizeof(unitab_xterm_std));
	ucsdata->unitab_xterm['_'] = ' ';

	/* Generate UCS ->line page table. */
	if (ucsdata->uni_tbl) 
	{
		for (i = 0; i < 256; i++)
		{
			if (ucsdata->uni_tbl[i])
			{
				sfree(ucsdata->uni_tbl[i]);
			}
		}
			
		sfree(ucsdata->uni_tbl);
		ucsdata->uni_tbl = 0;
	}
	if (!used_dtf) 
	{
		for (i = 0; i < 256; i++) 
		{
			if (DIRECT_CHAR(ucsdata->unitab_line[i]))
				continue;
			if (DIRECT_FONT(ucsdata->unitab_line[i]))
				continue;
			if (!ucsdata->uni_tbl) {
				ucsdata->uni_tbl = snewn(256, char *);
				memset(ucsdata->uni_tbl, 0, 256 * sizeof(char *));
			}
			j = ((ucsdata->unitab_line[i] >> 8) & 0xFF);
			if (!ucsdata->uni_tbl[j]) {
				ucsdata->uni_tbl[j] = snewn(256, char);
				memset(ucsdata->uni_tbl[j], 0, 256 * sizeof(char));
			}
			ucsdata->uni_tbl[j][ucsdata->unitab_line[i] & 0xFF] = i;
		}
	}

	/* Find the line control characters. */
	for (i = 0; i < 256; i++)
	{
		if (ucsdata->unitab_line[i] < ' '
			|| (ucsdata->unitab_line[i] >= 0x7F &&
				ucsdata->unitab_line[i] < 0xA0))
		{
			ucsdata->unitab_ctrl[i] = i;
		}
		else
		{
			ucsdata->unitab_ctrl[i] = 0xFF;
		}
	}
		

	/* Generate line->screen direct conversion links. */
	if (vtmode == VT_OEMANSI || vtmode == VT_XWINDOWS)
	{
		link_font(ucsdata->unitab_scoacs, ucsdata->unitab_oemcp, CSET_OEMCP);
	}

	link_font(ucsdata->unitab_line, m_unitab_font, CSET_ACP);
	link_font(ucsdata->unitab_scoacs, m_unitab_font, CSET_ACP);
	link_font(ucsdata->unitab_xterm, m_unitab_font, CSET_ACP);

	if (vtmode == VT_OEMANSI || vtmode == VT_XWINDOWS) 
	{
		link_font(ucsdata->unitab_line, ucsdata->unitab_oemcp, CSET_OEMCP);
		link_font(ucsdata->unitab_xterm, ucsdata->unitab_oemcp, CSET_OEMCP);
	}

	if (m_dbcs_screenfont &&
		m_font_codepage != m_line_codepage) 
	{
		/* F***ing Microsoft fonts, Japanese and Korean codepage fonts
		 * have a currency symbol at 0x5C but their unicode value is
		 * still given as U+005C not the correct U+00A5. */
		ucsdata->unitab_line['\\'] = CSET_OEMCP + '\\';
	}

	/* Last chance, if !unicode then try poorman links. */
	if (vtmode != VT_UNICODE) 
	{
		static const char poorman_scoacs[] =
			"CueaaaaceeeiiiAAE**ooouuyOUc$YPsaiounNao?++**!<>###||||++||++++++--|-+||++--|-+----++++++++##||#aBTPEsyt******EN=+><++-=... n2* ";
		static const char poorman_latin1[] =
			" !cL.Y|S\"Ca<--R~o+23'u|.,1o>///?AAAAAAACEEEEIIIIDNOOOOOxOUUUUYPBaaaaaaaceeeeiiiionooooo/ouuuuypy";
		static const char poorman_vt100[] = "*#****o~**+++++-----++++|****L.";

		for (i = 160; i < 256; i++)
			if (!DIRECT_FONT(ucsdata->unitab_line[i]) &&
				ucsdata->unitab_line[i] >= 160 &&
				ucsdata->unitab_line[i] < 256) {
				ucsdata->unitab_line[i] =
					(WCHAR)(CSET_ACP +
						poorman_latin1[ucsdata->unitab_line[i] - 160]);
			}
		for (i = 96; i < 127; i++)
			if (!DIRECT_FONT(ucsdata->unitab_xterm[i]))
				ucsdata->unitab_xterm[i] =
				(WCHAR)(CSET_ACP + poorman_vt100[i - 96]);
		for (i = 128; i < 256; i++)
			if (!DIRECT_FONT(ucsdata->unitab_scoacs[i]))
				ucsdata->unitab_scoacs[i] =
				(WCHAR)(CSET_ACP + poorman_scoacs[i - 128]);
	}
}

void UnicodeData::get_unitab(int codepage, wchar_t * unitab, int ftype)
{
	char tbuf[4];
	int i, max = 256, flg = MB_ERR_INVALID_CHARS;

	if (ftype) flg |= MB_USEGLYPHCHARS;
	if (ftype == 2) max = 128;
		
	if (codepage == CP_UTF8) 
	{
		for (i = 0; i < max; i++)
		{
			unitab[i] = i;
		}
		return;
	}

	if (codepage == CP_ACP)
	{
		codepage = GetACP();
	}
	else if (codepage == CP_OEMCP)
	{
		codepage = GetOEMCP();
	}

	if (codepage > 0 && codepage < 65536) 
	{
		for (i = 0; i < max; i++) 
		{
			tbuf[0] = i;

			if (mb_to_wc(codepage, flg, tbuf, 1, unitab + i, 1) != 1)
			{
				unitab[i] = 0xFFFD;
			}
		}
	}
	else 
	{
		int j = 256 - cp_list[codepage & 0xFFFF].cp_size;
		for (i = 0; i < max; i++)
		{
			unitab[i] = i;
		}
			
		for (i = j; i < max; i++)
		{
			unitab[i] = cp_list[codepage & 0xFFFF].cp_table[i - j];
		}
	}
}
