#pragma once

struct unicode_data 
{
	char **uni_tbl;
	bool dbcs_screenfont;
	int font_codepage;
	int line_codepage;
	wchar_t unitab_scoacs[256];
	wchar_t unitab_line[256];
	wchar_t unitab_font[256];
	wchar_t unitab_xterm[256];
	wchar_t unitab_oemcp[256];
	unsigned char unitab_ctrl[256];
};