#pragma once

enum VtMode 
{
	VT_XWINDOWS, 
	VT_OEMANSI, 
	VT_OEMONLY, 
	VT_POORMAN, 
	VT_UNICODE
};

class UnicodeData
{
public:
	UnicodeData();
	~UnicodeData();

public:
	int GetFontCodePage();
	void SetFontCodePage(int codepage);

	int GetLineCodePage();
	void SetLineCodePage(int codepage);

protected:
	void Init();

	void get_unitab(int codepage, wchar_t * unitab, int ftype);

protected:
	VtMode m_vtMode;
	char **m_uni_tbl;
	int m_dbcs_screenfont;
	int m_font_codepage;
	int m_line_codepage;

	wchar_t m_unitab_scoacs[256];
	wchar_t m_unitab_line[256];
	wchar_t m_unitab_font[256];
	wchar_t m_unitab_xterm[256];
	wchar_t m_unitab_oemcp[256];
	unsigned char m_unitab_ctrl[256];
};