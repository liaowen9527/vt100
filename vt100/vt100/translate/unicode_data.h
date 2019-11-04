#pragma once
#include "unicode_table.h"
#include "unicode_tbl.h"

enum {
	VT_XWINDOWS, VT_OEMANSI, VT_OEMONLY, VT_POORMAN, VT_UNICODE
};

/* Like Linux use the F000 page for direct to font. */
#define CSET_OEMCP   0x0000F000UL      /* OEM Codepage DTF */
#define CSET_ACP     0x0000F100UL      /* Ansi Codepage DTF */

class unicode_data
{
public:
	unicode_data();
	~unicode_data();

public:
	void init();

public:
	bool is_utf8();

	int get_font_codepage();
	void set_font_codepage(int codepage);

	int get_line_codepage();
	void set_line_codepage(int codepage);

	bool get_dbcs_screenfont();
	void set_dbcs_screenfont(bool val);

	unsigned char get_unitab_ctrl(unsigned char c);
	unicode_table* get_unitab_scoacs();
	unicode_table* get_unitab_line();
	unicode_table* get_unitab_font();
	unicode_table* get_unitab_xterm();
	unicode_table* get_unitab_oemcp();

protected:
	int get_vtmode();
	int cal_font_codepage(int vtmode);
	int cal_line_codepage(int vtmode);
	bool cal_dbcs_screenfont(int vtmode);
	
protected:
	void init_unitab_font(int vtmode);
	void init_unitab_oemcp();
	void init_unitab_scoacs(int vtmode);
	void init_unitab_line(int vtmode);
	void init_unitab_xterm();
	void init_line_page_table(int vtmode);
	void init_unitab_ctrl();

	void link_fonts(int vtmode);
	void poorman_links(int vtmode);

	bool is_used_dtf(int vtmode);

protected:
	unicode_tbl m_uni_tbl;
	bool m_dbcs_screenfont;
	int m_font_codepage;
	int m_line_codepage;

	unicode_table m_unitab_scoacs;
	unicode_table m_unitab_line;
	unicode_table m_unitab_font;
	unicode_table m_unitab_xterm;
	unicode_table m_unitab_oemcp;
	unsigned char m_unitab_ctrl[256];
};