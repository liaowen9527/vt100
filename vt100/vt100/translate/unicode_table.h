#pragma once

extern bool is_direct_char(wchar_t ch);
extern bool is_direct_font(wchar_t ch);

class unicode_table
{
public:
	unicode_table();
	~unicode_table();

public:
	wchar_t get(int index);
	void set(int index, wchar_t val);

	int size();

public:
	void init_128(int codepage, int ftype);
	void init_256(int codepage, int ftype);

	void init_some(int begin, int end, wchar_t charset);
	void copy_some(int begin, const wchar_t* p, int count);

	void link_font(unicode_table* t2, wchar_t attr);

protected:
	void init_table(int codepage, int count, int ftype);
	void init_utf8(int count);
	void init_mb(int codepage, int count, int flgs);
	void init_other(int codepage, int count);

protected:
	int get_codepage(int codepage);

	int mb_to_wc(int codepage, int flags, const char *mbstr, int mblen, wchar_t *wcstr, int wclen);

protected:
	wchar_t m_unitab[256];
};