#include "unicode_data.h"
#include <windows.h>

/* Character conversion arrays; they are usually taken from windows,
 * the xterm one has the four scanlines that have no unicode 2.0
 * equivalents mapped to their unicode 3.0 locations.
 */
static const wchar_t s_unitab_xterm_std[32] = {
	0x2666, 0x2592, 0x2409, 0x240c, 0x240d, 0x240a, 0x00b0, 0x00b1,
	0x2424, 0x240b, 0x2518, 0x2510, 0x250c, 0x2514, 0x253c, 0x23ba,
	0x23bb, 0x2500, 0x23bc, 0x23bd, 0x251c, 0x2524, 0x2534, 0x252c,
	0x2502, 0x2264, 0x2265, 0x03c0, 0x2260, 0x00a3, 0x00b7, 0x0020
};

unicode_data::unicode_data()
{
	m_dbcs_screenfont = false;
	m_font_codepage = 0;
	m_line_codepage = 0;
}

unicode_data::~unicode_data()
{

}

void unicode_data::init()
{
	int vtmode = get_vtmode();

	int font_codepage = cal_font_codepage(vtmode);
	int line_codepage = cal_line_codepage(vtmode);
	bool dbcs_screenfont = cal_dbcs_screenfont(vtmode);

	set_font_codepage(font_codepage);
	set_line_codepage(line_codepage);
	set_dbcs_screenfont(dbcs_screenfont);


	bool used_dtf = false;

	init_unitab_font(vtmode);
	init_unitab_oemcp();
	init_unitab_scoacs(vtmode);
	init_unitab_line(vtmode);
	init_unitab_xterm();
	init_line_page_table(vtmode);
	init_unitab_ctrl();

	link_fonts(vtmode);

	if (dbcs_screenfont && font_codepage != line_codepage) 
	{
		m_unitab_line.set('\\', CSET_OEMCP + '\\');
	}

	poorman_links(vtmode);
}

bool unicode_data::is_utf8()
{
	return m_line_codepage == CP_UTF8;
}

int unicode_data::get_font_codepage()
{
	return m_font_codepage;
}

void unicode_data::set_font_codepage(int codepage)
{
	m_font_codepage = codepage;
}

int unicode_data::get_line_codepage()
{
	return m_line_codepage;
}

void unicode_data::set_line_codepage(int codepage)
{
	m_line_codepage = codepage;
}

bool unicode_data::get_dbcs_screenfont()
{
	return m_dbcs_screenfont;
}

void unicode_data::set_dbcs_screenfont(bool val)
{
	m_dbcs_screenfont = val;
}

unsigned char unicode_data::get_unitab_ctrl(unsigned char c)
{
	return m_unitab_ctrl[c];
}

unicode_table* unicode_data::get_unitab_scoacs()
{
	return &m_unitab_scoacs;
}

unicode_table* unicode_data::get_unitab_line()
{
	return &m_unitab_line;
}

unicode_table* unicode_data::get_unitab_font()
{
	return &m_unitab_font;
}

unicode_table* unicode_data::get_unitab_xterm()
{
	return &m_unitab_xterm;
}

unicode_table* unicode_data::get_unitab_oemcp()
{
	return &m_unitab_oemcp;
}

int unicode_data::get_vtmode()
{
	//return conf_get_int(conf, CONF_vtmode);
	return 0;
}

int unicode_data::cal_font_codepage(int vtmode)
{
	int ret = get_font_codepage();
	ret = (ret <= 0) ? 0 : ret;

	return vtmode == VT_OEMONLY ? 437 : ret;
}

int unicode_data::cal_line_codepage(int vtmode)
{
	/* Decide on the Line and Font codepages */
	//int codepage = decode_codepage(conf_get_str(conf, CONF_line_codepage));
	int codepage = 0;

	if (codepage <= 0)
	{
		if (vtmode == VT_OEMONLY)
		{
			codepage = GetACP();
		}
		else
		{
			codepage = get_font_codepage();
		}
	}

	return codepage;
}

bool unicode_data::cal_dbcs_screenfont(int vtmode)
{
	bool ret = get_dbcs_screenfont();

	if (vtmode == VT_OEMONLY || get_font_codepage() <= 0)
	{
		ret = false;
	}

	return ret;
}

void unicode_data::init_unitab_font(int vtmode)
{
	/* Collect screen font ucs table */
	bool dbcs_screenfont = get_dbcs_screenfont();
	int font_codepage = get_font_codepage();

	if (dbcs_screenfont || font_codepage == 0)
	{
		m_unitab_font.init_128(font_codepage, 1);
		m_unitab_font.init_some(128, 256, CSET_ACP);
	}
	else
	{
		m_unitab_font.init_256(font_codepage, 1);

		/* CP437 fonts are often broken ... */
		if (font_codepage == 437)
		{
			wchar_t val = 0xFFFF;
			m_unitab_font.set(0, val);
			m_unitab_font.set(255, val);
		}
	}
	
	if (vtmode == VT_XWINDOWS)
	{
		int count = sizeof(s_unitab_xterm_std) / sizeof(wchar_t);
		m_unitab_font.copy_some(1, s_unitab_xterm_std, count);
	}
}

void unicode_data::init_unitab_oemcp()
{
	/* Collect OEMCP ucs table */
	m_unitab_oemcp.init_256(CP_OEMCP, 1);
}

void unicode_data::init_unitab_scoacs(int vtmode)
{
	/* Collect CP437 ucs table for SCO acs */
	if (vtmode == VT_OEMANSI || vtmode == VT_XWINDOWS)
	{
		m_unitab_scoacs = m_unitab_oemcp;
	}
	else
	{
		m_unitab_scoacs.init_256(437, 1);
	}
}

void unicode_data::init_unitab_line(int vtmode)
{
	/* Collect line set ucs table */
	if (is_used_dtf(vtmode))
	{
		/* For DBCS and POOR fonts force direct to font */
		m_unitab_line.init_some(0, 32, 0);
		m_unitab_line.init_some(32, 256, CSET_ACP);
		m_unitab_line.set(127, 127);
	}
	else
	{
		int line_codepage = get_line_codepage();
		m_unitab_line.init_256(line_codepage, 0);
	}
}

void unicode_data::init_unitab_xterm()
{
	/* VT100 graphics - NB: Broken for non-ascii CP's */
	m_unitab_xterm = m_unitab_line;

	int count = sizeof(s_unitab_xterm_std) / sizeof(wchar_t);
	m_unitab_xterm.copy_some('`', s_unitab_xterm_std, count);
	m_unitab_xterm.set('_', ' ');
}

void unicode_data::init_line_page_table(int vtmode)
{
	/* Generate UCS ->line page table. */
	m_uni_tbl.clear();

	if (!is_used_dtf(vtmode)) 
	{
		m_uni_tbl.init(256, 256, &m_unitab_line);
	}
}

void unicode_data::init_unitab_ctrl()
{
	/* Find the line control characters. */
	for (int i = 0; i < 256; i++)
	{
		wchar_t ch = m_unitab_line.get(i);
		bool in_range = (0x7F <= ch && ch < 0xA0);

		unsigned char ctrl = (ch < ' ' || in_range) ? i : 0xFF;
		m_unitab_ctrl[i] = ctrl;
	}
}

void unicode_data::link_fonts(int vtmode)
{
	/* Generate line->screen direct conversion links. */
	if (vtmode == VT_OEMANSI || vtmode == VT_XWINDOWS)
	{
		m_unitab_scoacs.link_font(&m_unitab_oemcp, CSET_OEMCP);
	}

	m_unitab_line.link_font(&m_unitab_font, CSET_ACP);
	m_unitab_scoacs.link_font(&m_unitab_font, CSET_ACP);
	m_unitab_xterm.link_font(&m_unitab_font, CSET_ACP);

	if (vtmode == VT_OEMANSI || vtmode == VT_XWINDOWS)
	{
		m_unitab_line.link_font(&m_unitab_oemcp, CSET_OEMCP);
		m_unitab_xterm.link_font(&m_unitab_oemcp, CSET_OEMCP);
	}
}

static const char s_poorman_scoacs[] = "CueaaaaceeeiiiAAE**ooouuyOUc$YPsaiounNao?++**!<>###||||++||++++++--|-+||++--|-+----++++++++##||#aBTPEsyt******EN=+><++-=... n2* ";
static const char s_poorman_latin1[] = " !cL.Y|S\"Ca<--R~o+23'u|.,1o>///?AAAAAAACEEEEIIIIDNOOOOOxOUUUUYPBaaaaaaaceeeeiiiionooooo/ouuuuypy";
static const char s_poorman_vt100[] = "*#****o~**+++++-----++++|****L.";

void unicode_data::poorman_links(int vtmode)
{
	/* Last chance, if !unicode then try poorman links. */
	if (vtmode == VT_UNICODE)
	{
		return;
	}

	for (int i = 160; i < 256; i++)
	{
		wchar_t line = m_unitab_line.get(i);
		bool direct_font = is_direct_font(line);
		if (!direct_font && 160 <= line && line < 256)
		{
			int index = line - 160;
			wchar_t font = CSET_ACP + s_poorman_latin1[index];
			m_unitab_line.set(i, font);
		}
	}

	for (int i = 96; i < 127; i++)
	{
		wchar_t line = m_unitab_xterm.get(i);
		bool direct_font = is_direct_font(line);
		if (!direct_font)
		{
			int index = i - 96;
			wchar_t font = CSET_ACP + s_poorman_vt100[index];
			m_unitab_xterm.set(i, font);
		}
	}

	for (int i = 128; i < 256; i++)
	{
		wchar_t line = m_unitab_scoacs.get(i);
		bool direct_font = is_direct_font(line);
		if (!direct_font)
		{
			int index = i - 128;
			wchar_t font = CSET_ACP + s_poorman_scoacs[index];
			m_unitab_scoacs.set(i, font);
		}
	}
}

bool unicode_data::is_used_dtf(int vtmode)
{
	int line_codepage = get_line_codepage();
	int font_codepage = get_font_codepage();
	bool dbcs_screenfont = get_dbcs_screenfont();

	bool same_codepage = (line_codepage == font_codepage);
	bool cond2 = (dbcs_screenfont || vtmode == VT_POORMAN || font_codepage == 0);

	return same_codepage && cond2;
}
